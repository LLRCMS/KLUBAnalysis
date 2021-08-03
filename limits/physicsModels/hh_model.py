# coding: utf-8

"""
Custom HH physics model implementing gluon gluon fusion (ggf / gghh), vector boson fusion
(vbf / qqhh) and V boson associated production (vhh) modes.

Authors:
  - Luca Cadamuro
  - Fabio Monti
  - Marcel Rieger
"""

__all__ = [
    # samples
    "HHSample", "GGFSample", "VBFSample", "VHHSample", "ggf_samples", "vbf_samples", "vhh_samples",
    # formulae
    "HHFormula", "GGFFormula", "VBFFormula", "VHHFormula",
    # br and h scaling
    "SM_HIGG_DECAYS", "SM_HIGG_PROD", "coeffs_br", "cxs_13", "ewk_13", "dZH", "HBRScaler",
    # model
    "HHModelBase", "HHModel", "create_model", "model_all", "model_default", "model_default_vhh",
    "model_no_ggf_kl0", "model_no_ggf_kl1", "model_no_ggf_kl2p45", "model_no_ggf_kl5",
    "model_no_vbf_sm", "model_no_vbf_kl0", "model_no_vbf_kl2", "model_no_vbf_C2V0",
    "model_no_vbf_C2V2", "model_no_vbf_CV0p5", "model_no_vbf_CV1p5",
    # xsec helpers
    "ggf_k_factor", "ggf_kl_coeffs", "create_ggf_xsec_str", "create_ggf_xsec_func",
    "create_vbf_xsec_func", "create_vhh_xsec_func", "create_hh_xsec_func", "get_ggf_xsec",
    "get_vbf_xsec", "get_vhh_xsec", "get_hh_xsec",
]


import os
import re
from collections import OrderedDict, defaultdict

import sympy

from HiggsAnalysis.CombinedLimit.PhysicsModel import PhysicsModelBase
from HiggsAnalysis.CombinedLimit.SMHiggsBuilder import SMHiggsBuilder


no_value = object()

# default data directory of the SMHiggsBuilder, as used by the HBRScaler
if "DHI_SOFTWARE" in os.environ:
    default_data_dir = "$DHI_SOFTWARE/HiggsAnalysis/CombinedLimit/data/lhc-hxswg"
else:
    default_data_dir = "$CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/data/lhc-hxswg"


####################################################################################################
### Samples
####################################################################################################

class HHSample(object):
    """
    Base class for samples that currently only stores a cross section *xs* in pb and a sample
    *label*.
    """

    # regular expression to check sample labels
    # to be overwritten in subclasses
    label_re = None

    def __init__(self, xs, label):
        super(HHSample, self).__init__()

        # check the label
        if not re.match(self.label_re, label):
            raise Exception("{} label '{}' does not match configured format {}".format(
                self.__class__.__name__, label, self.label_re))

        self.xs = xs
        self.label = label

    def matches_process(self, process):
        """
        Returns *True* when the *process* matches the sample label, and *False* otherwise.
        """
        return process.startswith(self.label)


class GGFSample(HHSample):
    """
    Class describing ggf samples, characterized by values of *kl* and *kt*.
    """

    label_re = r"^ggHH_kl_([pm0-9]+)_kt_([pm0-9]+)$"

    def __init__(self, kl, kt, xs, label):
        super(GGFSample, self).__init__(xs, label)

        self.kl = kl
        self.kt = kt


class VBFSample(HHSample):
    """
    Class describing vbf samples, characterized by values of *CV* (kV), *C2V* (k2V) and *kl*.
    """

    label_re = r"qqHH_CV_([pm0-9]+)_C2V_([pm0-9]+)_kl_([pm0-9]+)$"

    def __init__(self, CV, C2V, kl, xs, label):
        super(VBFSample, self).__init__(xs, label)

        self.CV = CV
        self.C2V = C2V
        self.kl = kl


class VHHSample(HHSample):
    """
    Class describing vhh samples, characterized by values of *CV* (kV), *C2V* (k2V) and *kl*.
    """

    label_re = r"VHH_CV_([pm0-9]+)_C2V_([pm0-9]+)_kl_([pm0-9]+)$"

    def __init__(self, CV, C2V, kl, xs, label):
        super(VHHSample, self).__init__(xs, label)

        self.CV = CV
        self.C2V = C2V
        self.kl = kl


# ggf samples with keys (kl, kt)
# cross section values are NLO with k-factor applied and only used in create_ggf_xsec_func below
ggf_samples = OrderedDict([
    ((0.0,  1.0), GGFSample(kl=0.0,  kt=1.0, xs=0.069725, label="ggHH_kl_0_kt_1")),
    ((1.0,  1.0), GGFSample(kl=1.0,  kt=1.0, xs=0.031047, label="ggHH_kl_1_kt_1")),
    ((2.45, 1.0), GGFSample(kl=2.45, kt=1.0, xs=0.013124, label="ggHH_kl_2p45_kt_1")),
    ((5.0,  1.0), GGFSample(kl=5.0,  kt=1.0, xs=0.091172, label="ggHH_kl_5_kt_1")),
])

# vbf samples with keys (CV, C2V, kl)
# cross section values are LO (from 2017/2018 gridpacks) x SM k-factor for N3LO (1.03477) and are
# only used in create_vbf_xsec_func below
vbf_samples = OrderedDict([
    ((1.0, 1.0, 1.0), VBFSample(CV=1.0, C2V=1.0, kl=1.0, xs=0.0017260, label="qqHH_CV_1_C2V_1_kl_1")),
    ((1.0, 1.0, 0.0), VBFSample(CV=1.0, C2V=1.0, kl=0.0, xs=0.0046089, label="qqHH_CV_1_C2V_1_kl_0")),
    ((1.0, 1.0, 2.0), VBFSample(CV=1.0, C2V=1.0, kl=2.0, xs=0.0014228, label="qqHH_CV_1_C2V_1_kl_2")),
    ((1.0, 0.0, 1.0), VBFSample(CV=1.0, C2V=0.0, kl=1.0, xs=0.0270800, label="qqHH_CV_1_C2V_0_kl_1")),
    ((1.0, 2.0, 1.0), VBFSample(CV=1.0, C2V=2.0, kl=1.0, xs=0.0142178, label="qqHH_CV_1_C2V_2_kl_1")),
    ((0.5, 1.0, 1.0), VBFSample(CV=0.5, C2V=1.0, kl=1.0, xs=0.0108237, label="qqHH_CV_0p5_C2V_1_kl_1")),
    ((1.5, 1.0, 1.0), VBFSample(CV=1.5, C2V=1.0, kl=1.0, xs=0.0660185, label="qqHH_CV_1p5_C2V_1_kl_1")),
])

# vhh samples with keys (CV, C2V, kl)
# cross section values are LO (no k-factor applied) and are only used in create_vhh_xsec_func below
vhh_samples = OrderedDict([
    ((1.0, 1.0, 1.0), VHHSample(CV=1.0, C2V=1.0, kl=1.0, xs=0.0008095, label="VHH_CV_1_C2V_1_kl_1")),
    ((1.0, 1.0, 2.0), VHHSample(CV=1.0, C2V=1.0, kl=2.0, xs=0.0013204, label="VHH_CV_1_C2V_1_kl_2")),
    ((1.0, 0.0, 1.0), VHHSample(CV=1.0, C2V=0.0, kl=1.0, xs=0.0002850, label="VHH_CV_1_C2V_0_kl_1")),
    ((1.0, 2.0, 1.0), VHHSample(CV=1.0, C2V=2.0, kl=1.0, xs=0.0021291, label="VHH_CV_1_C2V_2_kl_1")),
    ((0.5, 1.0, 1.0), VHHSample(CV=0.5, C2V=1.0, kl=1.0, xs=0.0005371, label="VHH_CV_0p5_C2V_1_kl_1")),
    ((1.5, 1.0, 1.0), VHHSample(CV=1.5, C2V=1.0, kl=1.0, xs=0.0017450, label="VHH_CV_1p5_C2V_1_kl_1")),
])


####################################################################################################
### Symbolic cross section formulae
####################################################################################################

