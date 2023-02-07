# coding: utf-8                                                                                                                             
_all_ = [ ]

import sys
import os
import argparse
import ROOT
import ConfigReader

def run(comm, dry_run):
    if dry_run:
        print(comm)
    else:
        os.system(comm)    

def run_limits(in_tags, channels, selections, masses, period, tag, var, signal, limit_dir, dry_run):
    # Generate datacards
    comm = 'bash make_res_cards.sh -d {dp} --channels {chn} --tag {tag} --in_tags {it}'.format(dp=period, chn=' '.join(channels), tag=tag, it=' '.join(in_tags))
    #run(comm, dry_run)

    # Generate workspaces
    comm = 'bash make_workspace_res.sh --tag {t} --masses {m} --var {v} --signal {s} --selections {sel} --channels {chn}'.format(t=tag, m=' '.join(masses), v=var, s=signal, sel=' '.join(selections), chn=' '.join(channels))
    #run(comm, dry_run)

    comm = 'bash combine_res_categories.sh --tag {t} --masses {m} --var {v} --signal {s}  --channels {chn}'.format(t=tag, m=' '.join(masses), v=var, s=signal, chn=' '.join(channels))
    run(comm, dry_run)

if __name__ == "__main__":
    usage = ('usage: %prog [options] datasetList\n %prog -h for help')
    parser = argparse.ArgumentParser(description=usage)
    parser.add_argument('-n', '--dry_run', action='store_true', help='dry run mode')
    FLAGS = parser.parse_args()

    period = 'UL18'
    tag = '10Feb_{}'.format(period)
    var = 'DNNoutSM_kl_1'
    signal = 'ggFRadion'
    limit_dir = '/home/llr/cms/${USER}/CMSSW_11_1_9/src/KLUBAnalysis/resonantLimits/'
    
    #channels = ('ETau', 'MuTau', 'TauTau')
    channels = ('ETau',)
    in_tags = ('10Feb_ETau_UL18', '10Feb_MuTau_UL18', '10Feb_TauTau_UL18')
    selections = ('s1b1jresolvedInvMcut', 's2b0jresolvedInvMcut', 'sboostedLLInvMcut')
    #masses = ('250', '260', '280', '300', '320', '350', '400', '450', '500', '550', '600', '650', '700', '750', '800', '850', '900', '1000', '1250', '1500', '1750', '2000', '2500', '3000')
    masses = ('1000',)

    run_limits(in_tags, channels, selections, masses, period, tag, var, signal, limit_dir, FLAGS.dry_run)
