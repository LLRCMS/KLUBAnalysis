###################################
# Author : L. Cadamuro (UF)
# Date   : 22/04/2020
# Brief  : code that implements the HH model in combine
# structure of the code :
# xxHHSample  -> defines the interface to the user, that will pass the xs and the coupling setups
# xxHHFormula -> implements the matrix component representation, that calculates the symbolic scalings
# HHModel     -> implements the interfaces to combine
###################################


from HiggsAnalysis.CombinedLimit.PhysicsModel import *
from sympy import *
from numpy import matrix
from numpy import linalg
from sympy import Matrix
from collections import OrderedDict, defaultdict


class VBFHHSample:
    def __init__(self, val_CV, val_C2V, val_kl, val_xs, label):
        self.val_CV  = val_CV
        self.val_C2V = val_C2V
        self.val_kl  = val_kl
        self.val_xs  = val_xs
        self.label   = label

####################

class GGFHHSample:
    def __init__(self, val_kl, val_kt, val_xs, label):
        self.val_kl  = val_kl
        self.val_kt  = val_kt
        self.val_xs  = val_xs
        self.label   = label

####################

class GGFHHFormula:
    def __init__(self, sample_list):
        self.sample_list = sample_list
        self.build_matrix()
        self.calculatecoeffients()

    def build_matrix(self):
        """ create the matrix M in this object """

        # the code will combine as many samples as passed to the input
        # into a matrix with 3 columns and Nsamples rows
        nrows = len(self.sample_list)
        ncols = 3
        M_tofill = [[None]*ncols for i in range(nrows)]

        for isample, sample in enumerate(self.sample_list):

            ## implement the 3 scalings - box, triangle, interf
            M_tofill[isample][0] = sample.val_kt**4
            M_tofill[isample][1] = sample.val_kt**2 * sample.val_kl**2
            M_tofill[isample][2] = sample.val_kt**3 * sample.val_kl

        # print M_tofill
        self.M = Matrix(M_tofill)

    def calculatecoeffients(self):
        """ create the function sigma and the six coefficients in this object """

        try: self.M
        except AttributeError: self.build_matrix()

        # ##############################################
        kl, kt, box, tri, interf = symbols('kl kt box tri interf')
        samples_symb = OrderedDict() # order is essential -> OrderedDict
        Nsamples     = self.M.shape[0] #num rows
        for i in range(Nsamples):
            sname = 's%i' % i
            samples_symb[sname] = Symbol(sname)

        ### the vector of couplings
        c = Matrix([
            [kt**4]         ,
            [kt**2 * kl**2] ,
            [kt**3 * kl]    ,
        ])

        ### the vector of components
        v = Matrix([
            [box]   ,
            [tri]   ,
            [interf],
        ])

        ### the vector of samples (i.e. cross sections)
        symb_list = [[sam] for sam in samples_symb.values()]
        s = Matrix(symb_list)

        ####
        Minv   = self.M.pinv()
        self.coeffs = c.transpose() * Minv # coeffs * s is the sigma, accessing per component gives each sample scaling
        self.sigma  = self.coeffs*s

#########################################