class HHFormula(object):
    """
    Base class for scaling formulae used to compute cross sections of various samples, depending on
    various coupling strengths given in a list *samples* containing :py:class:`Sample` instances of
    a specific type. All computed values are in units of pb.
    """

    # to be overwritten in subclasses
    sample_cls = None
    min_samples = None
    channel = None
    r_poi = None
    couplings = None

    @classmethod
    def check_samples(cls, samples):
        for sample in samples:
            if not isinstance(sample, cls.sample_cls):
                raise ValueError("{} expects samples to be of type {}, but got {}".format(
                    cls.__name__, cls.sample_cls.__name__, sample))

        if len(samples) < cls.min_samples:
            raise ValueError("{} expects at least {} samples, but got {}".format(
                cls.__name__, cls.min_samples, len(samples)))

    def __init__(self, samples):
        super(HHFormula, self).__init__()

        # check sample classes
        self.check_samples(samples)
        self.samples = list(samples)  # list of samples

        # symbolic expressions
        self.M = None  # the matrix to be inverted
        self.coeffs = None  # scaling coefficients per sample
        self.sigma = None  # cross section

        # eagerly build all matrix, coefficient and cross section expressions
        self.build_expressions()

    @property
    def n_samples(self):
        return len(self.samples)

    def build_expressions(self):
        """
        This method is supposed to define the invertible matrix :py:attr:`M`, as well as symbolic
        expressions of the per-sample coefficients :py:attr:`coeffs` and the cross section
        :py:attr:`sigma` itself.
        To be implemented by subclasses.
        """
        raise NotImplementedError


class GGFFormula(HHFormula):
    """
    Scaling formula for ggf samples, based on a n_samples x 3 matrix.
    """

    sample_cls = GGFSample
    min_samples = 3
    channel = "ggf"
    r_poi = "r_gghh"
    couplings = ["kl", "kt"]

    def build_expressions(self):
        # define the matrix with three scalings - box, triangle, interf
        self.M = sympy.Matrix([
            [
                sample.kt**4,
                sample.kt**2 * sample.kl**2,
                sample.kt**3 * sample.kl,
            ]
            for i, sample in enumerate(self.samples)
        ])

        # the vector of couplings
        kl, kt, box, tri, interf = sympy.symbols("kl kt box tri interf")
        c = sympy.Matrix([
            [kt**4],
            [kt**2 * kl**2],
            [kt**3 * kl],
        ])

        # the vector of symbolic sample cross sections
        s = sympy.Matrix([
            [sympy.Symbol("xs{}".format(i))]
            for i in range(self.n_samples)
        ])

        # actual computation, i.e., matrix inversion and multiplications with vectors
        M_inv = self.M.pinv()
        self.coeffs = c.transpose() * M_inv
        self.sigma = self.coeffs * s


class VBFFormula(HHFormula):
    """
    Scaling formula for vbf samples, based on a n_samples x 6 matrix.
    """

    sample_cls = VBFSample
    min_samples = 6
    channel = "vbf"
    r_poi = "r_qqhh"
    couplings = ["CV", "C2V", "kl"]

    def build_expressions(self):
        # define the matrix with three scalings - box, triangle, interf
        self.M = sympy.Matrix([
            [
                sample.CV**2 * sample.kl**2,
                sample.CV**4,
                sample.C2V**2,
                sample.CV**3 * sample.kl,
                sample.CV * sample.C2V * sample.kl,
                sample.CV**2 * sample.C2V,
            ]
            for i, sample in enumerate(self.samples)
        ])

        # the vector of couplings
        CV, C2V, kl, a, b, c, iab, iac, ibc = sympy.symbols("CV C2V kl a b c iab iac ibc")
        c = sympy.Matrix([
            [CV**2 * kl**2],
            [CV**4],
            [C2V**2],
            [CV**3 * kl],
            [CV * C2V * kl],
            [CV**2 * C2V],
        ])

        # the vector of symbolic sample cross sections
        s = sympy.Matrix([
            [sympy.Symbol("xs{}".format(i))]
            for i in range(self.n_samples)
        ])

        # actual computation, i.e., matrix inversion and multiplications with vectors
        M_inv = self.M.pinv()
        self.coeffs = c.transpose() * M_inv
        self.sigma = self.coeffs * s


class VHHFormula(VBFFormula):
    """
    Scaling formula for vhh samples, based on a n_samples x 6 matrix. Currently, this formula is
    neglecting differences between W and Z bosons which makes it identical to the
    :py:class:`VBFFormula`.
    """

    sample_cls = VHHSample
    min_samples = 6
    channel = "vhh"
    r_poi = "r_vhh"
    couplings = ["CV", "C2V", "kl"]


####################################################################################################
### BR and single H scaling
####################################################################################################

# H decay names that are supported in the br scaling
SM_HIGG_DECAYS = ["hww", "hzz", "hgg", "htt", "hbb", "hzg", "hmm", "hcc", "hgluglu", "hss"]

# single H production modes that are supported in the scaling
SM_HIGG_PROD = ["ggZH", "tHq", "tHW", "ggH", "qqH", "ZH", "WH", "VH", "ttH"]

# coefficients for the BR scaling with kl (from https://arxiv.org/abs/1709.08649, Eq 22)
coeffs_br = {
    "hgg": 0.49e-2,
    "hzz": 0.83e-2,
    "hww": 0.73e-2,
    "hgluglu": 0.66e-2,
    "htt": 0,
    "hbb": 0,
    "hcc": 0,
    "hss": 0,
    "hmm": 0,
}

# coefficients for the single H scaling with kl
# WH and ZH coeff are very similar, so use their average
cxs_13 = {
    "ggH": 0.66e-2,
    "qqH": 0.64e-2,
    "WH": 1.03e-2,
    "ZH": 1.19e-2,
    "ttH": 3.51e-2,
    "VH": (0.5 * (1.03e-2 + 1.19e-2)),
}
ewk_13 = {
    "ggH": 1.049,
    "qqH": 0.932,
    "WH": 0.93,
    "ZH": 0.947,
    "ttH": 1.014,
    "VH": (0.5 * (0.93 + 0.947)),
}
dZH = -1.536e-3


