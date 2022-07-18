#!/usr/bin/env python

import os
import sys
import argparse
import time
import subprocess
import ROOT

def create_dir(d):
    if not os.path.exists(d):
        os.makedirs(d)
create_dir(outdir)

def is_good_file (fileName) :
    ff = ROOT.TFile (fname)
    if ff.IsZombie() : return False
    if ff.TestBit(ROOT.TFile.kRecovered) : return False
    return True

def parse_input_file_list(fileName) :
    filelist = []
    with open (fileName) as fIn:
        for line in fIn:
            line = (line.split("#")[0]).strip()
            if line:
                filelist.append(line)
    return filelist

write = lambda stream, text: stream.write(text + '\n')

def skim_ntuple(FLAGS, curr_folder):
    # verify the result of the process
    if (FLAGS.hadd != 'none') :    
        with open (FLAGS.output + '/hadder.sh', 'w') as s:
            write(s, '#!/bin/bash')
            write(s, 'source /cvmfs/cms.cern.ch/cmsset_default.sh\n')
            write(s, 'cd /home/llr/cms/motta/HHLegacy/CMSSW_11_1_0pre6/src')
            write(s, 'eval `scram r -sh`')
            write(s, 'cd {}\n'.format(curr_folder))
            write(s, 'source scripts/setup.sh')
            write(s, 'mkdir ' + os.path.join(FLAGS.output, 'singleFiles'))
            write(s, 'mv ' + os.path.join(FLAGS.output, + '*') +
                  ' ' + os.path.join(FLAGS.output, 'singleFiles'))
            write(s, 'hadd ' + os.path.join(FLAGS.output, 'total.root') +
                  ' ' + os.path.join(FLAGS.output, '/singleFiles/*.root'))
            write(s, 'touch ' + os.path.join(FLAGS.output, 'done'))
            write(s, 'echo "Hadding finished"')

        os.system('chmod u+rwx ' + FLAGS.output + '/hadder.sh')
        command = ('/opt/exp_soft/cms/t3/t3submit -q cms \'' + os.path.join(FLAGS.output, '/hadder.sh\''))
        os.system(command)
        sys.exit(0)

    # verify the result of the process
    if (FLAGS.resub != 'none'):
        if (FLAGS.input == 'none'):
            print('Input folder to be checked missing')
            print('(this is the folder that contains the jobs to be submitted)')
            sys.exit(1)

        if FLAGS.input[-1] == '/' : FLAGS.input = FLAGS.input[:-1]
        tagname = FLAGS.tag + "/" if FLAGS.tag else ''
        FLAGS.input = tagname + 'SKIM_' + os.path.basename (FLAGS.input)
        jobs = [word.replace ('_', '.').split ('.')[1] for word in os.listdir (FLAGS.input) if 'skim' in word]
        missing = []

        # check the existence of the done file
        for num in jobs :
            if not os.path.exists(FLAGS.input + '/done_' + num) :
                if FLAGS.verb:
                    print(num, ' : missing done file')
                missing.append(num)

        # check the log file
        for num in jobs :
            # get the log file name
            filename = FLAGS.input + '/skimJob_' + num + '.sh'
