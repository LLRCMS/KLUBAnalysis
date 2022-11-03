import os
import sys
import argparse


write = lambda stream, text: stream.write(text + '\n')


def make_parser():
    parser = argparse.ArgumentParser(description='Command line parser of plotting options')
    parser.add_argument('--outdir', type=str, dest="outdir", required=True,
                        help='/afs/directory/to/submit/from. Must be afs\
    because condor submission is only allowed from /afs on lxplus.')
    parser.add_argument('--cfg', dest='cfg', required=True,
                        help='name of the main cfg to run', default=None)
    parser.add_argument('--tag', dest='tag', required=True, default=None,
                        help='name of working space')
    parser.add_argument('--njobs', dest='njobs', required=False, type=int,
                        help='number of jobs for parallelization', default=10)
    return parser


def create_dir(d):
    if not os.path.exists(d):
        print("{} does not exist. Creating now".format(d))
        os.makedirs(d)


def write_filler_executable(scriptpath, exe, cfg, njobs):
    with open(scriptpath, 'w') as s:
        write(s,'#!/bin/bash')
        write(s,'export X509_USER_PROXY=~/Condor/x509up_u149480')
        write(s,'source /cvmfs/cms.cern.ch/cmsset_default.sh')
        write(s,'cd {} || exit 1'.format(os.getcwd()))
        write(s,'export SCRAM_ARCH=slc6_amd64_gcc491')
        write(s,'eval `scram r -sh`')
        write(s,'source scripts/setup.sh')
        command = (exe + ' ' + cfg + ' ${1}' + ' ' + str(njobs))
        write(s,command)
        os.system('chmod u+rwx {}'.format(scriptpath))
    

def write_submit_file(submit_file, executable, out_dir, err_dir):
    with open(submit_file, 'w') as s:
        write(s,'Universe = vanilla')
        write(s,'Executable = {}'.format(executable))
        write(s,'input = /dev/null')
        write(s,'output = {}/$(ClusterId)$(ProcId).out'.format(out_dir))
        write(s,'error  = {}/$(ClusterId)$(ProcId).err'.format(err_dir))
        write(s,'getenv = true')
        write(s,'+JobFlavour = \"longlunch\"')
        write(s,'+SingularityCmd = ""')
        write(s,'Arguments = $(NAME) ')
        write(s,'queue') 


def write_postscript(postscript, tag,):
    with open(postscript, 'w') as ps:
        write(ps,'#!/bin/bash')
        write(ps,'export X509_USER_PROXY=~/Condor/x509up_u149480')
        write(ps,'source /cvmfs/cms.cern.ch/cmsset_default.sh')
        write(ps,'cd {} || exit 1'.format(os.getcwd()))
        write(ps,'export SCRAM_ARCH=slc6_amd64_gcc491')
        write(ps,'eval `scram r -sh`')
        write(ps,'source scripts/setup.sh')
        write(ps,'cd {} || exit 1'.format(tag))
        write(ps,'hadd outPlotter.root *.root')
        write(ps,'cd - || exit 1')
        write(ps,'python scripts/combineFillerOutputs.py --dir {}'.format(tag))
        os.system('chmod u+rwx ' + postscript)


def write_postscript_submit(submit_file, executable, out_dir, err_dir):
    with open(submit_file, 'w') as s:
        write(s,'Universe = vanilla')
        write(s,'Executable = {}'.format(executable))
        write(s,'input = /dev/null')
        write(s,'output = {}/post.out'.format(out_dir))
        write(s,'error  = {}/post.err'.format(err_dir))
        write(s,'getenv = true')
        write(s,'queue') 

def write_dagfile(dagfile, njobs, submit_file, postscript_submit):
    with open(dagfile, 'w') as dag:
        for job in range(njobs):
            write(dag, 'JOB {} {}'.format(job, submit_file))
            write(dag, 'VARS {} NAME=\"{}\"'.format(job, job))
        write(dag, 'JOB Cleanup {}'.format(postscript_submit))
        children = ' '.join([str(i) for i in range(njobs)])
        write(dag, 'PARENT {} CHILD Cleanup'.format(children))

#hourdate = datetime.datetime.now().strftime('%Y.%m.%d_%H.%M.%S').replace('.','-')

def main():
    parser = make_parser()
    args = parser.parse_args()
    outdir = args.outdir
    exe = 'testAnalysisHelper.exe'
    condor_executable = 'filler.sh'

    # check if outdir exists
    if os.path.exists(outdir):
        print("{} already exists. Checking if it's empty.".format(outdir))
        contents = os.listdir(outdir)
        if len(contents) > 0:
            print("{} contains the following :".format(outdir))
            for i in contents:
                print(i)
            print("Please remove these or use a different tag!")
            sys.exit(1)
        
    create_dir(outdir)
    condor_out = os.path.join(outdir, 'out')
    create_dir(condor_out)
    condor_err = os.path.join(outdir, 'err')
    create_dir(condor_err)

    scriptpath = os.path.join(outdir, condor_executable)
    write_filler_executable(scriptpath=scriptpath,
                            exe=exe,
                            cfg=args.cfg,
                            njobs=args.njobs)
    submit_file_path = os.path.join(outdir, 'filler.submit')
    write_submit_file(submit_file=submit_file_path,
                      executable=scriptpath,
                      out_dir=condor_out,
                      err_dir=condor_err)
    postscript_path = os.path.join(outdir, 'postscript.sh')
    write_postscript(postscript=postscript_path, tag=args.tag)
    postscript_submit_path = os.path.join(outdir, 'postscript.submit')
    write_postscript_submit(submit_file=postscript_submit_path,
                            executable=postscript_path,
                            out_dir=condor_out,
                            err_dir=condor_err)
    dagfile = os.path.join(outdir, 'filler.dag')
    write_dagfile(dagfile=dagfile,
                  njobs=args.njobs,
                  submit_file=submit_file_path,
                  postscript_submit=postscript_submit_path)


if __name__ == "__main__":
    main()
    #os.chdir(condor_dir)
    #launch_command = 'condor_submit {}'.format(scriptpath_condor)
    #print('The following command was run: {}'.format(launch_command))
    #os.system(launch_command)