class HBRScaler(object):
    """
    Class to produce BR scalings for anomalous couplings and XS * BR scalings for both H and HH.
    """

    def __init__(self, model_builder, scale_br=False, scale_h=False, data_dir=default_data_dir):
        super(HBRScaler, self).__init__()

        # setup and store combine related objects
        self.model_builder = model_builder
        data_dir = data_dir and os.path.expandvars(data_dir)
        self.higgs_builder = SMHiggsBuilder(self.model_builder, datadir=data_dir)

        # store names of expressions that model the scalings
        self.br_scalings = []
        self.h_scalings = []

        # build expressions right away
        if scale_br:
            self.build_br_scalings()
        if scale_h:
            self.build_h_scalings()

    def make_expr(self, *args, **kwargs):
        """
        Shorthand for :py:meth:`model_builder.factory_`.
        """
        return self.model_builder.factory_(*args, **kwargs)

    def get_expr(self, *args, **kwargs):
        """
        Shorthand for :py:meth:`model_builder.out.function`.
        """
        return self.model_builder.out.function(*args, **kwargs)

    def make_br(self, *args, **kwargs):
        """
        Shorthand for :py:meth:`higgs_builder.makeBR`.
        """
        return self.higgs_builder.makeBR(*args, **kwargs)

    def make_scaling(self, *args, **kwargs):
        """
        Shorthand for :py:meth:`higgs_builder.makeScaling`.
        """
        return self.higgs_builder.makeScaling(*args, **kwargs)

    def build_br_scalings(self):
        """
        Registers all expressions required for the BR scaling.
        """
        # ensure all br's of the H decay exist
        for d in SM_HIGG_DECAYS:
            self.make_br(d)

            # keep BR uncertainties frozen for now
            self.make_expr("HiggsDecayWidth_UncertaintyScaling_{}[1.0]".format(d))

        # define resolved loops
        self.make_scaling("hgluglu", Cb="1", Ctop="kt")
        self.make_scaling("hgg", Cb="1", Ctop="kt", CW="CV", Ctau="1")
        self.make_scaling("hzg", Cb="1", Ctop="kt", CW="CV", Ctau="1")

        # BR scaling vs kl (https://arxiv.org/abs/1709.08649 Eq 22)
        for d, c1 in coeffs_br.items():
            self.make_expr("expr::kl_scalBR_{}('(@0 - 1) * {}', kl)".format(d, c1))

        # define partial widths as a function of kl, kt, and CV
        self.make_expr("expr::CVktkl_Gscal_Z('(@0 * @0 + @3) * @1 * @2', CV, SM_BR_hzz, HiggsDecayWidth_UncertaintyScaling_hzz, kl_scalBR_hzz)")
        self.make_expr("expr::CVktkl_Gscal_W('(@0 * @0 + @3) * @1 * @2', CV, SM_BR_hww, HiggsDecayWidth_UncertaintyScaling_hww, kl_scalBR_hww)")
        self.make_expr("expr::CVktkl_Gscal_tau('(1 + @4) * @0 * @2 + (1 + @5) * @1 * @3', SM_BR_htt, SM_BR_hmm, HiggsDecayWidth_UncertaintyScaling_htt, HiggsDecayWidth_UncertaintyScaling_hmm,kl_scalBR_htt, kl_scalBR_hmm)")
        self.make_expr("expr::CVktkl_Gscal_top('(1 + @2) * @0 * @1', SM_BR_hcc, HiggsDecayWidth_UncertaintyScaling_hcc, kl_scalBR_hcc)")
        self.make_expr("expr::CVktkl_Gscal_bottom('(1 + @3) * (@0 * @2 + @1)', SM_BR_hbb, SM_BR_hss, HiggsDecayWidth_UncertaintyScaling_hbb, kl_scalBR_hbb)")
        self.make_expr("expr::CVktkl_Gscal_gluon('(@0 + @3) * @1 * @2', Scaling_hgluglu, SM_BR_hgluglu, HiggsDecayWidth_UncertaintyScaling_hgluglu, kl_scalBR_hgluglu)")
        self.make_expr("expr::CVktkl_Gscal_gamma('(@0 + @6) * @1 * @4 + @2 * @3 * @5', Scaling_hgg, SM_BR_hgg, Scaling_hzg, SM_BR_hzg, HiggsDecayWidth_UncertaintyScaling_hgg, HiggsDecayWidth_UncertaintyScaling_hzg, kl_scalBR_hgg)")

        # fix to have all BRs add up to unity
        self.make_expr("sum::CVktkl_SMBRs({})".format(", ".join("SM_BR_" + d for d in SM_HIGG_DECAYS)))

        # total width, normalized to SM
        # (just the sum over the partial widths/SM total BR)
        self.make_expr("expr::CVktkl_Gscal_tot('(@0 + @1 + @2 + @3 + @4 + @5 + @6) / @7', CVktkl_Gscal_Z, CVktkl_Gscal_W, CVktkl_Gscal_tau, CVktkl_Gscal_top, CVktkl_Gscal_bottom, CVktkl_Gscal_gluon, CVktkl_Gscal_gamma, CVktkl_SMBRs)")

        # BRs, normalized to SM
        # (scaling as (partial/partial_SM) / (total/total_SM))
        self.make_expr("expr::CVktkl_BRscal_hww('(@0 * @0 + @3) * @2 / @1', CV, CVktkl_Gscal_tot, HiggsDecayWidth_UncertaintyScaling_hww, kl_scalBR_hww)")
        self.make_expr("expr::CVktkl_BRscal_hzz('(@0 * @0 + @3) * @2 / @1', CV, CVktkl_Gscal_tot, HiggsDecayWidth_UncertaintyScaling_hzz, kl_scalBR_hzz)")
        self.make_expr("expr::CVktkl_BRscal_htt('(1 + @2) * @1 / @0', CVktkl_Gscal_tot, HiggsDecayWidth_UncertaintyScaling_htt, kl_scalBR_htt)")
        self.make_expr("expr::CVktkl_BRscal_hmm('(1 + @2) * @1 / @0', CVktkl_Gscal_tot, HiggsDecayWidth_UncertaintyScaling_hmm, kl_scalBR_hmm)")
        self.make_expr("expr::CVktkl_BRscal_hbb('(1 + @2) * @1 / @0', CVktkl_Gscal_tot, HiggsDecayWidth_UncertaintyScaling_hbb, kl_scalBR_hbb)")
        self.make_expr("expr::CVktkl_BRscal_hcc('(1 + @2) * @1 / @0', CVktkl_Gscal_tot, HiggsDecayWidth_UncertaintyScaling_hcc, kl_scalBR_hcc)")
        self.make_expr("expr::CVktkl_BRscal_hss('(1 + @2) * @1 / @0', CVktkl_Gscal_tot, HiggsDecayWidth_UncertaintyScaling_hss, kl_scalBR_hss)")
        self.make_expr("expr::CVktkl_BRscal_hgg('(@0 + @3) * @2 / @1', Scaling_hgg, CVktkl_Gscal_tot, HiggsDecayWidth_UncertaintyScaling_hgg,kl_scalBR_hgg)")
        self.make_expr("expr::CVktkl_BRscal_hzg('@0 * @2 / @1', Scaling_hzg, CVktkl_Gscal_tot, HiggsDecayWidth_UncertaintyScaling_hzg)")
        self.make_expr("expr::CVktkl_BRscal_hgluglu('(@0 + @3) * @2 / @1', Scaling_hgluglu, CVktkl_Gscal_tot, HiggsDecayWidth_UncertaintyScaling_hgluglu, kl_scalBR_hgluglu)")

        # store the final scaling expression names
        for d in SM_HIGG_DECAYS:
            self.br_scalings.append("CVktkl_BRscal_{}".format(d))

    def build_h_scalings(self, ecm="13TeV"):
        """
        Registers all expressions required for the single H scaling.
        """
        # get VBF, tHq, tHW, ggZH cross section and resolved loops
        self.make_scaling("qqH", CW="CV", CZ="CV")
        self.make_scaling("tHq", CW="CV", Ctop="kt")
        self.make_scaling("tHW", CW="CV", Ctop="kt")
        self.make_scaling("ggZH", CZ="CV", Ctop="kt", Cb="1")
        self.make_scaling("ggH", Cb="1", Ctop="kt", Cc="1")

        # create scalings for different production processes which require different formulae
        for p in ["ggH", "qqH"]:
            d = {"prod": p, "ecm": ecm, "cxs": cxs_13[p], "ewk": ewk_13[p], "dzh": dZH}
            self.make_expr("expr::CVktkl_XSscal_{prod}_{ecm}('(@1 + (@0 - 1) * {cxs} / {ewk}) / ((1 - (@0 * @0 - 1) * {dzh}))', kl, Scaling_{prod}_{ecm})".format(**d))
            self.make_expr("expr::CVktkl_pos_XSscal_{prod}_{ecm}('0. + @0 * (@0 > 0)', CVktkl_XSscal_{prod}_{ecm})".format(**d))
            self.h_scalings.append("CVktkl_pos_XSscal_{prod}_{ecm}".format(**d))

        for p in ["ggZH", "tHq", "tHW"]:
            d = {"prod": p, "ecm": ecm}
            self.make_expr("expr::CVktkl_pos_XSscal_{prod}_{ecm}('0. + @0 * (@0 > 0)', Scaling_{prod}_{ecm})".format(**d))
            self.h_scalings.append("CVktkl_pos_XSscal_{prod}_{ecm}".format(**d))

        for p in ["ZH", "WH", "VH"]:
            d = {"prod": p, "ecm": ecm, "cxs": cxs_13[p], "ewk": ewk_13[p], "dzh": dZH}
            self.make_expr("expr::CVktkl_XSscal_{prod}_{ecm}('(@1 * @1 + (@0 - 1) * {cxs} / {ewk}) / ((1 - (@0 * @0 - 1) * {dzh}))', kl, CV)".format(**d))
            self.make_expr("expr::CVktkl_pos_XSscal_{prod}_{ecm}('0. + @0 * (@0 > 0)', CVktkl_XSscal_{prod}_{ecm})".format(**d))
            self.h_scalings.append("CVktkl_pos_XSscal_{prod}_{ecm}".format(**d))

        for p in ["ttH"]:
            d = {"prod": p, "ecm": ecm, "cxs": cxs_13[p], "ewk": ewk_13[p], "dzh": dZH}
            self.make_expr("expr::CVktkl_XSscal_{prod}_{ecm}('(@1 * @1 + (@0 - 1) * {cxs} / {ewk}) / ((1 - (@0 * @0 - 1) * {dzh}))', kl, kt)".format(**d))
            self.make_expr("expr::CVktkl_pos_XSscal_{prod}_{ecm}('0. + @0 * (@0 > 0)', CVktkl_XSscal_{prod}_{ecm})".format(**d))
            self.h_scalings.append("CVktkl_pos_XSscal_{prod}_{ecm}".format(**d))

    def find_br_scalings(self, process, bin=None):
        """
        Given the name of a *process*, extracts the trailing decay string (separated by uncerscore)
        as well as names the matching br scaling expressions and returns them in a 2-tuple. Example:

        .. code-block:: python

            find_br_scalings("ggHH_hbbhtt")
            # -> ("hbbhtt", ["CVktkl_BRscal_hbb", "CVktkl_BRscal_htt"])
        """
        # to automatically parse the higgs decay from the process name, we need a fixed format,
        # "*_hXX[hYY[...]]Z" where hXX, hYY, etc must be in the offical SM_HIGG_DECAYS, and Z can be
        # an additional sub decay identifier that does not contain underscores
        br = process.split("_")[-1]

        # extract valid decays
        decays = []
        m = re.match("^(({})+)(.*)$".format("|".join(SM_HIGG_DECAYS)), br)
        if m:
            decays = re.findall("|".join(SM_HIGG_DECAYS), m.group(1))

        # find scaling names
        br_scalings = []
        for d in decays:
            for s in self.br_scalings:
                if s.endswith("_" + d):
                    br_scalings.append(s)
                    break
            else:
                raise Exception("unsupported BR scaling of decay {} for process {} (bin {})".format(
                    d, process, bin))

        return br, br_scalings

    def find_h_scaling(self, process, bin=None, ecm="13TeV"):
        """
        Given the name of a *process*, extracts the leading single H name (separated by
        uncerscore) as well as names the matching H scaling expression and returns them in a
        2-tuple. Example:

        .. code-block:: python

            find_h_scaling("ggHH_hbbhtt")
            # -> ("ggHH", "CVktkl_pos_XSscal_ggHH_13TeV")
        """
        # single H process names must have the format "PROD_*" where PROD must be in
        # SM_HIGG_PROD; None is returned when this is not the case
        prod = process.split("_")[0]
        if prod not in SM_HIGG_PROD:
            return None, None

        for h_scaling in self.h_scalings:
            if h_scaling.endswith("_{}_{}".format(prod, ecm)):
                break
        else:
            raise Exception("unsupported H scaling of process {} (bin {})".format(process, bin))

        return prod, h_scaling

    def build_xsbr_scaling_hh(self, xs_scaling, process, bin=None):
        """
        Creates the full xs times br scaling expression for a HH *process* given the name of the
        current expression for scaling its cross section, and returns the new expression name.
        """
        br, br_scalings = self.find_br_scalings(process, bin=bin)

        # when no scalings were found, print a warning since this might be intentional and return,
        # when != 2 scalings were found, this is most likely an error
        if not br_scalings:
            print("WARNING: the HH process {} (bin {}) does not contain valid decay strings to "
                "extract branching ratios to apply the scaling with model parameters".format(
                    process, bin))
            return None
        elif len(br_scalings) != 2:
            raise Exception("the HH process {} (bin {}) contains {} valid decay string(s) while "
                "two were expected".format(process, bin, len(br_scalings)))

        # build the new scaling
        xsbr_scaling = "{}_BRscal_{}".format(xs_scaling, br)
        if not self.get_expr(xsbr_scaling):
            self.make_expr("expr::{}('@0 * @1 * @2', {}, {}, {})".format(xsbr_scaling, xs_scaling, br_scalings[0], br_scalings[1]))

        return xsbr_scaling

    def build_xsbr_scaling_h(self, xs_scaling, process, bin=None):
        """
        Creates the full xs times br scaling expression for a single H *process* given the name of
        the current expression for scaling its cross section, and returns the new expression name.
        """
        br, br_scalings = self.find_br_scalings(process, bin=bin)

        # when no scalings were found, print a warning since this might be intentional and return,
        # when != 1 scalings were found, this is most likely an error
        if not br_scalings:
            print("WARNING: the H process {} (bin {}) does not contain valid decay strings to "
                "extract branching ratios to apply the scaling with model parameters".format(
                    process, bin))
            return None
        elif len(br_scalings) != 1:
            raise Exception("the H process {} (bin {}) contains {} valid decay string(s) while one "
                "was expected".format(process, bin, len(br_scalings)))

        # build the new scaling
        xsbr_scaling = "{}_BRscal_{}".format(xs_scaling, br)
        if not self.get_expr(xsbr_scaling):
            self.make_expr("expr::{}('@0 * @1', {}, {})".format(xsbr_scaling, xs_scaling, br_scalings[0]))

        return xsbr_scaling