#            print os.path.exists (filename)
            with open (filename, 'r') as myfile:
                data = [word for word in myfile.readlines () if 'log' in word]
            rootfile = data[0].split ()[2]
            if not os.path.exists(rootfile):
                if FLAGS.verb:
                    print(num, 'missing root file', rootfile)
                missing.append (num)
                continue
            if not is_goodF_file(rootfile):
                if FLAGS.verb:
                    print(num, 'root file corrupted', rootfile)
                missing.append(num)
                continue
            logfile = data[0].split()[-1]
            if not os.path.exists(logfile) :
                if FLAGS.verb:
                    print(num, 'missing log file')
                missing.append (num)
                continue
            with open (logfile, 'r') as logfile :
                problems = [word for word in logfile.readlines () if 'Error' in word and 'TCling' not in word]
                if len(problems) != 0:
                    if FLAGS.verb:
                        print(num, 'found error ', problems[0])
                    missing.append (num)
        print('The following jobs did not end successfully:')
        print(missing)
        for num in missing :
            command = '`cat ' + FLAGS.input + '/submit.sh | grep skimJob_' + num + '.sh | tr "\'" " "`'
            if FLAGS.verb:
                print(command)
        if FLAGS.resub == 'run':
            for num in missing:
                command = '`cat ' + FLAGS.input + '/submit.sh | grep skimJob_' + num + '.sh | tr "\'" " "`'
                time.sleep(int (num) % 5)
                os.system(command)
        sys.exit(0)

    # submit the jobs
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

    skimmer = 'skimNtuple2018_HHbtag.exe'
    if FLAGS.year == '2017':
        skimmer = 'skimNtuple2017_HHbtag.exe'
    elif FLAGS.year == '2016':
        skimmer = 'skimNtuple2016_HHbtag.exe'


    if FLAGS.config == 'none':
        print('Config file missing, exiting')
        sys.exit(1)

    if FLAGS.input[-1] == '/':
        FLAGS.input = FLAGS.input[:-1]
    if FLAGS.output == 'none':
        FLAGS.output = FLAGS.input + '_SKIM'

    if not os.path.exists(FLAGS.input) :
        print('Input folder', FLAGS.input, 'not existing, exiting')
        sys.exit(1)
    if not FLAGS.force and os.path.exists(FLAGS.output):
        print('Output folder', FLAGS.output, 'existing, exiting')
        sys.exit(1)
    elif os.path.exists(FLAGS.output):
        os.system('rm -rf ' + FLAGS.output + '/*')
    os.system('mkdir ' + FLAGS.output)
    os.system('cp ' + FLAGS.config + ' ' + FLAGS.output)

    inputfiles = parse_input_file_list(FLAGS.input)
    njobs = len(inputfiles) if njobs > len(inputfiles) else FLAGS.njobs
    nfiles = (len (inputfiles) + len (inputfiles) % njobs) / njobs
    inputlists = [inputfiles[x:x+nfiles] for x in xrange (0, len (inputfiles), nfiles)]

    tagname = "/" + FLAGS.tag if FLAGS.tag else ''
    jobsDir = curr_folder + tagname + '/SKIM_' + os.path.basename (FLAGS.input)
    jobsDir = jobsDir.rstrip ('.txt')
    if float(FLAGS.klreweight) > -990 and FLAGS.BSMname == 'none':
        print '!WARNING! You requested manual HH reweighting, but did not set a proper BSMname! Exiting!'
        sys.exit (0)
    elif FLAGS.EFTrew != 'none':
        jobsDir = jobsDir + '_' + FLAGS.EFTrew
    elif FLAGS.BSMname != 'none':
        jobsDir = jobsDir + '_' + FLAGS.BSMname

    if os.path.exists (jobsDir):
        os.system ('rm -f ' + jobsDir + '/*')
    else:
        os.system ('mkdir -p ' + jobsDir)
 
    with open(jobsDir + "/" + listFileName, 'w') as thisf:
        for line in listname:
            write(thisf, line)

    job_name = os.path.join(jobsDir, 'skimJob.sh')
    with open(job_name, 'w') as s:
        write(s, '#!/bin/bash\n')
        write(s, 'export X509_USER_PROXY=~/.t3/proxy.cert\n')
        write(s, 'source /cvmfs/cms.cern.ch/cmsset_default.sh\n')
        write(s, 'eval `scram r -sh`\n')
        write(s, 'cd {}\n'.format(curr_folder))
        write(s, 'source scripts/setup.sh\n')
        command = skimmer + ' ${1} ${2}' + ' ' + FLAGS.xs
        if FLAGS.isdata:
            command += ' 1 '
        else:
            command += ' 0 '
        command += ' ' + FLAGS.config + ' '
        if FLAGS.dokinfit == "True":
            command += " 1 "
        else:
            command += " 0 "
        command += " " + FLAGS.xsscale
        command += " " + FLAGS.htcut
        command += " " + FLAGS.htcutlow
        if FLAGS.toprew=="True":
            command += " 1 "
        else:
            command += " 0 "
        if FLAGS.genjets=="True":
            command += " 1 "
        else:
            command += " 0 "
        command += " " + FLAGS.topstitch
        if FLAGS.domt2:
            command += " 1 " ## inspiegabilmente questo e' un bool
        else:
            command += " 0 "
        if FLAGS.ishhsignal:
            command += " 1 "
        else:
            command += " 0 "
        command += (" " + FLAGS.njets)
        command += (" " + FLAGS.EFTrew + " " + FLAGS.order + " " + FLAGS.uncert + " " + FLAGS.cms_fake + " " + FLAGS.klreweight + " " + FLAGS.ktreweight + " " + FLAGS.c2reweight + " " + FLAGS.cgreweight + " " + FLAGS.c2greweight)
        command += (" " + FLAGS.susyModel)
        command += (" " + FLAGS.PUweights)
        command += (" " + FLAGS.DY_nJets)
        command += (" " + FLAGS.DY_nBJets)
        if FLAGS.DY:
            command += " 1 "
        else:
            command += " 0 "
        if FLAGS.ttHToNonBB:
            command += " 1 "
        else:
            command += " 0 "
        if FLAGS.hhNLO:
            command += " 1 "
        else:
            command += " 0 "
        if FLAGS.year=='2016':
            if FLAGS.isAPV:
                command += " 1 "
            else:
                command += " 0 "
        command += ' >& ' + FLAGS.output + '/' + "output_" + str(n) + '.log\n'
        write(s, command)
        write('touch ' + jobsDir + '/done_%{}\n'%.format(n))
      
        if FLAGS.doSyst:
            sys_command = "skimOutputter.exe"
            sys_command += (" " + FLAGS.output + "/output_"+str(n)+".root")
            sys_command += (" " + FLAGS.output + "/syst_output_"+str(n)+".root")
            sys_command += (" " + FLAGS.config)
            if FLAGS.isdata:
                sys_command += ' 1 '
            else:
                sys_command += ' 0 '
            sys_command += (" " + ">& " + FLAGS.output + "/syst_output_"+str(n)+".log\n")
            write(s, sys_command)
        
        write(s, 'echo "All done for job %d" \n'%n)
        os.system('chmod u+rwx {}'.format(job_name))

    condor_name = job_name.replace('.sh','.condor')
    condor_var = 'infile'
    condlog = os.path.join(jobsDir, 'outputs')
    create_dir(condlog)
    with open(scriptpath_condor, 'w') as s:
        write(s,'Universe = vanilla')
        write(s,'Executable = {}'.format(job_name))
        write(s,'input = /dev/null')
        write(s,'output = {}/condor_log_$(Process).o'.format(condlog))
        write(s,'error  = {}/condor_log_$(Process).e'.format(condlog))
        write(s,'getenv = true')
        write(s,'T3Queue = short')
        write(s,'WNTag=el7')
        write(s,'+SingularityCmd = ""')
        write(s,'include : /opt/exp_soft/cms/t3_tst/t3queue |')
        write(s,'Arguments = $({}) '.format(condor_var))
        write(s,'queue {} from ('.format(condor_var))
        for n in range(njobs): 
            line = os.path.join(jobsDir, 'filelist_{}.txt'.format(str(n)))
            line += ', '
            line += os.path.join(FLAGS.output, 'output_{}.root'.format(str(n)))
        write(s, line)
        write(s, ')')

    launch_command = 'condor_submit {}'.format(condor_name)
    with open (jobsDir + '/submit.sh', 'w') as comf:
        comf.write(command + '\n')

    if FLAGS.sleep:
        time.sleep (0.1)
    os.system(launch_command)

