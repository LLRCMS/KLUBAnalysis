# coding: utf-8                                                                                                                             
_all_ = [ ]

import argparse
import subprocess
import ROOT
import ConfigReader

def run(comm, dry_run):
    print('=== [{}] Running the `{}` script ==='.format(__file__, comm.split(' ')[1]))
    if dry_run:
        print(comm)
    else:
        command = comm.split(' ')
        command = [x for x in command if x != '']
        subprocess.run(command, shell=False)
    return None

def run_limits(in_tags, channels, selections, masses, period, tag, var, signal, limit_dir, dry_run):
    commands = []
    ws_opt = dict(it=' '.join(in_tags), user='bfontana',
                  dp=period, tag=tag, v=var, s=signal,
                  chn=' '.join(channels), sel=' '.join(selections),
                  m=' '.join(masses))
    
    # Generate datacards
    commands.append('bash make_res_cards.sh -d {dp} --channels {chn} --tag {tag} --in_tags {it}')

    # Generate workspaces
    commands.append('bash make_workspace_res.sh --tag {tag} --masses {m} --var {v} --signal {s} --selections {sel} --channels {chn}')

    # Combine all categories
    commands.append('bash combine_res_categories.sh --tag {tag} --masses {m} --var {v} --signal {s}  --channels {chn}')

    # Combine all channels
    commands.append('bash combine_res_channels.sh --tag {tag} --masses {m} --var {v} --signal {s} --selections {sel}')

    # Combine categories and channels
    commands.append('bash combine_res_all.sh --tag {tag} --masses {m} --var {v} --signal {s}')

    for mode in ('separate', 'sel_group', 'chn_group', 'all_group'):
        # Obtain limits on signal strength
        commands.append('bash get_limits_res.sh --mode ' + mode + ' --tag {tag} --masses {m} --var {v} --signal {s} --selections {sel} --channels {chn}')

        # Plot limits
        commands.append('python plot.py --mode ' + mode + ' --period {dp} --tag {tag} --masses {m} --var {v} --signal {s} --selections {sel} --channels {chn} --user {user}')

    # Run all commands
    for comm in commands:
        run(comm.format(**ws_opt), dry_run)

if __name__ == "__main__":
    usage = 'Run the resonant limits'
    parser = argparse.ArgumentParser(description=usage)
    parser.add_argument('-n', '--dry_run', action='store_true', help='dry run mode')
    FLAGS = parser.parse_args()

    period = 'UL18'
    var = 'DNNoutSM_kl_1'
    signal = 'ggFRadion'
    limit_dir = '/home/llr/cms/${USER}/CMSSW_11_1_9/src/KLUBAnalysis/resonantLimits/'
    
    channels = ('ETau', 'MuTau', 'TauTau')
    tag_ = '16FebNight'
    in_tags = ['{}_{}_UL18'.format(tag_, x) for x in channels]
    out_tag = '{}_{}'.format(tag_, period)
    selections = ('s1b1jresolvedMcut', 's2b0jresolvedMcut', 'sboostedLLMcut')
    #masses = ('250', '260', '280', '300', '320', '350', '400', '450', '500', '550', '600', '650', '700', '750', '800', '850', '900', '1000', '1250', '1500', '1750', '2000', '2500', '3000')
    masses = ('250', '260', '280', '300', '350', '400', '450', '500', '550', '600', '650', '700', '750', '800', '850', '900', '1000', '1250', '1500', '1750', '2000', '2500', '3000')

    run_limits(in_tags, channels, selections, masses, period, out_tag, var, signal, limit_dir, FLAGS.dry_run)