####################################################################################################
### Model classes
####################################################################################################

class HHModelBase(PhysicsModelBase):
    """
    Base class for HH physics models providing a common interface for subclasses such as the default
    HH model or a potential EFT model (e.g. kt-kl-C2).
    """

    # pois with initial (SM) value, start and stop
    # to be defined by subclasses
    R_POIS = OrderedDict()
    K_POIS = OrderedDict()

    def __init__(self, name):
        super(HHModelBase, self).__init__()

        # attributes
        self.name = name

        # names and values of physics options
        self.hh_options = {}

        # actual r and k pois, depending on used formulae and profiling options, set in reset_pois
        self.r_pois = None
        self.k_pois = None

        # mapping of (formula, sample) -> expression name that models the linear sample scales
        self.r_expressions = {}

        # nested mapping of formula -> sample -> matched processes for book keeping
        self.process_scales = defaultdict(lambda: defaultdict(set))

    def register_opt(self, name, default, is_flag=False):
        """
        Registers a physics option which is automatically parsed and set by
        :py:meth:`setPhysicsOptions`. Example:

        .. code-block:: python

            register_opt("myName", "some_default_value")
            # -> parses "--physics-option myName='other_value'"
        """
        self.hh_options[name] = {
            "value": default,
            "is_flag": is_flag,
        }

    def set_opt(self, name, value):
        """
        Sets the value of a physics option named *name*, previoulsy registered with
        :py:meth:`register_opt`, to *value*.
        """
        self.hh_options[name]["value"] = value

    def opt(self, name, default=no_value):
        """
        Helper to get the value of a physics option defined by *name* with an optional *default*
        value that is returned when no option with that *name* is registered.
        """
        if name in self.hh_options or default == no_value:
            return self.hh_options[name]["value"]
        else:
            return default

    def setPhysicsOptions(self, options):
        """
        Hook called by the super class to parse physics options received externally, e.g. via
        ``--physics-option`` or ``--PO``.
        """
        # split by "=" and check one by one
        pairs = [opt.split("=", 1) for opt in options if "=" in opt]
        for name, value in pairs:
            if name not in self.hh_options:
                print("[WARNING] unknown physics option '{}'".format(name))
                continue

            if self.hh_options[name]["is_flag"]:
                # boolean flag
                value = value.lower() in ["yes", "true", "1"]
            else:
                # string value, catch special cases
                value = None if value.lower() in ["", "none"] else value

            self.set_opt(name, value)
            print("[INFO] using model option {} = {}".format(name, value))

        # since settings might have changed, reset pois again
        self.reset_pois()

    def reset_pois(self):
        """
        Sets the instance-level :py:attr:`r_pois` and :py:attr:`k_pois` based on registered
        formulae.
        """
        # r pois
        self.r_pois = OrderedDict()
        for p, v in self.R_POIS.items():
            keep = p == "r"
            keep |= any(p == formula.r_poi for formula in self.get_formulae().values())
            if keep:
                self.r_pois[p] = v

        # k pois
        self.k_pois = OrderedDict()
        for p, v in self.K_POIS.items():
            keep = any(p in formula.couplings for formula in self.get_formulae().values())
            if keep:
                self.k_pois[p] = v

    def get_formulae(self):
        """
        Method that returns a dictionary of all used :py:class:`HHFormula` instances, mapped to
        their attribute names.
        To be implemented in subclasses.
        """
        raise NotImplementedError

    def create_hh_xsec_func(self, **kwargs):
        """
        Returns a function that can be used to compute cross sections, based on all formulae
        returned by :py:meth:`get_formulae`.
        """
        _kwargs = self.get_formulae()
        _kwargs.update(kwargs)
        return create_hh_xsec_func(**_kwargs)

    def make_expr(self, *args, **kwargs):
        """
        Shorthand for :py:meth:`modelBuilder.factory_`.
        """
        return self.modelBuilder.factory_(*args, **kwargs)

    def get_expr(self, *args, **kwargs):
        """
        Shorthand for :py:meth:`modelBuilder.out.function`.
        """
        return self.modelBuilder.out.function(*args, **kwargs)

    def make_var(self, *args, **kwargs):
        """
        Shorthand for :py:meth:`modelBuilder.doVar`.
        """
        return self.modelBuilder.doVar(*args, **kwargs)

    def get_var(self, *args, **kwargs):
        """
        Shorthand for :py:meth:`modelBuilder.out.var`.
        """
        return self.modelBuilder.out.var(*args, **kwargs)

    def make_set(self, *args, **kwargs):
        """
        Shorthand for :py:meth:`modelBuilder.doSet`.
        """
        return self.modelBuilder.doSet(*args, **kwargs)

    def done(self):
        """
        Hook called by the super class after the workspace is created.

        Here, we make sure that each formula will function as expected by checking that each of its
        samples is matched by exactly one process. When no processes was matched for _any_ sample of
        a formula, the scaling is disabled and the underlying r POI will have no effect.
        """
        super(HHModelBase, self).done()

        errors = []
        for formula in self.get_formulae().values():
            if not self.process_scales[formula]:
                continue

            # print matched process per expected sample
            print("\nmatching {} processes:".format(formula.channel))
            max_len = max(len(sample.label) for sample in formula.samples)
            unmatched_samples = []
            for sample in formula.samples:
                processes = self.process_scales[formula][sample]
                if not processes:
                    unmatched_samples.append(sample)
                print("  {}{} -> {}".format(
                    sample.label,
                    " " * (max_len - len(sample.label)),
                    ", ".join(processes) if processes else "MISSING",
                ))

            # complain about samples that were not matched by any process
            if len(unmatched_samples) not in [0, formula.n_samples]:
                errors.append("{} {} samples were not matched by any process: {}".format(
                    len(unmatched_samples),
                    formula.channel,
                    ", ".join(sample.label for sample in unmatched_samples),
                ))

        if errors:
            raise Exception("\n".join(errors))