class VBFHHFormula:
    def __init__(self, sample_list):
        self.sample_list = sample_list
        self.build_matrix()
        self.calculatecoeffients()

    def build_matrix(self):
        """ create the matrix M in this object """

        # the code will combine as many samples as passed to the input
        # into a matrix with 6 columns and Nsamples rows
        nrows = len(self.sample_list)
        ncols = 6
        M_tofill = [[None]*ncols for i in range(nrows)]

        for isample, sample in enumerate(self.sample_list):

            # implement the 3 scalings - box, triangle, interf
            M_tofill[isample][0] = sample.val_CV**2 * sample.val_kl**2
            M_tofill[isample][1] = sample.val_CV**4
            M_tofill[isample][2] = sample.val_C2V**2
            M_tofill[isample][3] = sample.val_CV**3 * sample.val_kl
            M_tofill[isample][4] = sample.val_CV    * sample.val_C2V    * sample.val_kl
            M_tofill[isample][5] = sample.val_CV**2 * sample.val_C2V

        # print M_tofill
        self.M = Matrix(M_tofill)


    def calculatecoeffients(self):
        """ create the function sigma and the six coefficients in this object """

        try: self.M
        except AttributeError: self.build_matrix()

        ##############################################
        CV, C2V, kl, a, b, c, iab, iac, ibc = symbols('CV C2V kl a b c iab iac ibc')
        samples_symb = OrderedDict() # order is essential -> OrderedDict
        Nsamples     = self.M.shape[0] #num rows
        for i in range(Nsamples):
            sname = 's%i' % i
            samples_symb[sname] = Symbol(sname)

        ### the vector of couplings
        c = Matrix([
            [CV**2 * kl**2] ,
            [CV**4]         ,
            [C2V**2]        ,
            [CV**3 * kl]    ,
            [CV * C2V * kl] ,
            [CV**2 * C2V]
        ])

        ### the vector of components
        v = Matrix([
            [a]   ,
            [b]   ,
            [c]   ,
            [iab] ,
            [iac] ,
            [ibc]
        ])

        ### the vector of samples (i.e. cross sections)
        symb_list = [[sam] for sam in samples_symb.values()]
        s = Matrix(symb_list)

        ####
        Minv   = self.M.pinv()
        self.coeffs = c.transpose() * Minv # coeffs * s is the sigma, accessing per component gives each sample scaling
        self.sigma  = self.coeffs*s

#########################################


