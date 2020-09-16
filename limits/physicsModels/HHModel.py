###################################
# Author : L. Cadamuro (UF)
# Date   : 11/03/2020
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

class VBFHHFormula:
    def __init__(self, sample_list):
        self.sample_list = sample_list
        self.build_matrix()
        self.calculatecoeffients()        

    def build_matrix(self):    
        """ create the matrix M in this object """

        if len(self.sample_list) != 6:
            print "[ERROR] : expecting 6 samples in input"
            raise RuntimeError("malformed vbf input sample list")
        M_tofill = [
            [None,None,None,None,None,None],
            [None,None,None,None,None,None],
            [None,None,None,None,None,None],
            [None,None,None,None,None,None],
            [None,None,None,None,None,None],
            [None,None,None,None,None,None]
        ]

        for isample, sample in enumerate(self.sample_list):
            # print isample, " CV, C2V, kl = ", sample.val_CV, sample.val_C2V, sample.val_kl
            
            ## implement the 6 scalings
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
        CV, C2V, kl, a, b, c, iab, iac, ibc, s1, s2, s3, s4, s5, s6 = symbols('CV C2V kl a b c iab iac ibc s1 s2 s3 s4 s5 s6')    
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
        s = Matrix([
            [s1] ,
            [s2] ,
            [s3] ,
            [s4] ,
            [s5] ,
            [s6] 
        ])    
        ####    
        Minv   = self.M.inv()
        self.coeffs = c.transpose() * Minv # coeffs * s is the sigma, accessing per component gives each sample scaling
        self.sigma  = self.coeffs*s

####################

class GGFHHFormula:
    def __init__(self, sample_list):
        self.sample_list = sample_list
        self.build_matrix()
        self.calculatecoeffients()        

    def build_matrix(self):    
        """ create the matrix M in this object """

        if len(self.sample_list) != 3:
            print "[ERROR] : expecting 3 samples in input"
            raise RuntimeError("malformed ggf input sample list")
        M_tofill = [
            [None,None,None],
            [None,None,None],
            [None,None,None],
        ]

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

        ##############################################    
        kl, kt, box, tri, interf, s1, s2, s3 = symbols('kl kt box tri interf s1 s2 s3')
        
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
        s = Matrix([
            [s1] ,
            [s2] ,
            [s3]
        ])    
        ####    
        Minv   = self.M.inv()
        self.coeffs = c.transpose() * Minv # coeffs * s is the sigma, accessing per component gives each sample scaling
        self.sigma  = self.coeffs*s

####################