class HHModel(HHModelBase):
    """
    Models the HH production as a linear sum of the input components for ggf, vbf and vhh processes.
    The following physics options are supported:

    - doNNLOscaling (bool)   : Convert ggf HH yields (that are given in NLO by convention) to NNLO.
    - doBRscaling (bool)     : Enable scaling Higgs branching ratios with model parameters.
    - doHscaling (bool)      : Enable scaling single Higgs cross sections with model parameters.
    - doklDependentUnc (bool): Add a theory uncertainty on ggf HH production that depends on model
                               parameters.
    - doProfileX (string)    : Either "flat" to enable the profiling of kappa parameter X with a
      X in {kl,kt,CV,C2V}      flat prior, or "gauss,FLOAT" (or "gauss,-FLOAT/+FLOAT") to use a
                               gaussian (asymmetric) prior. In any case, X will be profiled and is
                               hence removed from the list of POIs.

    A string encoded boolean flag is interpreted as *True* when it is either ``"yes"``, ``"true"``
    or ``1`` (case-insensitive).
    """

    # pois with initial (SM) value, start and stop
    R_POIS = OrderedDict([
        ("r", (1, -20, 20)),
        ("r_gghh", (1, -20, 20)),
        ("r_qqhh", (1, -20, 20)),
        ("r_vhh", (1, -20, 20)),
    ])
    K_POIS = OrderedDict([
        ("kl", (1, -30, 30)),
        ("kt", (1, -10, 10)),
        ("CV", (1, -10, 10)),
        ("C2V", (1, -10, 10)),
    ])

    def __init__(self, name, ggf_samples=None, vbf_samples=None, vhh_samples=None):
        super(HHModel, self).__init__(name)

        # attributes
        self.ggf_formula = GGFFormula(ggf_samples) if ggf_samples else None
        self.vbf_formula = VBFFormula(vbf_samples) if vbf_samples else None
        self.vhh_formula = VHHFormula(vhh_samples) if vhh_samples else None
        self.ggf_kl_dep_unc = "THU_HH"  # name for kl-dependent QCDscale + mtop uncertainty on ggf
        self.h_br_scaler = None  # initialized in create_scalings

        # register options
        self.register_opt("doNNLOscaling", True, is_flag=True)
        self.register_opt("doklDependentUnc", True, is_flag=True)
        self.register_opt("doBRscaling", True, is_flag=True)
        self.register_opt("doHscaling", True, is_flag=True)
        for p in self.K_POIS:
            self.register_opt("doProfile{}".format(p), None)

        # reset instance-level pois
        self.reset_pois()

    def reset_pois(self):
        super(HHModel, self).reset_pois()

        # remove profiled k pois
        for p in list(self.k_pois):
            if self.opt("doProfile" + p):
                del self.k_pois[p]

    def get_formulae(self):
        formulae = {}
        if self.ggf_formula:
            formulae["ggf_formula"] = self.ggf_formula
        if self.vbf_formula:
            formulae["vbf_formula"] = self.vbf_formula
        if self.vhh_formula:
            formulae["vhh_formula"] = self.vhh_formula
        return formulae

    def doParametersOfInterest(self):
        """
        Hook called by the super class to add parameters (of interest) to the model.

        Here, we add all r and k POIs depending on profiling options, define the POI group and
        re-initialize the MH parameter. By default, the main r POI will be the only floating one,
        whereas the others are either fixed or fully profiled.
        """
        # add rate POIs and freeze r_* by default
        pois = []
        for p, (value, start, stop) in self.r_pois.items():
            self.make_var("{}[{},{},{}]".format(p, value, start, stop))
            # freeze anything but r
            if p != "r":
                self.get_var(p).setConstant(True)
            pois.append(p)

        # first, add all known coupling modifiers
        for p in self.K_POIS:
            value, start, stop = self.k_pois.get(p, self.K_POIS[p])
            self.make_var("{}[{},{},{}]".format(p, value, start, stop))

        # then, configure coupling modifiers used as potential POIs
        for p, (value, start, stop) in self.k_pois.items():
            self.get_var(p).setConstant(True)
            pois.append(p)

        # define the POI group
        self.make_set("POI", ",".join(pois))
        print("using POIs {}".format(",".join(pois)))

        # set or redefine the MH variable on which some of the BRs depend
        if not self.options.mass:
            raise Exception("invalid mass value '{}', please provide a valid value using the "
                "--mass option".format(self.options.mass))
        if self.get_var("MH"):
            self.get_var("MH").removeRange()
            self.get_var("MH").setVal(self.options.mass)
        else:
            self.make_var("MH[{}]".format(self.options.mass))
        self.get_var("MH").setConstant(True)

        # add objects for kl dependent theory uncertainties
        if self.opt("doklDependentUnc"):
            self.create_ggf_kl_dep_unc()

        # create cross section scaling functions
        self.create_scalings()

    def create_ggf_kl_dep_unc(self):
        """
        Creates the expressions used to scale ggf process rates depending on kl, including the
        QCDscale + mtop uncertainty.
        """
        # add the parameter
        self.make_var("{}[-7,7]".format(self.ggf_kl_dep_unc))

        # add uncertainty bands
        expr_nom = create_ggf_xsec_str("nnlo", "@0")
        expr_hi1 = create_ggf_xsec_str("unc_u1", "@0")
        expr_hi2 = create_ggf_xsec_str("unc_u2", "@0")
        expr_lo1 = create_ggf_xsec_str("unc_d1", "@0")
        expr_lo2 = create_ggf_xsec_str("unc_d2", "@0")
        self.make_expr("expr::{}_kappaHi('max({}, {}) / ({})', kl)".format(
            self.ggf_kl_dep_unc, expr_hi1, expr_hi2, expr_nom))
        self.make_expr("expr::{}_kappaLo('min({}, {}) / ({})', kl)".format(
            self.ggf_kl_dep_unc, expr_lo1, expr_lo2, expr_nom))

        # create the interpolation
        # as in https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit/blob/102x/interface/ProcessNormalization.h
        d = {"x": self.ggf_kl_dep_unc}
        d["name"] = "{x}_kappa".format(**d)
        d["hi"] = "{x}_kappaHi".format(**d)
        d["lo"] = "{x}_kappaLo".format(**d)
        d["logKhi"] = "log({hi})".format(**d)
        d["logKlo"] = "-log({lo})".format(**d)
        d["avg"] = "0.5 * ({logKhi} + {logKlo})".format(**d)
        d["halfdiff"] = "0.5 * ({logKhi} - {logKlo})".format(**d)
        d["twox"] = "2 * {x}".format(**d)
        d["twox2"] = "({twox}) * ({twox})".format(**d)
        d["alpha"] = "0.125 * {twox} * ({twox2} * ({twox2} - 10.) + 15.)".format(**d)
        d["retCent"] = "{avg} + {alpha} * {halfdiff}".format(**d)
        d["retLow"] = d["logKlo"]
        d["retHigh"] = d["logKhi"]
        d["retFull"] = "{x} <= -0.5 ? ({retLow}) : {x} >= 0.5 ? ({retHigh}) : ({retCent})".format(**d)
        d["ret"] = "expr::{name}('exp({retFull})', {{{hi}, {lo}, {x}}})".format(**d)
        self.make_expr(d["ret"])

        # add the scaling
        self.make_expr("expr::scaling_{0}('pow(@0, @1)', {0}_kappa, {0})".format(self.ggf_kl_dep_unc))

    def create_scalings(self):
        """
        Creates the scaling expressions for morphing the registered signal samples into a single
        shape.
        """
        # initialize the HBRScaler
        self.h_br_scaler = HBRScaler(
            self.modelBuilder,
            scale_br=self.opt("doBRscaling"),
            scale_h=self.opt("doHscaling"),
        )

        def pow_to_mul_string(expr):
            """
            Convert powers in a sympy expression to Muls (e.g. "a**2 => a*a") and return a string.
            """
            pows = list(expr.atoms(sympy.Pow))
            if any(not e.is_Integer for b, e in (i.as_base_exp() for i in pows)):
                raise ValueError("a power in '{}' contains a non-integer exponent".format(expr))
            s = str(expr)
            for p in pows:
                base, exp = p.as_base_exp()
                repl = sympy.Mul(*(exp * [base]), evaluate=False)
                s = s.replace(str(p), str(repl))
            return s

        def replace_coupling(c, repl, s):
            """
            Replaces the coupling *c* by *repl* in a string *s*, ensuring that couplings consisting
            of a substring of *c* remain unchanged. Example:

            .. code-block:: python

                replace_coupling("kl", "@0", "3 * kl * kt")
                # -> "3 * @0 * kt"

                replace_coupling("c2", "@0", "3 * c2 * c2g")
                # -> "3 * @0 * c2g
                # note the unchanged "c2g"
            """
            return re.sub(r"{}([^0-9a-zA-Z])".format(c), r"{}\1".format(repl), s + " ")[:-1]

        # add ggf sample scalings
        if self.ggf_formula:
            for sample, coeff in zip(self.ggf_formula.samples, self.ggf_formula.coeffs):
                # create the expression that scales this particular sample based on the formula
                name = "f_scale_ggf_sample_{}".format(sample.label)
                expr = pow_to_mul_string(coeff)
                for i, coupling in enumerate(self.ggf_formula.couplings):
                    expr = replace_coupling(coupling, "@{}".format(i), expr)
                self.make_expr("expr::{}('{}', {})".format(
                    name, expr, ", ".join(self.ggf_formula.couplings)))

                # optionally multiply the theory uncertainty scaling to the expression
                if self.opt("doklDependentUnc"):
                    new_name = "{}__kl_dep_unc".format(name)
                    self.make_expr("prod::{}(scaling_{}, {})".format(
                        new_name, self.ggf_kl_dep_unc, name))
                    name = new_name

                # optionally rescale to nnlo (expecting the normalization to be nlo * k initially)
                if self.opt("doNNLOscaling"):
                    new_name = "{}__nlo2nnlo".format(name)
                    nlo_expr = create_ggf_xsec_str("nlo", "@0")
                    nnlo_expr = create_ggf_xsec_str("nnlo", "@0")
                    self.make_expr("expr::{}('@1 * ({}) / ({} * ({}))', kl, {})".format(
                        new_name, nnlo_expr, ggf_k_factor, nlo_expr, name))
                    name = new_name

                # scale it by the channel specific r POI
                new_name = "{}__r_gghh".format(name)
                self.make_expr("prod::{}(r_gghh, {})".format(new_name, name))
                name = new_name

                # scale it by the common r POI
                new_name = "{}__r".format(name)
                self.make_expr("prod::{}(r, {})".format(new_name, name))
                name = new_name

                # store the final expression name
                self.r_expressions[(self.ggf_formula, sample)] = name

        # add vbf sample scalings
        if self.vbf_formula:
            for sample, coeff in zip(self.vbf_formula.samples, self.vbf_formula.coeffs):
                # create the expression that scales this particular sample based on the formula
                name = "f_scale_vbf_sample_{}".format(sample.label)
                expr = pow_to_mul_string(coeff)
                for i, coupling in enumerate(self.vbf_formula.couplings):
                    expr = replace_coupling(coupling, "@{}".format(i), expr)
                self.make_expr("expr::{}('{}', {})".format(
                    name, expr, ", ".join(self.vbf_formula.couplings)))

                # scale it by the channel specific r POI
                new_name = "{}__r_qqhh".format(name)
                self.make_expr("prod::{}(r_qqhh, {})".format(
                    new_name, name))
                name = new_name

                # scale it by the common r POI
                new_name = "{}__r".format(name)
                self.make_expr("prod::{}(r, {})".format(
                    new_name, name))
                name = new_name

                # store the final expression name
                self.r_expressions[(self.vbf_formula, sample)] = name

        # add vhh sample scalings
        if self.vhh_formula:
            for sample, coeff in zip(self.vhh_formula.samples, self.vhh_formula.coeffs):
                # create the expression that scales this particular sample based on the formula
                name = "f_scale_vhh_sample_{}".format(sample.label)
                expr = pow_to_mul_string(coeff)
                for i, coupling in enumerate(self.vhh_formula.couplings):
                    expr = replace_coupling(coupling, "@{}".format(i), expr)
                self.make_expr("expr::{}('{}', {})".format(
                    name, expr, ", ".join(self.vhh_formula.couplings)))

                # scale it by the channel specific r POI
                new_name = "{}__r_vhh".format(name)
                self.make_expr("prod::{}(r_vhh, {})".format(
                    new_name, name))
                name = new_name

                # scale it by the common r POI
                new_name = "{}__r".format(name)
                self.make_expr("prod::{}(r, {})".format(
                    new_name, name))
                name = new_name

                # store the final expression name
                self.r_expressions[(self.vhh_formula, sample)] = name

    def preProcessNuisances(self, nuisances):
        """
        Hook called by the super class before nuisances are processed.

        Here, we make sure that all custom parameters are properly added to the model:

        - kl-dependent ggf theory uncertainty
        - parameters of coupling modifiers when profiling
        """
        # enable profiling of k POIs with a configurable prior when requested
        for p in self.K_POIS:
            value = self.opt("doProfile" + p, False)
            if not value:
                continue

            # get the prior and add it
            prior, width = value.split(",", 1) if "," in value else (value, None)
            if prior == "flat":
                self.modelBuilder.DC.flatParamNuisances[p] = True
                print("adding flat prior for parameter {}".format(p))
            elif prior == "gauss":
                nuisances.append((p, False, "param", ["1", width, "[-7,7]"], []))
                print("adding gaussian prior for parameter {} with width {}".format(p, width))
            else:
                raise Exception("unknown prior '{}' for parameter {}".format(prior, p))

        # add the theory uncertainty on ggf whwn configured
        if self.opt("doklDependentUnc"):
            nuisances.append((self.ggf_kl_dep_unc, False, "param", ["0", "1"], []))

    def getYieldScale(self, bin, process):
        """
        Hook called by the super class to determine the scaling, or an expressions modeling the
        scaling of a *process* in a specific datacard *bin*.

        Here, we distinguish several cases, depending on which type of process is considered:

        - HH signal:
            - Use the scaling expression build in :py:meth:`create_scalings`
            - Optionally add kappa dependence on branching ratios

        - Single H backgrounds:
            - Optionally add kappa dependence on production cross sections
            - Optionally add kappa dependence on branching ratios

        - Other backgrounds:
            - Return 1 to express that we are using the rate as saved in datacards
        """
        # find signal matches
        for formula in self.get_formulae().values():
            # get matching samples
            matching_samples = []
            for sample in formula.samples:
                if sample.matches_process(process):
                    matching_samples.append(sample)

            # complain when there is more than one hit
            if len(matching_samples) > 1:
                raise Exception("found {} matches for {} signal process {} in bin {}".format(
                    len(matching_samples), formula.channel, process, bin))

            # get the scale when there is a hit
            if len(matching_samples) == 1:
                sample = matching_samples[0]
                # store the process
                self.process_scales[formula][sample].add(process)
                # get the scale expression
                scaling = self.r_expressions[(formula, sample)]
                # when the BR scaling is enabled, try to extract the decays from the process name
                if self.opt("doBRscaling"):
                    scaling = self.h_br_scaler.build_xsbr_scaling_hh(scaling, process, bin) or scaling
                return scaling

        # complain when the process is a signal but no sample matched
        if self.DC.isSignal[process]:
            raise Exception("signal process {} did not match any HH sample in bin {}".format(
                process, bin))

        # single H match?
        if self.opt("doHscaling"):
            scaling = self.h_br_scaler.find_h_scaling(process, bin)[1]
            # when the BR scaling is enabled, try to extract the decay from the process name
            if scaling and self.opt("doBRscaling"):
                scaling = self.h_br_scaler.build_xsbr_scaling_h(scaling, process, bin) or scaling
            return scaling or 1.

        # at this point we are dealing with a background process that is also not single-H-scaled,
        # so it is safe to return 1 since any misconfiguration should have been raised already
        return 1.