class HHModel(PhysicsModel):
    """ Models the HH production as linear sum of the input components for VBF (>= 6) and GGF (>= 3) """

    def __init__(self, ggf_sample_list, vbf_sample_list, name):
        PhysicsModel.__init__(self)
        self.doNNLOscaling    = True
        self.doklDependentUnc = True
        self.klUncName        = "THU_HH"
        self.name             = name

        self.check_validity_ggf(ggf_sample_list)
        self.check_validity_vbf(vbf_sample_list)

        self.ggf_formula = GGFHHFormula(ggf_sample_list)
        self.vbf_formula = VBFHHFormula(vbf_sample_list)

        self.scalingMap = defaultdict(list)

        #self.dump_inputs()

    def setPhysicsOptions(self, physOptions):
        opts = [opt.split("=", 1) for opt in physOptions if "=" in opt]
        for key, value in opts:
            if key == "doNNLOscaling":
                self.doNNLOscaling = value.lower() in ["yes", "true", "1"]
                print("[INFO] set doNNLOscaling of model {} to {}".format(
                    self.name, self.doNNLOscaling))
            if key == "doklDependentUnc":
                self.doklDependentUnc = value.lower() in ["yes", "true", "1"]
                print("[INFO] set doklDependentUnc of model {} to {}".format(
                    self.name, self.doklDependentUnc))

    def preProcessNuisances(self, nuisances):
        ''' this method is executed before nuisances are processed'''
        if not self.doklDependentUnc: return
        nuisances.append((self.klUncName, False, "param", [ "0", "1"], [] ) )

    def makeInterpolation(self, nameout, nameHi, nameLo, x):
        # as in https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit/blob/102x/interface/ProcessNormalization.h
        ## maybe we can try to reuse that should be fast
        d = {"name":nameout, "hi":nameHi, "lo":nameLo, 'x':x}
        
        d['logKhi']    = "log({hi})"  .format(**d)
        d['logKlo']    = "-log({lo})" .format(**d)
        d['avg']       = "0.5*({logKhi} + {logKlo})".format(**d)
        d['halfdiff']  = "0.5*({logKhi} - {logKlo})".format(**d)
        d["twox"]      = "2*{x}".format(**d)
        d["twox2"]     = "({twox})*({twox})".format(**d)
        d['alpha']     = '0.125 * {twox} * ({twox2} * ({twox2} - 10.) + 15.)'.format(**d)
        
        d['retCent']   = "{avg}+{alpha}*{halfdiff}".format(**d)
        d['retLow']    = d['logKlo']
        d['retHigh']   = d['logKhi']
        d['retFull'] = "{x} <= -0.5 ? ({retLow}) : {x} >= 0.5 ? ({retHigh}) : ({retCent})".format(**d)
        
        d['ret'] = 'expr::{name}("exp({retFull})",{{{hi},{lo},{x}}})'.format(**d)
        
        # print "[DEBUG]","[makeInterpolation]","going to build: ",d['ret']
        self.modelBuilder.factory_(d['ret'])
        
        return

    def makeklDepTheoUncertainties(self):
        ''' Construct and import uncertanties on the workspace'''
        #upper_unc[kl] = Max[72.0744-51.7362*kl+11.3712*kl2, 70.9286-51.5708*kl+11.4497*kl2] in fb.
        #lower_unc[kl] = Min[66.0621-46.7458*kl+10.1673*kl2, 66.7581-47.721*kl+10.4535*kl2] in fb.
        # if not self.doklDependentUnc: return

        self.modelBuilder.doVar("%s[-7,7]" % self.klUncName)
        
        self.modelBuilder.factory_('expr::%s_kappaHi("max(72.0744-51.7362*@0+11.3712*@0*@0,70.9286-51.5708*@0+11.4497*@0*@0) / (70.3874 - 50.4111*@0 + 11.0595*@0*@0)",kl)' % self.klUncName)
        self.modelBuilder.factory_('expr::%s_kappaLo("min(66.0621-46.7458*@0+10.1673*@0*@0,66.7581-47.721*@0+10.4535*@0*@0)  / (70.3874 - 50.4111*@0 + 11.0595*@0*@0)",kl)'  % self.klUncName)

        self.makeInterpolation("%s_kappa" % self.klUncName, "%s_kappaHi" % self.klUncName, "%s_kappaLo" % self.klUncName , self.klUncName)
        
        ## make scaling
        self.modelBuilder.factory_("expr::scaling_{name}(\"pow(@0,@1)\",{name}_kappa,{name})".format(name=self.klUncName)) 
        return

    def check_validity_ggf(self, ggf_sample_list):
        if len(ggf_sample_list) < 3:
            raise RuntimeError("%s : malformed GGF input sample list - expect at least 3 samples" % self.name)
        if not isinstance(ggf_sample_list, list) and not isinstance(ggf_sample_list, tuple):
            raise RuntimeError("%s : malformed GGF input sample list - expect list or tuple" % self.name)
        for s in ggf_sample_list:
            if not isinstance(s, GGFHHSample):
                raise RuntimeError("%s : malformed GGF input sample list - each element must be a GGFHHSample" % self.name)

    def check_validity_vbf(self, vbf_sample_list):
        if len(vbf_sample_list) < 6:
            raise RuntimeError("%s : malformed VBF input sample list - expect at least 6 samples" % self.name)
        if not isinstance(vbf_sample_list, list) and not isinstance(vbf_sample_list, tuple):
            raise RuntimeError("%s : malformed VBF input sample list - expect list or tuple" % self.name)
        for s in vbf_sample_list:
            if not isinstance(s, VBFHHSample):
                raise RuntimeError("%s : malformed VBF input sample list - each element must be a VBFHHSample" % self.name)


    def dump_inputs(self):
        print "[INFO]  HH model : " , self.name
        print "......  GGF configuration"
        for i,s in enumerate(self.ggf_formula.sample_list):
            print "        {0:<3} ... kl : {1:<3}, kt : {2:<3}, xs : {3:<3.8f} pb, label : {4}".format(i, s.val_kl, s.val_kt, s.val_xs, s.label)
        print "......  VBF configuration"
        for i,s in enumerate(self.vbf_formula.sample_list):
            print "        {0:<3} ... CV : {1:<3}, C2V : {2:<3}, kl : {3:<3}, xs : {4:<3.8f} pb, label : {5}".format(i, s.val_CV, s.val_C2V, s.val_kl, s.val_xs, s.label)


    def doParametersOfInterest(self):

        ## the model is built with:
        ## r x [GGF + VBF]
        ## GGF = r_GGF x [sum samples(kl, kt)]
        ## VBF = r_VBF x [sum samples(kl, CV, C2V)]

        POIs = "r,r_gghh,r_qqhh,CV,C2V,kl,kt"

        self.modelBuilder.doVar("r[1,-20,20]")
        self.modelBuilder.doVar("r_gghh[1,-20,20]")
        self.modelBuilder.doVar("r_qqhh[1,-20,20]")
        self.modelBuilder.doVar("CV[1,-10,10]")
        self.modelBuilder.doVar("C2V[1,-10,10]")
        self.modelBuilder.doVar("kl[1,-30,30]")
        self.modelBuilder.doVar("kt[1,-10,10]")

        self.modelBuilder.doSet("POI",POIs)

        self.modelBuilder.out.var("r_gghh") .setConstant(True)
        self.modelBuilder.out.var("r_qqhh") .setConstant(True)
        self.modelBuilder.out.var("CV")     .setConstant(True)
        self.modelBuilder.out.var("C2V")    .setConstant(True)
        self.modelBuilder.out.var("kl")     .setConstant(True)
        self.modelBuilder.out.var("kt")     .setConstant(True)

        if self.doklDependentUnc:
            self.makeklDepTheoUncertainties()
        self.create_scalings()

    def create_scalings(self):
        """ create the functions that scale the >= 6 components of vbf and the >= 3 components of ggf """

        self.f_r_vbf_names = [] # the RooFormulae that scale the components (VBF)
        self.f_r_ggf_names = [] # the RooFormulae that scale the components (GGF)

        def pow_to_mul_string(expr):
            """ Convert integer powers in an expression to Muls, like a**2 => a*a. Returns a string """
            pows = list(expr.atoms(Pow))
            if any(not e.is_Integer for b, e in (i.as_base_exp() for i in pows)):
                raise ValueError("A power contains a non-integer exponent")
            s = str(expr)
            repl = zip(pows, (Mul(*[b]*e,evaluate=False) for b,e in (i.as_base_exp() for i in pows)))
            for fr, to in repl:
                s = s.replace(str(fr), str(to))
            return s

        ### loop on the GGF scalings
        for i, s in enumerate(self.ggf_formula.sample_list):
            # f_name = 'f_ggfhhscale_sample_{0}'.format(i)
            f_name = 'f_ggfhhscale_sample_{0}'.format(s.label)
            f_expr = self.ggf_formula.coeffs[i] # the function that multiplies each sample

            kl = symbols('kl')
            #NLO xsec formula
            f_NLO_xsec = '62.5339 - 44.3231*kl + 9.6340*kl*kl'
            #NNLO xsec formula https://twiki.cern.ch/twiki/bin/view/LHCPhysics/LHCHXSWGHH#Latest_recommendations_for_gluon
            f_NNLO_xsec = '70.3874 - 50.4111*kl + 11.0595*kl*kl'

            # print f_expr
            # for ROOFit, this will convert expressions as a**2 to a*a
            s_expr = pow_to_mul_string(f_expr)

            couplings_in_expr = []
            if 'kl'  in s_expr: couplings_in_expr.append('kl')
            if 'kt'  in s_expr: couplings_in_expr.append('kt')

            # no constant expressions are expected
            if len(couplings_in_expr) == 0:
                raise RuntimeError('GGF HH : scaling expression has no coefficients')

            for idx, ce in enumerate(couplings_in_expr):
                # print '..replacing', ce
                symb = '@{}'.format(idx)
                s_expr = s_expr.replace(ce, symb)

            # embed the scaling due to the xs uncertainty
            if self.doklDependentUnc:
                s_expr = 'scaling_{name} * ({expr})'.format(name=self.klUncName, expr=s_expr)
                couplings_in_expr.append('scaling_{name}'.format(name=self.klUncName))

            if(self.doNNLOscaling):
                #print str(f_expr)
                if('kl' not in str(f_expr)): couplings_in_expr.append('kl')

                for idx, ce in enumerate(couplings_in_expr):
                    symb = '@{}'.format(idx)
                    f_NLO_xsec = f_NLO_xsec.replace(ce, symb)
                    f_NNLO_xsec = f_NNLO_xsec.replace(ce, symb)

                arglist = ','.join(couplings_in_expr)
                #this will scale NNLO_xsec
                exprname = 'expr::{}("({}) / (1.115 * ({}) / ({}))" , {})'.format(f_name, s_expr, f_NLO_xsec, f_NNLO_xsec, arglist)
                self.modelBuilder.factory_(exprname) # the function that scales each VBF sample
                #self.modelBuilder.out.function(f_name).Print("")

            else:
                arglist = ','.join(couplings_in_expr)
                exprname = 'expr::{}(\"{}\" , {})'.format(f_name, s_expr, arglist)
                self.modelBuilder.factory_(exprname) # the function that scales each VBF sample

            # print exprname

            f_prod_name_pmode = f_name + '_r_gghh'
            prodname_pmode = 'prod::{}(r_gghh,{})'.format(f_prod_name_pmode, f_name)
            self.modelBuilder.factory_(prodname_pmode)  ## the function that scales this production mode
            # self.modelBuilder.out.function(f_prod_name).Print("") ## will just print out the values

            f_prod_name = f_prod_name_pmode + '_r'
            prodname = 'prod::{}(r,{})'.format(f_prod_name, f_prod_name_pmode)
            self.modelBuilder.factory_(prodname)  ## the function that scales this production mode
            # self.modelBuilder.out.function(f_prod_name).Print("") ## will just print out the values

            self.f_r_ggf_names.append(f_prod_name) #bookkeep the scaling that has been created

        ### loop on the VBF scalings
        for i, s in enumerate(self.vbf_formula.sample_list):
            # f_name = 'f_vbfhhscale_sample_{0}'.format(i)
            f_name = 'f_vbfhhscale_sample_{0}'.format(s.label)
            f_expr = self.vbf_formula.coeffs[i] # the function that multiplies each sample

            # print f_expr
            # for ROOFit, this will convert expressions as a**2 to a*a
            s_expr = pow_to_mul_string(f_expr)

            couplings_in_expr = []
            if 'CV'  in s_expr: couplings_in_expr.append('CV')
            if 'C2V' in s_expr: couplings_in_expr.append('C2V')
            if 'kl'  in s_expr: couplings_in_expr.append('kl')

            # no constant expressions are expected
            if len(couplings_in_expr) == 0:
                raise RuntimeError('VBF HH : scaling expression has no coefficients')

            for idx, ce in enumerate(couplings_in_expr):
                # print '..replacing', ce
                symb = '@{}'.format(idx)
                s_expr = s_expr.replace(ce, symb)

            arglist = ','.join(couplings_in_expr)
            exprname = 'expr::{}(\"{}\" , {})'.format(f_name, s_expr, arglist)
            # print exprname
            self.modelBuilder.factory_(exprname) # the function that scales each VBF sample

            f_prod_name_pmode = f_name + '_r_qqhh'
            prodname_pmode = 'prod::{}(r_qqhh,{})'.format(f_prod_name_pmode, f_name)
            self.modelBuilder.factory_(prodname_pmode)  ## the function that scales this production mode
            # self.modelBuilder.out.function(f_prod_name_pmode).Print("") ## will just print out the values

            f_prod_name = f_prod_name_pmode + '_r'
            prodname = 'prod::{}(r,{})'.format(f_prod_name, f_prod_name_pmode)
            self.modelBuilder.factory_(prodname)  ## the function that scales this production mode
            # self.modelBuilder.out.function(f_prod_name).Print("") ## will just print out the values

            self.f_r_vbf_names.append(f_prod_name) #bookkeep the scaling that has been created

    def getYieldScale(self, bin, process):
        # only deal with signal processes here
        if not self.DC.isSignal[process]:
            return 1.

        def find_matches(samples, kind):
            # get the matching ggf sample index
            matching_indices = []
            for i, sample in enumerate(samples):
                if process.startswith(sample.label):
                    matching_indices.append(i)

            # complain when there is more than one hit
            if len(matching_indices) > 1:
                raise Exception("found {} matches for {} signal process {} in bin {}".format(
                    len(matching_indices), kind, process, bin))

            # return the index when there is only one hit
            if len(matching_indices) == 1:
                return matching_indices[0]

            # otherwise, return nothing
            return None

        # ggf match?
        isample = find_matches(self.ggf_formula.sample_list, "GGF")
        if isample is not None:
            self.scalingMap[process].append((isample, "GGF"))
            return self.f_r_ggf_names[isample]

        # vbf match?
        isample = find_matches(self.vbf_formula.sample_list, "VBF")
        if isample is not None:
            self.scalingMap[process].append((isample, "VBF"))
            return self.f_r_vbf_names[isample]

        # complain when neither a ggf nor a vbf match was found
        raise Exception("signal process {} did not match any GGF or VBF samples in bin {}".format(
            process, bin))