if __name__ == "__main__":

    usage = 'Command line parser of skimming a bit Ntuple.'
    parser = argparse.ArgumentParser(description=usage)
    parser.add_argument('-i', '--input', dest='input', default='none', help='input folder')
    parser.add_argument('-Y', '--year', dest='year', default='2018', help='year')
    parser.add_argument('-A', '--APV', dest='isAPV', default=False, help='isAPV')
    parser.add_argument('-x', '--xs', dest='xs', help='sample xs', default='1.')
    parser.add_argument('-f', '--force', dest='force', default=False, help='replace existing reduced ntuples')
    parser.add_argument('-o', '--output', dest='output', default='none', help='output folder')
    parser.add_argument('-q', '--queue', dest='queue', default='short', help='batch queue')
    parser.add_argument('-r', '--resub', dest='resub', default='none', help='resubmit failed jobs')
    parser.add_argument('-v', '--verb', dest='verb', default=False, help='verbose')
    parser.add_argument('-s', '--sleep', dest='sleep', default=False, help='sleep in submission')
    parser.add_argument('-d', '--isdata', dest='isdata', default=False, help='data flag')
    parser.add_argument('-T', '--tag', dest='tag', default='', help='folder tag name')
    parser.add_argument('-H', '--hadd', dest='hadd', default='none', help='hadd the resulting ntuples')
    parser.add_argument('-c', '--config', dest='config', default='none', help='skim config file')
    parser.add_argument('-n', '--njobs', dest='njobs', default=100, type=int, help='number of skim jobs')
    parser.add_argument('-k', '--kinfit', dest='dokinfit', default='True', help='run HH kin fitter')
    parser.add_argument('-m', '--mt2', dest='domt2', default=True, help='run stransverse mass calculation')
    parser.add_argument('-y', '--xsscale', dest='xsscale', default='1.0',
                        help='scale to apply on XS for stitching')
    parser.add_argument('-Z', '--htcutlow', dest='htcutlow', default='-999.0',
                        help='HT low cut for stitching on inclusive')
    parser.add_argument('-z', '--htcut', dest='htcut', default='-999.0',
                        help='HT cut for stitching on inclusive')
    parser.add_argument('-e', '--njets', dest='njets', default='-999',
                        help='njets required for stitching on inclusive')
    parser.add_argument('-t', '--toprew', dest='toprew', default=False,
                        help='is TT bar sample to compute reweight?')
    parser.add_argument('-b', '--topstitch' , dest='topstitch' , default='0',
                        help='type of TT gen level decay pruning for stitch')
    parser.add_argument('-g', '--genjets', dest='genjets', default=False,
                        help='loop on genjets to determine the number of b hadrons')
    parser.add_argument('-a', '--ishhsignal', dest='ishhsignal', default=False, help='isHHsignal')
    parser.add_argument('--BSMname', dest='BSMname', default='none', help='additional name for EFT benchmarks')
    parser.add_argument('--EFTbm', dest='EFTrew', default='none',
                        help='EFT benchmarks [SM, 1..12, 1b..7b, 8a, c2scan, manual]')
    parser.add_argument('--order', dest='order', default='nlo', help='order of reweight: lo/nlo')
    parser.add_argument('--uncert', dest='uncert', default='0', help='uncertainty on the reweight coeffs')
    parser.add_argument('--cms_fake', dest='cms_fake', default='0', help='invert some couplings for 2017/2018')
    parser.add_argument('--kl', dest='klreweight', default='-999.0', help='invert some couplings for 2017/2018')
    parser.add_argument('--kt', dest='ktreweight', default='-999.0', help='kt for dynamic reweight')
    parser.add_argument('--c2', dest='c2reweight', default='-999.0', help='c2 for dynamic reweight')
    parser.add_argument('--cg', dest='cgreweight', default='-999.0', help='cg for dynamic reweight')
    parser.add_argument('--c2g', dest='c2greweight', default='-999.0', help='c2g for dynamic reweight')
    parser.add_argument('--susy', dest='susyModel', default='NOTSUSY', help='name of susy model to select')
    parser.add_argument('--pu', dest='PUweights', default='none', help='name of susy model to select')
    parser.add_argument('--nj', dest='DY_nJets', default='-1', help='number of gen Jets for DY bins')
    parser.add_argument('--nb', dest='DY_nBJets', default='-1', help='number of gen BJets for DY bins')
    parser.add_argument('--DY', dest='DY', default=False, help='if it is a DY sample')
    parser.add_argument('--ttHToNonBB', dest='ttHToNonBB', default=False,
                        help='if it is a ttHToNonBB sample')
    parser.add_argument('--hhNLO', dest='hhNLO', default=False, action='store_true',
                        help='if it is an HH NLO sample')
    parser.add_argument('--doSyst', dest='doSyst', default=False, action='store_true',
                        help='compute up/down values of outputs')

    FLAGS = parser.parse_args()
    curr_folder = os.getcwd()
    skim_ntuple(FLAGS, curr_folder)