def create_model(name, skip_ggf=None, skip_vbf=None, skip_vhh=True, **kwargs):
    """
    Returns a new :py:class:`HHModel` instance named *name*. Its ggf, vbf and vhh sample lists are
    using all availabe samples except those defined in *skip_ggf*, *skip_vbf* and *skip_vhh*. The
    order of passed keys to be skipped does not matter. All additional *kwargs* are forwarded to the
    model constructor.
    """
    # get all unskipped ggf keys
    ggf_keys = [] if skip_ggf is True else [
        key for key in ggf_samples
        if skip_ggf is None or key not in skip_ggf
    ]

    # get all unskipped vbf keys
    vbf_keys = [] if skip_vbf is True else [
        key for key in vbf_samples
        if skip_vbf is None or key not in skip_vbf
    ]

    # get all unskipped vhh keys
    vhh_keys = [] if skip_vhh is True else [
        key for key in vhh_samples
        if skip_vhh is None or key not in skip_vhh
    ]

    # create the return the model
    return HHModel(
        name=name,
        ggf_samples=[ggf_samples[key] for key in ggf_keys],
        vbf_samples=[vbf_samples[key] for key in vbf_keys],
        vhh_samples=[vhh_samples[key] for key in vhh_keys],
        **kwargs
    )


# some named, default models
model_all = create_model("model_all")
model_all_vhh = create_model("model_all_vhh", skip_vhh=[])

