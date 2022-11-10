import os
import sys
import argparse


write = lambda stream, text: stream.write(text + '\n')


def make_parser():
    parser = argparse.ArgumentParser(description='Command line parser of plotting options')
    parser.add_argument('--submit_dir', type=str, dest="submit_dir", required=True,
                        help='/afs/directory/to/submit/from. Must be afs\
    because condor submission is only allowed from /afs on lxplus.')
    parser.add_argument('--cfg', dest='cfg', required=True,
                        help='name of the main cfg to run', default=None)
    parser.add_argument('--outdir', dest='outdir', required=True, default=None,
                        help='name of working space')
    parser.add_argument('--njobs', dest='njobs', required=False, type=int,
                        help='number of jobs for parallelization', default=20)
    parser.add_argument('--dry', '-d', dest='dry', action='store_true')
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


def write_postscript(postscript, outdir,):
    with open(postscript, 'w') as ps:
        write(ps,'#!/bin/bash')
        write(ps,'export X509_USER_PROXY=~/Condor/x509up_u149480')
        write(ps,'source /cvmfs/cms.cern.ch/cmsset_default.sh')
        write(ps,'cd {} || exit 1'.format(os.getcwd()))
        write(ps,'export SCRAM_ARCH=slc6_amd64_gcc491')
        write(ps,'eval `scram r -sh`')
        write(ps,'source scripts/setup.sh')
        write(ps,'cd {} || exit 1'.format(outdir))
        write(ps,'hadd outPlotter.root *.root')
        write(ps,'cd - || exit 1')
        write(ps,'python scripts/combineFillerOutputs.py --dir {}'.format(outdir))
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

def main(submit_dir,
         cfg,
         outdir,
         njobs,
         executable='testAnalysisHelper.exe',
         condor_executable='filler.sh',
         dry=False):
    # check if submit_dir exists
    if os.path.exists(submit_dir):
        print("{} already exists. Checking if it's empty.".format(submit_dir))
        contents = os.listdir(submit_dir)
        if len(contents) > 0:
            print("{} contains the following :".format(submit_dir))
            for i in contents:
                print(i)
            print("Please remove these or use a different outdir!")
            sys.exit(1)
        
    create_dir(submit_dir)
    condor_out = os.path.join(submit_dir, 'out')
    create_dir(condor_out)
    condor_err = os.path.join(submit_dir, 'err')
    create_dir(condor_err)

    scriptpath = os.path.join(submit_dir, condor_executable)
    write_filler_executable(scriptpath=scriptpath,
                            exe=executable,
                            cfg=cfg,
                            njobs=njobs)
    submit_file_path = os.path.join(submit_dir, 'filler.submit')
    write_submit_file(submit_file=submit_file_path,
                      executable=scriptpath,
                      out_dir=condor_out,
                      err_dir=condor_err)
    postscript_path = os.path.join(submit_dir, 'postscript.sh')
    write_postscript(postscript=postscript_path, outdir=outdir)
    postscript_submit_path = os.path.join(submit_dir, 'postscript.submit')
    write_postscript_submit(submit_file=postscript_submit_path,
                            executable=postscript_path,
                            out_dir=condor_out,
                            err_dir=condor_err)
    dagfile = os.path.join(submit_dir, 'filler.dag')
    write_dagfile(dagfile=dagfile,
                  njobs=njobs,
                  submit_file=submit_file_path,
                  postscript_submit=postscript_submit_path)
    if not dry:
        os.chdir(submit_dir)
        launch_command = 'condor_submit_dag {}'.format(dagfile)
        print('The following command was run: {}'.format(launch_command))
        os.system(launch_command)


if __name__ == "__main__":
    parser = make_parser()
    args = parser.parse_args()
    main(submit_dir=args.submit_dir,
         cfg=args.cfg,
         outdir=args.outdir,
         njobs=args.njobs,
         dry=args.dry)