# ggf samples with keys (kl, kt), ordered by kl
# cross section values are NLO with k-factor applied and only used in create_ggf_xsec_func below
ggf_samples = OrderedDict([
    ((0, 1),    GGFHHSample(0,    1, val_xs=0.069725, label="ggHH_kl_0_kt_1")),
    ((1, 1),    GGFHHSample(1,    1, val_xs=0.031047, label="ggHH_kl_1_kt_1")),
    ((2.45, 1), GGFHHSample(2.45, 1, val_xs=0.013124, label="ggHH_kl_2p45_kt_1")),
    ((5, 1),    GGFHHSample(5,    1, val_xs=0.091172, label="ggHH_kl_5_kt_1")),
])

# vbf samples with keys (CV, C2V, kl), SM point first, then ordered by kl, then C2V, then CV
# cross section values are LO (from 2017/2018 gridpacks) x SM k-factor for N3LO (1.03477) and only used in create_vbf_xsec_func below
vbf_samples = OrderedDict([
    ((1,   1, 1), VBFHHSample(1,   1, 1, val_xs=0.0017260, label="qqHH_CV_1_C2V_1_kl_1")),
    ((1,   1, 0), VBFHHSample(1,   1, 0, val_xs=0.0046089, label="qqHH_CV_1_C2V_1_kl_0")),
    ((1,   1, 2), VBFHHSample(1,   1, 2, val_xs=0.0014228, label="qqHH_CV_1_C2V_1_kl_2")),
    ((1,   0, 1), VBFHHSample(1,   0, 1, val_xs=0.0270800, label="qqHH_CV_1_C2V_0_kl_1")),
    ((1,   2, 1), VBFHHSample(1,   2, 1, val_xs=0.0142178, label="qqHH_CV_1_C2V_2_kl_1")),
    ((0.5, 1, 1), VBFHHSample(0.5, 1, 1, val_xs=0.0108237, label="qqHH_CV_0p5_C2V_1_kl_1")),
    ((1.5, 1, 1), VBFHHSample(1.5, 1, 1, val_xs=0.0660185, label="qqHH_CV_1p5_C2V_1_kl_1")),
])