# model used for the combination
model_default = create_model("model_default", skip_ggf=[(0, 1)], skip_vbf=[(0.5, 1, 1)])
model_default_vhh = create_model("model_default_vhh", skip_ggf=[(0, 1)], skip_vbf=[(0.5, 1, 1)],
    skip_vhh=[])

# ggf test models
model_no_ggf_kl0    = create_model("model_no_ggf_kl0",    skip_ggf=[(0, 1)],    skip_vbf=[(1, 0, 1)])
model_no_ggf_kl1    = create_model("model_no_ggf_kl1",    skip_ggf=[(1, 1)],    skip_vbf=[(1, 0, 1)])
model_no_ggf_kl2p45 = create_model("model_no_ggf_kl2p45", skip_ggf=[(2.45, 1)], skip_vbf=[(1, 0, 1)])
model_no_ggf_kl5    = create_model("model_no_ggf_kl5",    skip_ggf=[(5, 1)],    skip_vbf=[(1, 0, 1)])

# vbf test models
model_no_vbf_sm    = create_model("model_no_vbf_sm",    skip_ggf=[(0, 1)], skip_vbf=[(1, 1, 1)])
model_no_vbf_kl0   = create_model("model_no_vbf_kl0",   skip_ggf=[(0, 1)], skip_vbf=[(1, 1, 0)])
model_no_vbf_kl2   = create_model("model_no_vbf_kl2",   skip_ggf=[(0, 1)], skip_vbf=[(1, 1, 2)])
model_no_vbf_C2V0  = create_model("model_no_vbf_C2V0",  skip_ggf=[(0, 1)], skip_vbf=[(1, 0, 1)])
model_no_vbf_C2V2  = create_model("model_no_vbf_C2V2",  skip_ggf=[(0, 1)], skip_vbf=[(1, 2, 1)])
model_no_vbf_CV0p5 = create_model("model_no_vbf_CV0p5", skip_ggf=[(0, 1)], skip_vbf=[(0.5, 1, 1)])
model_no_vbf_CV1p5 = create_model("model_no_vbf_CV1p5", skip_ggf=[(0, 1)], skip_vbf=[(1.5, 1, 1)])


####################################################################################################
### Cross section helpers
####################################################################################################

# ggf NLO -> NNLO k-factor
ggf_k_factor = 1.115

# coefficients for modeling the kl dependence of the ggf cross section (in fb) and its uncertainty
# (a0, a1, a2) -> a0 + a1 * kl + a2 * kl**2
# values from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/LHCHXSWGHH?rev=70
ggf_kl_coeffs = {
    "nlo": (62.5339, -44.3231, 9.6340),
    "nnlo": (70.3874, -50.4111, 11.0595),
    "unc_u1": (76.6075, -56.4818, 12.6350),
    "unc_u2": (75.4617, -56.3164, 12.7135),
    "unc_d1": (57.6809, -42.9905, 9.58474),
    "unc_d2": (58.3769, -43.9657, 9.87094),
    # old uncertainty coefficients for backward checks from
    # https://twiki.cern.ch/twiki/bin/view/LHCPhysics/LHCHXSWGHH?rev=65
    # "unc_u1": (72.0744, -51.7362, 11.3712),
    # "unc_u2": (70.9286, -51.5708, 11.4497),
    # "unc_d1": (66.0621, -46.7458, 10.1673),
    # "unc_d2": (66.7581, -47.7210, 10.4535),
}


def create_ggf_xsec_str(coeffs, s):
    """
    Returns a string expression of one set of coefficients stored in :py:attr:`ggf_kl_coeffs` as
    *coeffs* with kl being replaced by *s*. Example:

    .. code-block:: python

        create_ggf_xsec_str("nlo", "kl")
        # -> "62.5339 - 44.3231 * kl + 9.634 * kl * kl"
    """
    a0, a1, a2 = ggf_kl_coeffs[coeffs]
    sign1, abs1 = ["+", "-"][a1 < 0], abs(a1)
    sign2, abs2 = ["+", "-"][a2 < 0], abs(a2)
    return "{} {} {} * {} {} {} * {} * {}".format(a0, sign1, abs1, s, sign2, abs2, s, s)


def create_ggf_xsec_func(formula=None):
    """
    Creates and returns a function that can be used to calculate numeric ggf cross section values in
    pb given an appropriate *formula*, which defaults to *model_default.ggf_formula*. The returned
    function has the signature ``(kl=1.0, kt=1.0, nnlo=True, unc=None)``.

    When *nnlo* is *False*, the constant k-factor is still applied. Otherwise, the returned value is
    in full next-to-next-to-leading order. In this case, *unc* can be set to eiher "up" or "down" to
    return the up / down varied cross section instead where the uncertainty is composed of a *kl*
    dependent QCDscale + mtop uncertainty and a flat PDF uncertainty of 3%.

    Example:

    .. code-block:: python

        get_ggf_xsec = create_ggf_xsec_func()

        print(get_ggf_xsec(kl=2.))
        # -> 0.013803...

        print(get_ggf_xsec(kl=2., nnlo=False))
        # -> 0.013852...

        print(get_ggf_xsec(kl=2., unc="up"))
        # -> 0.014305...

    Formulae are taken from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/LHCHWGHH?rev=70.
    """
    if formula is None:
        formula = model_default.ggf_formula

    # create the lambdify'ed evaluation function
    symbol_names = ["kl", "kt"] + list(map("xs{}".format, range(formula.n_samples)))
    xsec_func = sympy.lambdify(sympy.symbols(symbol_names), formula.sigma)

    # nlo-to-nnlo scaling functions in case nnlo is set
    # (not really clean to use eval here, but this way create_ggf_xsec_str remains the only
    # place where the formula needs to be mainted)
    expr_nlo = create_ggf_xsec_str("nlo", "kl")
    expr_nnlo = create_ggf_xsec_str("nnlo", "kl")
    xsec_nlo = eval("lambda kl: 0.001 * {} * ({})".format(ggf_k_factor, expr_nlo))
    xsec_nnlo = eval("lambda kl: 0.001 * ({})".format(expr_nnlo))
    nlo2nnlo = lambda xsec, kl: xsec * xsec_nnlo(kl) / xsec_nlo(kl)  # noqa

    # scale+mtop uncertainty in case unc is set
    expr_u1 = create_ggf_xsec_str("unc_u1", "kl")
    expr_u2 = create_ggf_xsec_str("unc_u2", "kl")
    expr_d1 = create_ggf_xsec_str("unc_d1", "kl")
    expr_d2 = create_ggf_xsec_str("unc_d2", "kl")
    xsec_nnlo_scale_up = eval("lambda kl: 0.001 * max({}, {})".format(expr_u1, expr_u2))
    xsec_nnlo_scale_down = eval("lambda kl: 0.001 * min({}, {})".format(expr_d1, expr_d2))

    def apply_uncertainty_nnlo(kl, xsec_nom, unc):
        # note on kt: in the twiki linked above, uncertainties on the ggf production cross section
        # are quoted for different kl values but otherwise fully SM parameters, esp. kt=1;
        # however, the nominal cross section *xsec_nom* might be subject to a different kt value
        # and thus, the following implementation assumes that the relative uncertainties according
        # to the SM recommendation are preserved; for instance, if the the scale+mtop uncertainty
        # for kl=2,kt=1 would be 10%, then the code below will assume an uncertainty for kl=2,kt!=1
        # of 10% as well

        # compute the relative, signed scale+mtop uncertainty
        if unc.lower() not in ("up", "down"):
            raise ValueError("unc must be 'up' or 'down', got '{}'".format(unc))
        scale_func = {"up": xsec_nnlo_scale_up, "down": xsec_nnlo_scale_down}[unc.lower()]  # noqa
        xsec_nom_kt1 = xsec_func(kl, 1., *(sample.xs for sample in formula.samples))[0, 0]
        xsec_unc = (scale_func(kl) - xsec_nom_kt1) / xsec_nom_kt1

        # combine with flat 3% PDF uncertainty, preserving the sign
        unc_sign = 1 if xsec_unc > 0 else -1
        xsec_unc = unc_sign * (xsec_unc**2 + 0.03**2)**0.5

        # compute the shifted absolute value
        xsec = xsec_nom * (1. + xsec_unc)

        return xsec

    # wrap into another function to apply defaults and nlo-to-nnlo scaling
    def wrapper(kl=1.0, kt=1.0, nnlo=True, unc=None):
        xsec = xsec_func(kl, kt, *(sample.xs for sample in formula.samples))[0, 0]

        # nnlo scaling?
        if nnlo:
            xsec = nlo2nnlo(xsec, kl)

        # apply uncertainties?
        if unc:
            if not nnlo:
                raise NotImplementedError("NLO ggf cross section uncertainties are not implemented")
            xsec = apply_uncertainty_nnlo(kl, xsec, unc)

        return xsec

    return wrapper


