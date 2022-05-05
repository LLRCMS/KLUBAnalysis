import os
import ROOT
import argparse

parser = argparse.ArgumentParser(description='Command line parser of splitting options')

parser.add_argument('--sample',    dest='sample',   help='', default=None)
parser.add_argument('--path',    dest='path',   help='', default=None)
args = parser.parse_args()

sample = args.sample
input_dir = args.path

# define real/fake separation
cut_real = '((pairType==2 && isTau1real==1 && isTau2real==1) || (pairType<2 && isTau2real==1) || pairType>2)'
cut_fake = '!' + cut_real

samples_dict = {}

samples_dict[sample] = {}
samples_dict[sample]["path"] = input_dir+'SKIM_'+sample+'/'
samples_dict[sample]["files"] = []
with open(samples_dict[sample]["path"]+'goodfiles.txt') as flist:
    for line in flist:
        line = (line.split("#")[0]).strip()
        if line:
            samples_dict[sample]["files"].append(line)

print(samples_dict)

print(sample)
input_path = samples_dict[sample]['path']
output_path_real = input_path.replace('220420','220420_realTaus')
output_path_fake = input_path.replace('220420','220420_fakeTaus')

# create directories if needed
if not os.path.isdir(output_path_real):
    os.makedirs(output_path_real)      
if not os.path.isdir(output_path_fake):
    os.makedirs(output_path_fake)   
        
# copy goodfiles.txt
with open(input_path+'goodfiles.txt') as file_in:
    with open(output_path_real+'goodfiles.txt','w') as out_real:
        with open(output_path_fake+'goodfiles.txt','w') as out_fake:
            for line in file_in:
                line = (line.split("#")[0]).strip()
                if line:
                    out_real.write(line.replace('220420','220420_realTaus'))
                    out_real.write('\n')
                    out_fake.write(line.replace('220420','220420_fakeTaus'))
                    out_fake.write('\n')
                    

                    
for file_in in samples_dict[sample]['files']:
    
    # read input file
    f_in = ROOT.TFile.Open(file_in)
    tree_in  = f_in.Get('HTauTauTree')
    h_eff_in = f_in.Get('h_eff')
    h_effSummary_in = f_in.Get('h_effSummary')
    h_syst_in = f_in.Get('h_syst')
    
    # create 'realTaus' outputs
    file_out_realTaus = file_in.replace('220420','220420_realTaus')        
    f_out_real = ROOT.TFile(file_out_realTaus,'recreate')
    f_out_real.cd()
    tree_real_out = tree_in.CopyTree(cut_real)
    
    # save 'realTaus' outputs
    tree_real_out.Write()
    h_eff_in.Write()
    h_effSummary_in.Write()
    h_syst_in.Write()
    f_out_real.Close()
    

    # create 'fakeTaus' outputs
    file_out_fakeTaus = file_in.replace('220420','220420_fakeTaus')
    f_out_fake = ROOT.TFile(file_out_fakeTaus,'recreate')
    f_out_fake.cd()
    tree_fake_out = tree_in.CopyTree(cut_fake)
    
    # save 'fakeTaus' outputs
    tree_fake_out.Write()
    h_eff_in.Write()
    h_effSummary_in.Write()
    h_syst_in.Write()
    f_out_fake.Close()
    
    f_in.Close()
    
        