def get_ggf_samples(keys):
    """
    Returns a list of :py:class:`GGFHHSample` instances that are mapped to certain *keys* in the
    ordered *ggf_samples* dictionary above. A key can either be a tuple of (kl, kt) values as used
    in the dict, or a numeric index. Example:

    .. code-block:: python

        get_ggf_samples([(2.45, 1), 3])
        # -> [GGFHHSample:ggHH_kl_2p45_kt_1, GGFHHSample:ggHH_kl_5_kt_1]
    """
    all_keys = list(ggf_samples.keys())
    return [
        ggf_samples[key if isinstance(key, tuple) else all_keys[key]]
        for key in keys
    ]


def get_vbf_samples(keys):
    """
    Returns a list of :py:class:`VBFHHSample` instances that are mapped to certain *keys* in the
    ordered *vbf_samples* dictionary above. A key can either be a tuple of (CV, C2V, kl) values as
    used in the dict, or a numeric index. Example:

    .. code-block:: python

        get_vbf_samples([2, (1, 2, 1)])
        # -> [VBFHHSample:qqHH_CV_1_C2V_1_kl_2, VBFHHSample:qqHH_CV_1_C2V_2_kl_1]
    """
    all_keys = list(ggf_samples.keys())
    return [
        vbf_samples[key if isinstance(key, tuple) else all_keys[key]]
        for key in keys
    ]