def create_vbf_xsec_func(formula=None):
    """
    Creates and returns a function that can be used to calculate numeric vbf cross section values in
    pb given an appropriate *formula*, which defaults to *model_default.vbf_formula*. The returned
    function has the signature ``(C2V=1.0, CV=1.0, kl=1.0, unc=None)``.

    *unc* can be set to eiher "up" or "down" to return the up / down varied cross section instead
    where the uncertainty is composed of scale variations and pdf+alpha_s.

    Example:

    .. code-block:: python

        get_vbf_xsec = create_vbf_xsec_func()

        print(get_vbf_xsec(C2V=2.))
        # -> 0.014218... (or similar)

    Uncertainties taken from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/LHCHXSWGHH?rev=70.
    """
    if formula is None:
        formula = model_default.vbf_formula

    # create the lambdify'ed evaluation function
    symbol_names = ["C2V", "CV", "kl"] + list(map("xs{}".format, range(formula.n_samples)))
    xsec_func = sympy.lambdify(sympy.symbols(symbol_names), formula.sigma)

    # wrap into another function to apply defaults
    def wrapper(C2V=1., CV=1., kl=1., unc=None):
        xsec = xsec_func(C2V, CV, kl, *(sample.xs for sample in formula.samples))[0, 0]

        # apply uncertainties?
        if unc:
            if unc.lower() not in ["up", "down"]:
                raise ValueError("unc must be 'up' or 'down', got '{}'".format(unc))
            scale_rel = {"up": 0.0003, "down": 0.0004}[unc.lower()]
            pdf_rel = 0.021
            unc_rel = (1 if unc.lower() == "up" else -1) * (scale_rel**2 + pdf_rel**2)**0.5
            xsec *= 1 + unc_rel

        return xsec

    return wrapper


def create_vhh_xsec_func(formula=None):
    """
    Creates and returns a function that can be used to calculate numeric vhh cross section values in
    pb given an appropriate *formula*, which defaults to *model_default_vhh.vhh_formula*. The
    returned function has the signature ``(C2V=1.0, CV=1.0, kl=1.0)``.

    Example:

    .. code-block:: python

        get_vhh_xsec = create_vhh_xsec_func()

        print(get_vhh_xsec(C2V=2.))
        # -> 0.005316... (or similar)
    """
    if formula is None:
        formula = model_default_vhh.vhh_formula

    # create the lambdify'ed evaluation function
    symbol_names = ["C2V", "CV", "kl"] + list(map("xs{}".format, range(formula.n_samples)))
    xsec_func = sympy.lambdify(sympy.symbols(symbol_names), formula.sigma)

    # wrap into another function to apply defaults
    def wrapper(C2V=1.0, CV=1.0, kl=1.0):
        xsec = xsec_func(C2V, CV, kl, *(sample.xs for sample in formula.samples))[0, 0]
        return xsec

    return wrapper


def create_hh_xsec_func(ggf_formula=None, vbf_formula=None, vhh_formula=None):
    """
    Creates and returns a function that can be used to calculate numeric HH cross section values in
    pb given appropriate *ggf_formula*, *vbf_formula* and *vhh_formula* objects, which default to
    *model_default.ggf_formula*, *model_default.vbf_formula* and *model_default.vhh_formula*,
    respectively. When a forumla is set explicitely to *False*, the corresponding process is not
    considered in the inclusive calculation. The returned function has the signature
    ``(kl=1.0, kt=1.0, C2V=1.0, CV=1.0, nnlo=True, unc=None)``.

    The *nnlo* setting only affects the ggf component of the cross section. When *nnlo* is *False*,
    the constant k-factor of the ggf calculation is still applied. Otherwise, the returned value is
    in full next-to-next-to-leading order for ggf. *unc* can be set to eiher "up" or "down" to
    return the up / down varied cross section instead where the uncertainty is composed of a *kl*
    dependent scale+mtop uncertainty and an independent PDF uncertainty of 3% for ggf, and a scale
    and pdf+alpha_s uncertainty for vbf. The uncertainties of the ggf and vbf processes are treated
    as uncorrelated.

    Example:

    .. code-block:: python

        get_hh_xsec = create_hh_xsec_func()

        print(get_hh_xsec(kl=2.))
        # -> 0.015226...

        print(get_hh_xsec(kl=2., nnlo=False))
        # -> 0.015275...

        print(get_hh_xsec(kl=2., unc="up"))
        # -> 0.015702...
    """
    # default function for a disabled process
    no_xsec = lambda *args, **kwargs: 0.

    # get the particular wrappers of the components
    get_ggf_xsec = no_xsec if ggf_formula is False else create_ggf_xsec_func(ggf_formula)
    get_vbf_xsec = no_xsec if vbf_formula is False else create_vbf_xsec_func(vbf_formula)
    get_vhh_xsec = no_xsec if vhh_formula is False else create_vhh_xsec_func(vhh_formula)

    # create a combined wrapper with the merged signature
    def wrapper(kl=1.0, kt=1.0, C2V=1.0, CV=1.0, nnlo=True, unc=None):
        ggf_xsec = get_ggf_xsec(kl=kl, kt=kt, nnlo=nnlo)
        vbf_xsec = get_vbf_xsec(C2V=C2V, CV=CV, kl=kl)
        vhh_xsec = get_vhh_xsec(C2V=C2V, CV=CV, kl=kl)
        xsec = ggf_xsec + vbf_xsec + vhh_xsec

        # apply uncertainties?
        if unc:
            if unc.lower() not in ["up", "down"]:
                raise ValueError("unc must be 'up' or 'down', got '{}'".format(unc))
            # ggf uncertainty
            ggf_unc = get_ggf_xsec(kl=kl, kt=kt, nnlo=nnlo, unc=unc) - ggf_xsec
            # vbf uncertainty
            vbf_unc = get_vbf_xsec(C2V=C2V, CV=CV, kl=kl, unc=unc) - vbf_xsec
            # vhh uncertainty
            vhh_unc = 0.
            # combine
            sign = 1 if unc.lower() == "up" else -1
            unc = sign * (ggf_unc**2 + vbf_unc**2 + vhh_unc**2)**0.5
            xsec += unc

        return xsec

    return wrapper


# default ggf cross section getter using the formula of the *model_default* model
get_ggf_xsec = create_ggf_xsec_func()

# default vbf cross section getter using the formula of the *model_default* model
get_vbf_xsec = create_vbf_xsec_func()

# default vhh cross section getter using the formula of the *model_default* model
get_vhh_xsec = create_vhh_xsec_func()

# default combined cross section getter using the formulas of the *model_default* model
get_hh_xsec = create_hh_xsec_func()