class HHModel(PhysicsModel):
    """ Models the HH production as linear sum of 6 components (VBF) and 3 components (GGF) """
    
    def __init__(self, ggf_sample_list, vbf_sample_list, name):
        PhysicsModel.__init__(self)

        self.name            = name

        self.check_validity_ggf(ggf_sample_list)
        self.check_validity_vbf(vbf_sample_list)

        self.ggf_formula = GGFHHFormula(ggf_sample_list)
        self.vbf_formula = VBFHHFormula(vbf_sample_list)

        self.dump_inputs()

    def check_validity_ggf(self, ggf_sample_list):
        if len(ggf_sample_list) != 3:
            raise RuntimeError("%s : malformed GGF input sample list - expect 3 samples" % self.name)
        if not isinstance(ggf_sample_list, list) and not isinstance(ggf_sample_list, tuple):
            raise RuntimeError("%s : malformed GGF input sample list - expect list or tuple" % self.name)
        for s in ggf_sample_list:
            if not isinstance(s, GGFHHSample):
                raise RuntimeError("%s : malformed GGF input sample list - each element must be a GGFHHSample" % self.name)

    def check_validity_vbf(self, vbf_sample_list):
        if len(vbf_sample_list) != 6:
            raise RuntimeError("%s : malformed VBF input sample list - expect 6 samples" % self.name)
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
        
        self.modelBuilder.doVar("r[1,0,10]")
        self.modelBuilder.doVar("r_gghh[1,0,10]")
        self.modelBuilder.doVar("r_qqhh[1,0,10]")
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

        self.create_scalings()

    def create_scalings(self):
        """ create the functions that scale the six components of vbf and the 3 components of ggf """

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
            f_name = 'f_ggfhhscale_sample_{0}'.format(i)
            f_expr = self.ggf_formula.coeffs[i] # the function that multiplies each sample

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

            arglist = ','.join(couplings_in_expr)
            exprname = 'expr::{}(\"{}\" , {})'.format(f_name, s_expr, arglist)
            # print exprname
            self.modelBuilder.factory_(exprname) # the function that scales each VBF sample

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
            f_name = 'f_vbfhhscale_sample_{0}'.format(i)
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

    def getYieldScale(self,bin,process):
        
        ## my control to verify for a unique association between process <-> scaling function
        try:
            self.scalingMap
        except AttributeError:
            self.scalingMap = {}

        if not self.DC.isSignal[process]: return 1
        # match the process name in the datacard to the input sample of the calculation
        # this is the only point where the two things must be matched

        if not process in self.scalingMap:
            self.scalingMap[process] = []

        imatched_ggf = []
        imatched_vbf = []

        for isample, sample in enumerate(self.ggf_formula.sample_list):
            if process.startswith(sample.label):
                # print self.name, ": {:>40}  ===> {:>40}".format(process, sample.label)
                imatched_ggf.append(isample)

        for isample, sample in enumerate(self.vbf_formula.sample_list):
            if process.startswith(sample.label):
                # print self.name, ": {:>40}  ===> {:>40}".format(process, sample.label)
                imatched_vbf.append(isample)

        ## this checks that a process finds a unique scaling
        if len(imatched_ggf) + len(imatched_vbf) != 1:
            print "[ERROR] : in HH model named", self.name, "there are", len(imatched_ggf), "GGF name matches and", len(imatched_vbf), "VBF name matches"
            raise RuntimeError('HHModel : could not uniquely match the process %s to the expected sample list' % process)

        if len(imatched_ggf) == 1:
            isample = imatched_ggf[0]
            self.scalingMap[process].append((isample, 'GGF'))
            return self.f_r_ggf_names[isample]
        
        if len(imatched_vbf) == 1:
            isample = imatched_vbf[0]
            self.scalingMap[process].append((isample, 'VBF'))
            return self.f_r_vbf_names[isample]

        raise RuntimeError('HHModel : fatal error in getYieldScale - this should never happen')

    def done(self):
        ## this checks that a scaling has been attached to a unique process
        scalings = {}
        for k, i in self.scalingMap.items(): ## key -> process, item -> [(isample, 'type')]
            samples = list(set(i)) # remove duplicates
            for s in samples:
                if not s in scalings:
                    scalings[s] = []
                scalings[s].append(k)

        for key, val in scalings.items():
            if len(val) > 1:
                print "[ERROR] : in HH model named", self.name, "there is a double assignment of a scaling : ", key, " ==> ", val
                raise RuntimeError('HHModel : coudl not uniquely match the scaling to the process')

        ## now check that, if a VBF/GGF scaling exists, there are actually 6/3 samples in the card
        n_VBF = 0
        n_GGF = 0
        for k, i in self.scalingMap.items():
            # the step above ensured me that the list contains a single element -> i[0]
            if i[0][1] == "GGF":
                n_GGF += 1
            elif i[0][1] == "VBF":
                n_VBF += 1
            else:
                raise RuntimeError("HHModel : unrecognised type %s - should never happen" % i[0][1])

        if n_GGF > 0 and n_GGF != 3:
            raise RuntimeError("HHModel : you did not pass all the 3 samples needed to build the GGF HH model")
        
        if n_VBF > 0 and n_VBF != 6:
            raise RuntimeError("HHModel : you did not pass all the 6 samples needed to build the VBF HH model")

########################################################
# definition of the model inputs

## NOTE : the scaling functions are not sensitive to global scalings of the xs of each sample
## so by convention use val_xs of the samples *without* the decay BR

## NOTE 2 : the xs *must* correspond to the generator one

# VBF : val_CV, val_C2V, val_kl
VBF_sample_list = [
    VBFHHSample(1,1,1,   val_xs = 0.00054/(0.3364), label = 'qqHH_CV_1_C2V_1_kl_1'  ),
    VBFHHSample(1,2,1,   val_xs = 0.00472/(0.3364), label = 'qqHH_CV_1_C2V_2_kl_1'  ),
    VBFHHSample(1,1,2,   val_xs = 0.00044/(0.3364), label = 'qqHH_CV_1_C2V_1_kl_2'  ),
    VBFHHSample(1,1,0,   val_xs = 0.00145/(0.3364), label = 'qqHH_CV_1_C2V_1_kl_0'  ),
    VBFHHSample(0.5,1,1, val_xs = 0.00353/(0.3364), label = 'qqHH_CV_0p5_C2V_1_kl_1'),
    VBFHHSample(1.5,1,1, val_xs = 0.02149/(0.3364), label = 'qqHH_CV_1p5_C2V_1_kl_1'),
]

# VBF : val_kl, val_kt
GGF_sample_list = [
    GGFHHSample(1,1,      val_xs = 0.02675, label = 'ggHH_kl_1_kt_1'  ),
    GGFHHSample(0,1,      val_xs = 0.06007, label = 'ggHH_kl_0_kt_1'  ),
    GGFHHSample(5,1,      val_xs = 0.07903, label = 'ggHH_kl_5_kt_1'  ),
    #GGFHHSample(2.45,1,   val_xs = 0.01133, label = 'ggHH_kl_2p45_kt_1'  ),
]

HHdefault = HHModel(
    ggf_sample_list = GGF_sample_list,
    vbf_sample_list = VBF_sample_list,
    name            = 'HHdefault'
)

# g = GGFHHFormula(GGF_sample_list)
# print g.sigma
# print g.sigma.evalf(subs = {
#     kl : 2.45,
#     kt : 1.0,
#     s1 : s.sample_list[0].val_xs,
#     s2 : s.sample_list[1].val_xs,
#     s3 : s.sample_list[2].val_xs,
# })