def create_model(name, skip_ggf=None, skip_vbf=None):
    """
    Returns a new :py:class:`HHModel` instance named *name*. Its ggf and vbf sample lists are
    using all availabe samples except those defined in *skip_ggf* and *skip_vbf*, respectively,
    through :py:func:`get_ggf_samples` and :py:func:`get_vbf_samples`. The order of passed keys to
    be skipped does not matter.
    """
    # get all unskipped ggf keys
    ggf_keys = []
    for i, key in enumerate(ggf_samples):
        if not skip_ggf or not any(skip_key in (i, key) for skip_key in skip_ggf):
            ggf_keys.append(key)

    # get all unskipped vbf keys
    vbf_keys = []
    for i, key in enumerate(vbf_samples):
        if not skip_vbf or not any(skip_key in (i, key) for skip_key in skip_vbf):
            vbf_keys.append(key)

    # create the return the model
    return HHModel(
        ggf_sample_list=get_ggf_samples(ggf_keys),
        vbf_sample_list=get_vbf_samples(vbf_keys),
        name=name,
    )


# some named, default models
model_all      = create_model("model_all")
model_default  = create_model("model_default",  skip_ggf=[(0, 1)], skip_vbf=[(1, 0, 1)])
model_bbgg     = create_model("model_bbgg",     skip_ggf=[(0, 1)], skip_vbf=[(0.5, 1, 1)])
model_bbtautau = create_model("model_bbtautau", skip_ggf=[(0, 1)], skip_vbf=[(0.5, 1, 1)])
# to be decided and then set as the new default
# model_comb     = create_model("model_comb", skip_ggf=[(...)], skip_vbf=[(...)])

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


