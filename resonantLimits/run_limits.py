# coding: utf-8                                                                                                                             
_all_ = [ ]

import argparse
import ROOT
import ConfigReader

import subprocess
from subprocess import Popen, PIPE

def run(comm, dry_run):
    if dry_run:
        print(comm)
    else:
        command = comm.split(' ')
        command = [x for x in command if x != '']
        subprocess.run(command, shell=False)

def run_limits(in_tags, channels, selections, masses, period, tag, var, signal, limit_dir, dry_run):
    # Generate datacards
    comm = 'bash make_res_cards.sh -d {dp} --channels {chn} --tag {tag} --in_tags {it}'.format(dp=period, chn=' '.join(channels), tag=tag, it=' '.join(in_tags))
    run(comm, dry_run)

    ws_opt = dict(dp=period, tag=tag, v=var, s=signal)
    # Generate workspaces
    comm = 'bash make_workspace_res.sh --tag {tag} --masses {m} --var {v} --signal {s} --selections {sel} --channels {chn}'.format(m=' '.join(masses), sel=' '.join(selections), chn=' '.join(channels), **ws_opt)
    #run(comm, dry_run)

    comm = 'bash combine_res_categories.sh --tag {tag} --masses {m} --var {v} --signal {s}  --channels {chn}'.format(t=tag, m=' '.join(masses), chn=' '.join(channels), **ws_opt)
    #run(comm, dry_run)

    comm = 'bash combine_res_channels.sh --tag {tag} --masses {m} --var {v} --signal {s} --selections {sel}'.format(m=' '.join(masses), sel=' '.join(selections), **ws_opt)
    #run(comm, dry_run)

    comm = 'bash combine_res_all.sh --tag {tag} --masses {m} --var {v} --signal {s}'.format(m=' '.join(masses), **ws_opt)
    #run(comm, dry_run)

    comm = 'bash combine_res_all.sh --tag {tag} --masses {m} --var {v} --signal {s}'.format(m=' '.join(masses), **ws_opt)
    #run(comm, dry_run)

    comm = 'bash get_limits_res.sh --tag {tag} --masses {m} --var {v} --signal {s} --selections {sel} --channels {chn}'.format(m=' '.join(masses), sel=' '.join(selections), chn=' '.join(channels), **ws_opt)
    #run(comm, dry_run)

    comm = 'python plotSimple_resMass.py --period {dp} --tag {tag} --masses {m} --var {v} --signal {s} --selections {sel} --channels {chn}'.format(m=' '.join(masses), sel=' '.join(selections), chn=' '.join(channels), **ws_opt)
    comm += ' --user bfontana'
    #run(comm, dry_run)

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
    
    channels = ('ETau', 'MuTau', 'TauTau')
    #channels = ('ETau',)
    in_tags = ['10Feb_' + x + '_UL18' for x in channels]
    selections = ('s1b1jresolvedInvMcut', 's2b0jresolvedInvMcut', 'sboostedLLInvMcut')
    #masses = ('250', '260', '280', '300', '320', '350', '400', '450', '500', '550', '600', '650', '700', '750', '800', '850', '900', '1000', '1250', '1500', '1750', '2000', '2500', '3000')
    masses = ('250', '260', '300', '320', '350', '400', '450', '500', '550', '600', '650', '700', '750', '800', '850', '900', '1000', '1250', '1500', '1750', '2000', '2500', '3000')

    run_limits(in_tags, channels, selections, masses, period, tag, var, signal, limit_dir, FLAGS.dry_run)
