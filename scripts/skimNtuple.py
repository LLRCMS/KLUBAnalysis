#!/usr/bin/env python

import os,sys
import optparse
import fileinput
import commands
import time
import glob
import subprocess
from os.path import basename


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


if __name__ == "__main__":

    usage = 'usage: %prog [options]'
    parser = optparse.OptionParser(usage)
    parser.add_option ('-i', '--input'  , dest='input'  , help='input folder'                     , default='none')
    parser.add_option ('-x', '--xs'     , dest='xs'     , help='sample xs'                        , default='1.')
    parser.add_option ('-f', '--force'  , dest='force'  , help='replace existing reduced ntuples' , default=False)
    parser.add_option ('-o', '--output' , dest='output' , help='output folder'                    , default='none')
    parser.add_option ('-q', '--queue'  , dest='queue'  , help='batch queue'                      , default='cms')
    parser.add_option ('-r', '--resub'  , dest='resub'  , help='resubmit failed jobs'             , default='none')
    parser.add_option ('-v', '--verb'   , dest='verb'   , help='verbose'                          , default=False)
    parser.add_option ('-s', '--sleep'  , dest='sleep'  , help='sleep in submission'              , default=False)
    parser.add_option ('-d', '--isdata' , dest='isdata' , help='data flag'                        , default=False)
    parser.add_option ('-H', '--hadd'   , dest='hadd'   , help='hadd the resulting ntuples'       , default='none')
    parser.add_option ('-c', '--config' , dest='config' , help='skim config file'                 , default='none')
    (opt, args) = parser.parse_args()

    currFolder = os.getcwd ()

    # verify the result of the process
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

    if (opt.hadd != 'none') :

        scriptFile = open (opt.output + '/hadder.sh', 'w')
        scriptFile.write ('#!/bin/bash\n')
        scriptFile.write ('source /cvmfs/cms.cern.ch/cmsset_default.sh\n')
        scriptFile.write ('cd /data_CMS/cms/govoni/CMSSW_7_4_5/src\n')
        scriptFile.write ('export SCRAM_ARCH=slc6_amd64_gcc472\n')
        scriptFile.write ('eval `scram r -sh`\n')
        scriptFile.write ('cd %s\n'%currFolder)
        scriptFile.write ('source scripts/setup.sh\n')
        scriptFile.write ('mkdir ' + opt.output + '/singleFiles\n')
        scriptFile.write ('mv ' + opt.output + '/* ' + opt.output + '/singleFiles\n')
        scriptFile.write ('hadd ' + opt.output + '/total.root ' + opt.output + '/singleFiles/*.root\n')
        scriptFile.write ('touch ' + opt.output + '/done\n')
        scriptFile.write ('echo "Hadding finished" \n')
        scriptFile.close ()
        os.system ('chmod u+rwx ' + opt.output + '/hadder.sh')
        command = ('/opt/exp_soft/cms/t3/t3submit -q cms \'' +  opt.output + '/hadder.sh\'')
        os.system (command)
        sys.exit (0)

    # verify the result of the process
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

    if (opt.resub != 'none') :
        if (opt.input == 'none') :
            print 'input folder to be checked missing\n'
            print '(this is the folder that contains the jobs to be submitted)'
            sys.exit (1)

        if opt.input[-1] == '/' : opt.input = opt.input[:-1]
        opt.input = 'SKIM_' + basename (opt.input)
        jobs = [word.replace ('_', '.').split ('.')[1] for word in os.listdir (opt.input) if 'skim' in word]
        missing = []
        
        # check the existence of the done file
        for num in jobs :
            if not os.path.exists (opt.input + '/done_' + num) :
                if opt.verb : print num, ' : missing done file'
                missing.append (num)

        # check the log file
        for num in jobs :
            # get the log file name
            filename = opt.input + '/skimJob_' + num + '.sh'