# legacy objects for development
GGF_sample_list = [
    # GGFHHSample(0,    1, val_xs=0.069725, label="ggHH_kl_0_kt_1"),
    GGFHHSample(1,    1, val_xs=0.031047, label="ggHH_kl_1_kt_1"),
    GGFHHSample(2.45, 1, val_xs=0.013124, label="ggHH_kl_2p45_kt_1"),
    GGFHHSample(5,    1, val_xs=0.091172, label="ggHH_kl_5_kt_1"),
]
VBF_sample_list = [
    VBFHHSample(1,   1, 1, val_xs=0.0017260, label="qqHH_CV_1_C2V_1_kl_1"),
    VBFHHSample(1,   1, 0, val_xs=0.0046089, label="qqHH_CV_1_C2V_1_kl_0"),
    VBFHHSample(1,   1, 2, val_xs=0.0014228, label="qqHH_CV_1_C2V_1_kl_2"),
    # VBFHHSample(1,   0, 1, val_xs=0.0270800, label="qqHH_CV_1_C2V_0_kl_1"),
    VBFHHSample(1,   2, 1, val_xs=0.0142178, label="qqHH_CV_1_C2V_2_kl_1"),
    VBFHHSample(0.5, 1, 1, val_xs=0.0108237, label="qqHH_CV_0p5_C2V_1_kl_1"),
    VBFHHSample(1.5, 1, 1, val_xs=0.0660185, label="qqHH_CV_1p5_C2V_1_kl_1"),
]
HHdefault = HHModel(
    ggf_sample_list=GGF_sample_list,
    vbf_sample_list=VBF_sample_list,
    name="HHdefault",
)


