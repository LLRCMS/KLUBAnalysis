# coding: utf-8                                                                                                                             
_all_ = [ ]

import os
import argparse
import subprocess
from blessings import Terminal
import ROOT
import ConfigReader

def run(comm, dryrun):
    if dryrun:
        term = Terminal()
        print(term.red('=== [{}] Running the `{}` script ==='.format(__file__, comm.split(' ')[1])))
        print(comm)
    else:
        print('=== [{}] Running the `{}` script ==='.format(__file__, comm.split(' ')[1]))
        command = comm.split(' ')
        command = [x for x in command if x != '']
        subprocess.run(command, shell=False)
    return None

def run_limits(in_tags, channels, selections, selection_prefixes, masses, 
               period, tag, var, spin, work_dir, config_files, noprep,
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

    signal = 'GGF_Radion' if spin == "0" else 'GGF_Graviton'

    if not combination:
        commands = [
            # Generate datacards
            'bash make_res_cards.sh -d {dp} --channels {chn} --tag {tag} --in_tags {it} --var {v} -b {b} --cfg {cfg} --selections {sel} --masses {m} --spin {spin} ',
            # Generate workspaces
            'bash make_workspace_res.sh --tag {tag} --masses {m} --var {v} --signal {s} --selections {sel} --channels {chn} -b {b}',
            # Combine all categories
            'bash combine_res_categories.sh --tag {tag} --masses {m} --var {v} --signal {s} --channels {chn} -b {b} --period {dp} ', #--selprefixes {selpref}
            # Combine all channels
            'bash combine_res_channels.sh --tag {tag} --masses {m} --var {v} --signal {s} --selprefixes {selpref} -b {b} --period {dp}',
            # Combine categories and channels
            'bash combine_res_all.sh --tag {tag} --masses {m} --var {v} --signal {s} -b {b}'
        ]
        if noprep:
            commands[0] += '--noprep '

    else:
        commands = [
            # Combine all categories
            'bash combine_res_categories_years.sh --masses {m} --var {v} --signal {s} --channels {chn} -b {b} --selprefixes {selpref}',
            # Combine all channels
            'bash combine_res_channels_years.sh --masses {m} --var {v} --signal {s} --selprefixes {selpref} -b {b}',
            # Combine categories and channels
            'bash combine_res_all_years.sh --masses {m} --var {v} --signal {s} -b {b}'
        ]

    # Obtain limits on signal strength
    if combination:
        #limit_modes = ('sel_years', 'chn_years', 'all_years',)
        limit_modes = ('all_years',)
    else:
        #limit_modes = ('separate', 'sel_group', 'chn_group', 'all_group')
        limit_modes = ('all_group',)
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
        #plot_modes = limit_modes + ('overlay_channels', 'overlay_selections')
        plot_modes = limit_modes
        
    for mode in plot_modes:
        commands.append('python plot.py --mode ' + mode + ' --period {dp} --masses {m} --var {v} --signal {s} --selections {selpref} --channels {chn} --user {user} --basedir {b} --atlas') #  --interpolate
        if not combination:
            commands[-1] += " --tag {tag}"

    # Run all commands
    ws_opt = dict(it=' '.join(in_tags), 
                  user=user,
                  dp=period, 
                  tag=tag, 
                  v=var, 
                  s=signal,
                  spin=spin,
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
    parser.add_argument('--year', default="UL18", choices=("UL16APV", "UL16", "UL17", "UL18"),
                        help='Data period.')
    parser.add_argument('--spin', required=True, choices=("0", "2"),
                        help='Spin hypothesis.')
    parser.add_argument('--tag', required=True, help='Tag.')
    
    FLAGS = parser.parse_args()
    
    varsfit = ('pdnn_m{1}_s' + FLAGS.spin + '_hh',) # HHKin_mass',
    
    channels = ('ETau', 'MuTau', 'TauTau')

    in_tags = ['{}_{}/{}'.format(FLAGS.tag, FLAGS.year, x) for x in channels]
    cfg_files = ['mainCfg_{}_{}.cfg'.format(x, FLAGS.year) for x in channels]

    out_tag = '{}_{}'.format(FLAGS.tag, FLAGS.year)
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
    
    # masses = ("250", "260", "270", "280", "300", "320", "350",
    #           "400", "450", "500", "550", "600", "650",
    #           "700", "750", "800", "850", "900", "1000",
    #           "1250", "1500", "1750", "2000", "2500", "3000")
    masses = ("250", "280", "300", "320", "350",
              "400", "450", "500", "550", "600", "650",
              "700", "750", "800", "850", "900", "1000",
              "1250", "1500", "1750", "2000", "2500", "3000")
    work_dir = os.path.join('/home/llr/cms/', os.environ['USER'], 'CMSSW_11_1_9/src/KLUBAnalysis')

    for var in varsfit:
        run_limits(in_tags, channels, selections, selection_prefixes, masses,
                   FLAGS.year, out_tag, var, FLAGS.spin, work_dir, cfg_files,
                   FLAGS.noprep, FLAGS.user, FLAGS.dryrun, FLAGS.combination)
