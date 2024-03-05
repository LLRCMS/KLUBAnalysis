# coding: utf-8                                                                                                                             
_all_ = [ ]

import os
import argparse
import subprocess
import ROOT
import ConfigReader

def run(comm, dryrun):
    print('=== [{}] Running the `{}` script ==='.format(__file__, comm.split(' ')[1]))
    if dryrun:
        print(comm)
    else:
        command = comm.split(' ')
        command = [x for x in command if x != '']
        subprocess.run(command, shell=False)
    return None

def run_limits(in_tags, channels, selections, selection_prefixes, masses,
               period, tag, var, signal, work_dir, config_files, noprep,
               user, dryrun, combination):
    """
    - selection_prefixes:
        Selection prefixes are used to group different categories sharing the same name prefixes. Used for mHH categories.
        Without mHH categories 'selections' and 'selection_prefixes' will be identical.
    - user:
        username in eos, used to store the files in the website provided by CERN
    """
    assert(len(channels) == len(config_files))
    
    commands = []

    if not combination:
        # Generate datacards
        commands.append('bash make_res_cards.sh -d {dp} --channels {chn} --tag {tag} --in_tags {it} --var {v} -b {b} --cfg {cfg} --selections {sel} --masses {m} --signal {s} -u ')
        if noprep:
            commands[-1] += '--noprep '

        # Generate workspaces
        commands.append('bash make_workspace_res.sh --tag {tag} --masses {m} --var {v} --signal {s} --selections {sel} --channels {chn} -b {b}')

        # Combine all categories
        commands.append('bash combine_res_categories.sh --tag {tag} --masses {m} --var {v} --signal {s} --channels {chn} -b {b} --period {dp} --selprefixes {selpref}')
        
        # Combine all channels
        commands.append('bash combine_res_channels.sh --tag {tag} --masses {m} --var {v} --signal {s} --selprefixes {selpref} -b {b} --period {dp}')

        # combine categories and channels
        commands.append('bash combine_res_all.sh --tag {tag} --masses {m} --var {v} --signal {s} -b {b}')

    else:
        # Combine all categories
        commands.append('bash combine_res_categories_years.sh --masses {m} --var {v} --signal {s} --channels {chn} -b {b} --selprefixes {selpref}')
        
        # Combine all channels
        commands.append('bash combine_res_channels_years.sh --masses {m} --var {v} --signal {s} --selprefixes {selpref} -b {b}')

        # combine categories and channels
        commands.append('bash combine_res_all_years.sh --masses {m} --var {v} --signal {s} -b {b}')

    # Obtain limits on signal strength
    if combination:
        limit_modes = ('sel_years', 'chn_years', 'all_years',)
    else:
        limit_modes = ('separate', 'sel_group', 'chn_group', 'all_group')
    #assert any('year' in x for x in limit_modes) and period == "All"
        
    for mode in limit_modes:
        commands.append('bash get_limits_res.sh --mode ' + mode + ' --masses {m} --var {v} --signal {s} --channels {chn} -b {b}')
        #if mode == 'sel_group' and set(selections) != set(selection_prefixes):
        commands[-1] += ' --selections {selpref}'
        if not combination:
            commands[-1] += ' --tag {tag}'

    # Plot limits
    if combination:
        plot_modes =  ('all_years', 'overlay_channels_years', 'overlay_selections_years')
        #plot_modes = ('sel_years', 'chn_years',)
    else:
        plot_modes = limit_modes + ('overlay_channels', 'overlay_selections')
        
    for mode in plot_modes:
        commands.append('python plot.py --mode ' + mode + ' --period {dp} --masses {m} --var {v} --signal {s} --selections {selpref} --channels {chn} --user {user} --basedir {b} --atlas')
        if not combination:
            commands[-1] += " --tag {tag}"

    # Run all commands
    ws_opt = dict(it=' '.join(in_tags), 
                  user=user,
                  dp=period, 
                  tag=tag, 
                  v=var, 
                  s=signal,
                  chn=' '.join(channels),
                  sel=' '.join(selections),
                  selpref=' '.join(selection_prefixes),
                  m=' '.join(masses),
                  b=work_dir,
                  cfg=' '.join(config_files))
    for comm in commands:
        run(comm.format(**ws_opt), dryrun)

if __name__ == '__main__':
    usage = 'Run the resonant limits'
    parser = argparse.ArgumentParser(description=usage)
    parser.add_argument('-u', '--user', default='bfontana', help='lxplus username')
    parser.add_argument('-n', '--dryrun', action='store_true', help='dry run mode')
    parser.add_argument('-p', '--noprep', action='store_true',
                        help='do not run the histograms preparing step')
    parser.add_argument('-c', '--combination', action='store_true',
                        help='run datacard combination across multiple data periods')
    FLAGS = parser.parse_args()

    period = 'UL18' #All
    signal = 'GGF_Radion'
    varsfit = ('DNNoutSM_kl_1',) # HHKin_mass',)
    
    channels = ('ETau', 'MuTau', 'TauTau')

    tag_ = 'OpenCadi_V1'
    suffix = ''
    #in_tags = ['{}_{}_{}{}'.format(tag_, x, period, suffix) for x in channels]
    in_tags = ['{}_{}{}'.format(tag_, x, suffix) for x in channels]
    #cfg_files = ['mainCfg_{}_{}{}.cfg'.format(x, period, suffix) for x in channels]
    cfg_files = ['mainCfg_{}_{}.cfg'.format(x, period) for x in channels]

    out_tag = '{}_{}{}'.format(tag_, period, suffix)
    selections = ('res1b', 'res2b', 'boostedL_pnet')
    selection_prefixes = ('res1b', 'res2b', 'boostedL_pnet')
    # selections = ('s1b1jresolvedMcutmHH250_335',
    #               's1b1jresolvedMcutmHH335_475',
    #               's1b1jresolvedMcutmHH475_725',
    #               's1b1jresolvedMcutmHH725_1100',
    #               's1b1jresolvedMcutmHH1100_3500',
    #               's2b0jresolvedMcutmHH250_335',
    #               's2b0jresolvedMcutmHH335_475',
    #               's2b0jresolvedMcutmHH475_725',
    #               's2b0jresolvedMcutmHH725_1100',
    #               's2b0jresolvedMcutmHH1100_3500',
    #               'sboostedLLMcutmHH250_625',
    #               'sboostedLLMcutmHH625_775',
    #               'sboostedLLMcutmHH775_1100',
    #               'sboostedLLMcutmHH1100_3500'
    #               )
    
    masses = ('250', '260', '280', '300', '320', '350', '400', '450', '500', '550', '600', '650', '700', '750', '800', '850', '900', '1000', '1250', '1500', '1750', '2000',)
    work_dir = os.path.join('/home/llr/cms/', os.environ['USER'], 'CMSSW_11_1_9/src/KLUBAnalysis')

    for var in varsfit:
        run_limits(in_tags, channels, selections, selection_prefixes, masses, period, out_tag, var, signal, work_dir, cfg_files, FLAGS.noprep, FLAGS.user, FLAGS.dryrun, FLAGS.combination)