def create_ggf_xsec_func(formula=None):
    """
    Creates and returns a function that can be used to calculate numeric ggF cross section values in
    pb given an appropriate *formula*, which defaults to *model_default.ggf_formula*. The returned
    function has the signature ``(kl=1.0, kt=1.0, nnlo=True, unc=None)``. When *nnlo* is *False*,
    the constant k-factor is still applied. Otherwise, the returned value is in full
    next-to-next-to-leading order. In this case, *unc* can be set to eiher "up" or "down" to return
    the up / down varied cross section instead where the uncertainty is composed of a *kl* dependent
    scale uncertainty and an independent PDF uncertainty of 3%.

    Example:

    .. code-block:: python

        get_ggf_xec = create_ggf_xsec_func()

        print(get_ggf_xec(kl=2.))
        # -> 0.013803...

        print(get_ggf_xec(kl=2., nnlo=False))
        # -> 0.013852...

        print(get_ggf_xec(kl=2., unc="up"))
        # -> 0.014305...

    Formulas are taken from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/LHCHXSWGHH?rev=60.
    """
    if formula is None:
        formula = model_default.ggf_formula

    # create the lambdify'ed evaluation function
    n_samples = len(formula.sample_list)
    symbol_names = ["kl", "kt"] + list(map("s{}".format, range(n_samples)))
    func = lambdify(symbols(symbol_names), formula.sigma)

    # nlo-to-nnlo scaling functions in case nnlo is set
    xsec_nlo = lambda kl: 0.001 * 1.115 * (62.5339 - 44.3231 * kl + 9.6340 * kl**2.)
    xsec_nnlo = lambda kl: 0.001 * (70.3874 - 50.4111 * kl + 11.0595 * kl**2.)
    nlo2nnlo = lambda xsec, kl: xsec * xsec_nnlo(kl) / xsec_nlo(kl)

    # uncertainty application in case unc is set
    xsec_nnlo_scale_up = lambda kl: 0.001 * max(
        72.0744 - 51.7362 * kl + 11.3712 * kl**2.,
        70.9286 - 51.5708 * kl + 11.4497 * kl**2.,
    )
    xsec_nnlo_scale_down = lambda kl: 0.001 * min(
        66.0621 - 46.7458 * kl + 10.1673 * kl**2.,
        66.7581 - 47.7210 * kl + 10.4535 * kl**2.,
    )

    def apply_uncertainty_nnlo(kl, xsec_nom, unc):
        # compute absolute values of the scale uncertainties
        if unc.lower() == "up":
            xsec_unc = xsec_nnlo_scale_up(kl) - xsec_nom
        elif unc.lower() == "down":
            xsec_unc = xsec_nnlo_scale_down(kl) - xsec_nom
        else:
            raise ValueError("unc must be 'up' or 'down', got '{}'".format(unc))

        # combine with flat 3% PDF uncertainty, preserving the sign
        unc_sign = 1 if xsec_unc > 0 else -1
        xsec_unc = unc_sign * ((0.03 * xsec_nom)**2. + xsec_unc**2.)**0.5

        # add signed uncertainty back to nominal value
        xsec = xsec_nom + xsec_unc

        return xsec

    # wrap into another function to apply defaults and nlo-to-nnlo scaling
    def wrapper(kl=1., kt=1., nnlo=True, unc=None):
        xsec = func(kl, kt, *(sample.val_xs for sample in formula.sample_list))[0, 0]

        # nnlo scaling?
        if nnlo:
            xsec = nlo2nnlo(xsec, kl)

        # apply uncertainties?
        if unc:
            if not nnlo:
                raise NotImplementedError("NLO ggF cross section uncertainties are not implemented")
            xsec = apply_uncertainty_nnlo(kl, xsec, unc)

        return xsec

    return wrapper


def create_vbf_xsec_func(formula=None):
    """
    Creates and returns a function that can be used to calculate numeric VBF cross section values in
    pb given an appropriate *formula*, which defaults to *model_default.vbf_formula*. The returned
    function has the signature ``(c2v=1.0, cv=1.0, kl=1.0)``.

    Example:

    .. code-block:: python

        get_vbf_xsec = create_vbf_xsec_func()

        print(get_vbf_xsec(c2v=2.))
        # -> 0.013916... (or similar)
    """
    if formula is None:
        formula = model_default.vbf_formula

    # create the lambdify'ed evaluation function
    n_samples = len(formula.sample_list)
    symbol_names = ["C2V", "CV", "kl"] + list(map("s{}".format, range(n_samples)))
    func = lambdify(symbols(symbol_names), formula.sigma)

    # wrap into another function to apply defaults
    def wrapper(c2v=1., cv=1., kl=1.):
        xsec = func(c2v, cv, kl, *(sample.val_xs for sample in formula.sample_list))[0, 0]
        return xsec

    return wrapper


#: Default function for getting ggF cross sections using the formula of the *model_default* model.
get_ggf_xsec = create_ggf_xsec_func()

#: Default function for getting VBF cross sections using the formula of the *model_default* model.
get_vbf_xsec = create_vbf_xsec_func()
