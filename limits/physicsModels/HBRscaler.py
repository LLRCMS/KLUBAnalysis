# coding: utf-8

"""
Reorganization of https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit/blob/102x/python/TrilinearCouplingModels.py
for HH combination.
"""

import os
import re

from HiggsAnalysis.CombinedLimit.SMHiggsBuilder import SMHiggsBuilder
from HiggsAnalysis.CombinedLimit.PhysicsModel import SM_HIGG_DECAYS, SM_HIGG_PROD


# single Higgs production scalings coefficients
# WH and ZH coeff are very similar --> build VH coeff as an average btw the two
energy = "13TeV"
cXSmap_13 = {
    "ggH": 0.66e-2,
    "qqH": 0.64e-2,
    "WH": 1.03e-2,
    "ZH": 1.19e-2,
    "ttH": 3.51e-2,
    "VH": (0.5 * (1.03e-2 + 1.19e-2)),
}
EWKmap_13 = {
    "ggH": 1.049,
    "qqH": 0.932,
    "WH": 0.93,
    "ZH": 0.947,
    "ttH": 1.014,
    "VH": (0.5 * (0.93 + 0.947)),
}
dZH = -1.536e-3

# BR scaling vs kl (https://arxiv.org/abs/1709.08649 Eq 22)
cGammap = {
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

# ensure hss in the SM_HIGG_DECAYS
if "hss" not in SM_HIGG_DECAYS:
    # do not change it inplace not to interfer with upstream code
    SM_HIGG_DECAYS = SM_HIGG_DECAYS + ["hss"]

# single H production modes that are supported in the scaling
SM_SCALED_SINGLE_HIGG_PROD = ["ggZH", "tHq", "tHW", "ggH", "qqH", "ZH", "WH", "VH", "ttH"]


class HBRscaler(object):
    """
    Produce single H and BR scalings for anomalous couplings, and produce XS*BR scalings for both H
    and HH.
    """

    def __init__(self, modelBuilder, doBRscaling, doHscaling):
        super(HBRscaler, self).__init__()

        self.modelBuilder = modelBuilder
        # use SMHiggsBuilder to build single H XS and BR scalings
        datadir = None
        if "CMSSW_BASE" in os.environ:
            datadir = os.path.expandvars("$CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/data/lhc-hxswg")
        elif "DHI_SOFTWARE" in os.environ:
            datadir = os.path.expandvars("$DHI_SOFTWARE/HiggsAnalysis/CombinedLimit/data/lhc-hxswg")
        self.SMH = SMHiggsBuilder(self.modelBuilder, datadir=datadir)

        self.doBRscaling = doBRscaling
        self.doHscaling = doHscaling
        self.f_BR_scalings = []
        self.f_H_scalings = []

        if self.doBRscaling:
            self.buildBRScalings()
        if self.doHscaling:
            self.buildHScalings()

    def buildBRScalings(self):
        for d in SM_HIGG_DECAYS:
            self.SMH.makeBR(d)

        # FIXME: to check how to deal with BR uncertainties --> for now keep them frozen
        for d in SM_HIGG_DECAYS:
            self.modelBuilder.factory_("HiggsDecayWidth_UncertaintyScaling_%s[1.0]" % d)

        # fix to have all BRs add up to unity
        self.modelBuilder.factory_("sum::c7_SMBRs(%s)" % (",".join("SM_BR_" + d for d in SM_HIGG_DECAYS)))
        # self.modelBuilder.out.function("c7_SMBRs").Print("")

        # define resolved loops
        self.SMH.makeScaling("hgluglu", Cb="1", Ctop="kt")
        self.SMH.makeScaling("hgg", Cb="1", Ctop="kt", CW="CV", Ctau="1")
        self.SMH.makeScaling("hzg", Cb="1", Ctop="kt", CW="CV", Ctau="1")

        # BR scaling vs kl (https://arxiv.org/abs/1709.08649 Eq 22)
        for dec, valC1 in cGammap.items():
            self.modelBuilder.factory_("expr::kl_scalBR_%s('(@0-1) * %g', kl)" % (dec, valC1))

        # partial widths as a function of kl, kt, and CV
        self.modelBuilder.factory_("expr::CVktkl_Gscal_Z('(@0*@0+@3) * @1 * @2', CV, SM_BR_hzz, HiggsDecayWidth_UncertaintyScaling_hzz, kl_scalBR_hzz)")
        self.modelBuilder.factory_("expr::CVktkl_Gscal_W('(@0*@0+@3) * @1 * @2', CV, SM_BR_hww, HiggsDecayWidth_UncertaintyScaling_hww, kl_scalBR_hww)")
        self.modelBuilder.factory_("expr::CVktkl_Gscal_tau('(1+@4) * @0 * @2 + (1+@5)*@1*@3', SM_BR_htt, SM_BR_hmm, HiggsDecayWidth_UncertaintyScaling_htt, HiggsDecayWidth_UncertaintyScaling_hmm,kl_scalBR_htt, kl_scalBR_hmm)")
        self.modelBuilder.factory_("expr::CVktkl_Gscal_top('(1+@2) * @0 * @1', SM_BR_hcc, HiggsDecayWidth_UncertaintyScaling_hcc, kl_scalBR_hcc)")
        self.modelBuilder.factory_("expr::CVktkl_Gscal_bottom('(1+@3) * (@0*@2+@1)', SM_BR_hbb, SM_BR_hss, HiggsDecayWidth_UncertaintyScaling_hbb, kl_scalBR_hbb)")
        self.modelBuilder.factory_("expr::CVktkl_Gscal_gluon('(@0+@3) * @1 * @2', Scaling_hgluglu, SM_BR_hgluglu, HiggsDecayWidth_UncertaintyScaling_hgluglu, kl_scalBR_hgluglu)")
        # no kl dependance on H->zg known yet ?
        self.modelBuilder.factory_("expr::CVktkl_Gscal_gamma('(@0+@6)*@1*@4 + @2*@3*@5', Scaling_hgg, SM_BR_hgg, Scaling_hzg, SM_BR_hzg, HiggsDecayWidth_UncertaintyScaling_hgg, HiggsDecayWidth_UncertaintyScaling_hzg, kl_scalBR_hgg)")
        # fix to have all BRs add up to unity
        self.modelBuilder.factory_("sum::CVktkl_SMBRs(%s)" % (",".join("SM_BR_" + d for d in SM_HIGG_DECAYS)))
        # self.modelBuilder.out.function("CVktkl_SMBRs").Print("")

        # total witdh, normalized to the SM one (just the sum over the partial widths/SM total BR)
        self.modelBuilder.factory_("expr::CVktkl_Gscal_tot('(@0+@1+@2+@3+@4+@5+@6)/@7', CVktkl_Gscal_Z, CVktkl_Gscal_W, CVktkl_Gscal_tau, CVktkl_Gscal_top, CVktkl_Gscal_bottom, CVktkl_Gscal_gluon, CVktkl_Gscal_gamma, CVktkl_SMBRs)")

        # BRs, normalized to the SM ones: they scale as (partial/partial_SM) / (total/total_SM)
        self.modelBuilder.factory_("expr::CVktkl_BRscal_hww('(@0*@0+@3)*@2/@1', CV, CVktkl_Gscal_tot, HiggsDecayWidth_UncertaintyScaling_hww, kl_scalBR_hww)")
        self.modelBuilder.factory_("expr::CVktkl_BRscal_hzz('(@0*@0+@3)*@2/@1', CV, CVktkl_Gscal_tot, HiggsDecayWidth_UncertaintyScaling_hzz, kl_scalBR_hzz)")
        self.modelBuilder.factory_("expr::CVktkl_BRscal_htt('(1+@2)*@1/@0', CVktkl_Gscal_tot, HiggsDecayWidth_UncertaintyScaling_htt, kl_scalBR_htt)")
        self.modelBuilder.factory_("expr::CVktkl_BRscal_hmm('(1+@2)*@1/@0', CVktkl_Gscal_tot, HiggsDecayWidth_UncertaintyScaling_hmm, kl_scalBR_hmm)")
        self.modelBuilder.factory_("expr::CVktkl_BRscal_hbb('(1+@2)*@1/@0', CVktkl_Gscal_tot, HiggsDecayWidth_UncertaintyScaling_hbb, kl_scalBR_hbb)")
        self.modelBuilder.factory_("expr::CVktkl_BRscal_hcc('(1+@2)*@1/@0', CVktkl_Gscal_tot, HiggsDecayWidth_UncertaintyScaling_hcc, kl_scalBR_hcc)")
        self.modelBuilder.factory_("expr::CVktkl_BRscal_hss('(1+@2)*@1/@0', CVktkl_Gscal_tot, HiggsDecayWidth_UncertaintyScaling_hss, kl_scalBR_hss)")
        self.modelBuilder.factory_("expr::CVktkl_BRscal_hgg('(@0+@3)*@2/@1', Scaling_hgg, CVktkl_Gscal_tot, HiggsDecayWidth_UncertaintyScaling_hgg,kl_scalBR_hgg)")
        self.modelBuilder.factory_("expr::CVktkl_BRscal_hzg('@0*@2/@1', Scaling_hzg, CVktkl_Gscal_tot, HiggsDecayWidth_UncertaintyScaling_hzg)")
        self.modelBuilder.factory_("expr::CVktkl_BRscal_hgluglu('(@0+@3)*@2/@1', Scaling_hgluglu, CVktkl_Gscal_tot, HiggsDecayWidth_UncertaintyScaling_hgluglu, kl_scalBR_hgluglu)")

        for d in SM_HIGG_DECAYS:
            self.f_BR_scalings.append("CVktkl_BRscal_%s" % d)

    def buildHScalings(self):
        # get VBF, tHq, tHW, ggZH cross section and resolved loops
        self.SMH.makeScaling("qqH", CW="CV", CZ="CV")
        self.SMH.makeScaling("tHq", CW="CV", Ctop="kt")
        self.SMH.makeScaling("tHW", CW="CV", Ctop="kt")
        self.SMH.makeScaling("ggZH", CZ="CV", Ctop="kt", Cb="1")
        self.SMH.makeScaling("ggH", Cb="1", Ctop="kt", Cc="1")

        for production in SM_HIGG_PROD:
            if production in ["ggZH", "tHq", "tHW"]:
                self.modelBuilder.factory_("expr::CVktkl_pos_XSscal_%s_%s('0.+@0*(@0>0)', Scaling_%s_%s)"
                                           % (production, energy, production, energy))
                self.f_H_scalings.append("CVktkl_pos_XSscal_%s_%s" % (production, energy))

            elif production in ["ggH", "qqH"]:
                EWK = EWKmap_13[production]
                self.modelBuilder.factory_("expr::CVktkl_XSscal_%s_%s('(@1+(@0-1)*%g/%g)/((1-(@0*@0-1)*%g))', kl, Scaling_%s_%s)"
                                           % (production,energy,cXSmap_13[production],EWK,dZH,production,energy))
                self.modelBuilder.factory_("expr::CVktkl_pos_XSscal_%s_%s('0.+@0*(@0>0)', CVktkl_XSscal_%s_%s)"
                                           % (production, energy, production, energy))
                self.f_H_scalings.append("CVktkl_pos_XSscal_%s_%s"%(production, energy))

            elif production in ["ZH", "WH", "VH"]:
                EWK = EWKmap_13[production]
                self.modelBuilder.factory_("expr::CVktkl_XSscal_%s_%s('(@1*@1+(@0-1)*%g/%g)/((1-(@0*@0-1)*%g))', kl, CV)"
                                           % (production, energy, cXSmap_13[production], EWK, dZH))
                self.modelBuilder.factory_("expr::CVktkl_pos_XSscal_%s_%s('0.+@0*(@0>0)', CVktkl_XSscal_%s_%s)"
                                           % (production, energy, production, energy))
                self.f_H_scalings.append("CVktkl_pos_XSscal_%s_%s" % (production, energy))

            elif production == "ttH":
                EWK = EWKmap_13[production]
                self.modelBuilder.factory_("expr::CVktkl_XSscal_%s_%s('(@1*@1+(@0-1)*%g/%g)/((1-(@0*@0-1)*%g))', kl, kt)"
                                           % (production, energy, cXSmap_13[production], EWK, dZH))
                self.modelBuilder.factory_("expr::CVktkl_pos_XSscal_%s_%s('0.+@0*(@0>0)', CVktkl_XSscal_%s_%s)"
                                           % (production, energy, production, energy))
                self.f_H_scalings.append("CVktkl_pos_XSscal_%s_%s" % (production, energy))

    def findBRScalings(self, process):
        # to automatically parse the higgs decay from the process name, we need a fixed format,
        # "*_hXX[hYY[...]]Z" where hXX, hYY, etc must be in the offical SM_HIGG_DECAYS, and Z can be
        # an additional sub decay identifier that does not contain underscores
        BRstr = process.split("_")[-1]
        # extract valid decays
        decays = []
        m = re.match("^(({})+)(.*)$".format("|".join(SM_HIGG_DECAYS)), BRstr)
        if m:
            decays = re.findall("|".join(SM_HIGG_DECAYS), m.group(1))
        # create scaling names
        proc_f_BR_scalings = ["CVktkl_BRscal_" + d for d in decays]

        # make sure that are all scalings are supported
        for proc_f_BR_scaling in proc_f_BR_scalings:
            if proc_f_BR_scaling not in self.f_BR_scalings:
                raise Exception("braching ratio scaling {} for process {} is not supported".format(
                    proc_f_BR_scaling, process))

        return proc_f_BR_scalings, BRstr

    def findSingleHMatch(self, bin, process):
        # single H process names must have the format "PROD_*" where PROD must be in
        # SM_SCALED_SINGLE_HIGG_PROD; None is returned when this is not the case
        production = process.split("_")[0]
        if production not in SM_SCALED_SINGLE_HIGG_PROD:
            return None

        proc_f_H_scaling = "CVktkl_pos_XSscal_%s_%s" % (production, energy)

        if proc_f_H_scaling not in self.f_H_scalings:
            raise Exception("scaling is not supported for single H process {} (bin {}), "
                "this is likely a misconfiguration".format(process, bin))

        return proc_f_H_scaling

    def buildXSBRScalingHH(self, f_XS_scaling, bin, process):
        BRscalings, BRstr = self.findBRScalings(process)

        # when no scalings were found, print a warning since this might be intentional and return,
        # when != 2 scalings were found, this is most likely an error
        if not BRscalings:
            print("WARNING: the HH process {} (bin {}) does not contain valid decay strings to "
                "extract branching ratios to apply the scaling with model parameters".format(
                    process, bin))
            return None
        elif len(BRscalings) != 2:
            raise Exception("the HH process {} (bin {}) contains {} valid decay string(s) while "
                "two were expected".format(process, bin, len(BRscalings)))

        f_XSBR_scaling = "%s_BRscal_%s" % (f_XS_scaling, BRstr)

        if not self.modelBuilder.out.function(f_XSBR_scaling):
            self.modelBuilder.factory_("expr::%s('@0*@1*@2', %s, %s, %s)" % (f_XSBR_scaling, f_XS_scaling, BRscalings[0], BRscalings[1]))

        return f_XSBR_scaling

    def buildXSBRScalingH(self, f_XS_scaling, bin, process):
        BRscalings, BRstr = self.findBRScalings(process)

        # when no scalings were found, print a warning since this might be intentional and return,
        # when != 1 scalings were found, this is most likely an error
        if not BRscalings:
            print("WARNING: the H process {} (bin {}) does not contain valid decay strings to "
                "extract branching ratios to apply the scaling with model parameters".format(
                    process, bin))
            return None
        elif len(BRscalings) != 1:
            raise Exception("the H process {} (bin {}) contains {} valid decay string(s) while one "
                "was expected".format(process, bin, len(BRscalings)))

        f_XSBR_scaling = "%s_BRscal_%s" % (f_XS_scaling, BRstr)

        if not self.modelBuilder.out.function(f_XSBR_scaling):
            self.modelBuilder.factory_("expr::%s('@0*@1', %s, %s)" % (f_XSBR_scaling, f_XS_scaling, BRscalings[0]))

        return f_XSBR_scaling