#            print os.path.exists (filename) 
            with open (filename, 'r') as myfile :
                data = [word for word in myfile.readlines () if 'log' in word]
            rootfile = data[0].split ()[2]
            if not os.path.exists (rootfile) :
                if opt.verb : print num, 'missing root file', rootfile
                missing.append (num)
                continue
            logfile = data[0].split ()[-1]
            if not os.path.exists (logfile) :
                if opt.verb : print num, 'missing log file'
                missing.append (num)
                continue
            with open (logfile, 'r') as logfile :
                problems = [word for word in logfile.readlines () if 'Error' in word and 'TCling' not in word]
                if len (problems) != 0 :
                    if opt.verb : print num, 'found error ', problems[0]
                    missing.append (num)
        print 'the following jobs did not end successfully:'
        print missing   
        for num in missing :
            command = '`cat ' + opt.input + '/submit.sh | grep skimJob_' + num + '.sh | tr "\'" " "`'
            if opt.verb : print command
        if (opt.resub == 'run') :
            for num in missing :
                command = '`cat ' + opt.input + '/submit.sh | grep skimJob_' + num + '.sh | tr "\'" " "`'
                time.sleep (int (num) % 5)
                os.system (command)
        sys.exit (0)

    # submit the jobs
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

#    skimmer = './bin/skimNtuple.exe'
    skimmer = 'skimNtupleInclusive.exe'

    if opt.config == 'none' :
        print 'config file missing, exiting'
        sys.exit (1)

    if opt.input[-1] == '/' : opt.input = opt.input[:-1]
    if opt.output == 'none' : opt.output = opt.input + '_SKIM'
   
    if not os.path.exists (opt.input) :
        print 'input folder', opt.input, 'not existing, exiting'
        sys.exit (1)
    if not opt.force and os.path.exists (opt.output) :
        print 'output folder', opt.output, 'existing, exiting'
        sys.exit (1)
    elif os.path.exists (opt.output) :
        os.system ('rm -rf ' + opt.output + '/*')
    os.system ('mkdir ' + opt.output)
    
    inputfiles = glob.glob (opt.input + '/output*.root')    
    jobsDir = currFolder + '/SKIM_' + basename (opt.input)
    if os.path.exists (jobsDir) : os.system ('rm -f ' + jobsDir + '/*')
    else                        : os.system ('mkdir ' + jobsDir)

    proc = subprocess.Popen ('voms-proxy-info', stdout=subprocess.PIPE)
    tmp = [word for word in proc.stdout.read ().split ('\n') if 'timeleft' in word]
    if len (tmp) == 0 or int (tmp[0].split (':')[1]) < 24 : # hours
        os.system ('source /opt/exp_soft/cms/t3/t3setup')

    n = int (0)
    commandFile = open (jobsDir + '/submit.sh', 'w')
    for filename in inputfiles : 
        #create a wrapper for standalone cmssw job
        scriptFile = open ('%s/skimJob_%d.sh'% (jobsDir,n), 'w')
        scriptFile.write ('#!/bin/bash\n')
        scriptFile.write ('export X509_USER_PROXY=~/.t3/proxy.cert\n')
        scriptFile.write ('source /cvmfs/cms.cern.ch/cmsset_default.sh\n')
        scriptFile.write ('cd /data_CMS/cms/govoni/CMSSW_7_4_5/src\n')
        scriptFile.write ('export SCRAM_ARCH=slc6_amd64_gcc472\n')
        scriptFile.write ('eval `scram r -sh`\n')
        scriptFile.write ('cd %s\n'%currFolder)
        scriptFile.write ('source scripts/setup.sh\n')
        command = skimmer + ' ' + filename + ' ' + opt.output + '/' + basename (filename) + ' ' + opt.xs 
        if opt.isdata :  command += ' 1 '
        else          :  command += ' 0 '    
        command += ' ' + opt.config + ' '
        command += ' >& ' + opt.output + '/' + basename (filename) + '.log\n'
        scriptFile.write (command)
        scriptFile.write ('touch ' + jobsDir + '/done_%d\n'%n)
        scriptFile.write ('echo "All done for job %d" \n'%n)
        scriptFile.close ()
        os.system ('chmod u+rwx %s/skimJob_%d.sh'% (jobsDir,n))

        command = ('/opt/exp_soft/cms/t3/t3submit -q cms \'' + jobsDir + '/skimJob_' + str (n) + '.sh\'')
        if opt.sleep : time.sleep (0.1)
        os.system (command)
        commandFile.write (command + '\n')
        n = n + 1
    commandFile.close ()
    


