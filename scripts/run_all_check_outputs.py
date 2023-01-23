import os
import glob
import argparse
import re

def get_jobs_list(condor_file, itresub):
    """Parses condor files outputting the number of jobs that were launched."""
    with open(condor_file) as f:
        for line in f:
            line = line.strip('\n')
            matches = re.match('queue\s(.+)$', line)
            if matches is not None:
                if itresub:
                    break
                else:
                    g = matches.groups()
                    assert len(g) == 1
                    return [x for x in range(int(g[0]))]
        jobs = []
        for line in f:
            line = line.strip('\n')
            if line == ')':
                return jobs
            n = int(line)
            jobs.append(n)
    raise RuntimeError('the provided condor file {} does not match the expected format.'.format(condor_file))

def run_all_check_outputs(args):
    base_dir = os.path.join('/data_CMS/cms/', args.user, 'HHresonant_SKIMS/')
    base_dir = os.path.join(base_dir, 'SKIMS_' + args.data_period + '_' + args.in_tag)

    sample_dirs = [x for x in glob.glob(os.path.join(base_dir, '*/'))]
    if len(sample_dirs) == 0:
        print('No folders were found under {}/.'.format(base_dir))

    for sd in sample_dirs:
        if args.itresub:
            condor_file = os.path.join(sd, 'job_resub' + str(args.itresub) + '.condor')
        else:
            condor_file = os.path.join(sd, 'job.condor')

        if not os.path.exists(condor_file):
            print('[WARNING] File {} was not found.'.format(condor_file))
            continue

        joblist = get_jobs_list(condor_file, args.itresub)
        print('Processing folder {} with {} jobs for resubmission iteration {}...'.format(sd, len(joblist), args.itresub))
        for job in joblist:
            if args.itresub:
                comm = 'python scripts/check_outputs.py -r {sd}/output_{n}.root -o {sd}/live_logs/{n}_resub' + str(args.itresub) + '.out -e {sd}/live_logs/{n}_resub' + str(args.itresub) + '.err -l {sd}/logs/{n}_resub' + str(args.itresub) + '.log -v'
            else:
                comm = 'python scripts/check_outputs.py -r {sd}/output_{n}.root -o {sd}/live_logs/{n}.out -e {sd}/live_logs/{n}.err -l {sd}/logs/{n}.log -v'
            comm = comm.format(sd=sd, n=job)
            if args.dry_run:
                print(comm)
            else:
               os.system(comm)


if __name__ == "__main__":
    usage = 'Produces all `goodfiles.txt` and `badfiles.txt` files when scripts/check_outputs.py fails but the skimmed files were nevertheless produced.'
    parser = argparse.ArgumentParser(description=usage)
    parser.add_argument('--data_period', default='UL18', choices=('UL16', 'UL17', 'UL18'), help='data period')
    parser.add_argument('--in_tag', required=True, help='tag used to generate skimmed files')
    parser.add_argument('--user', required=True, help='user that submitted the skimming step')
    parser.add_argument('--dry_run', action='store_true', help='prints debug info and does not launch any command')
    parser.add_argument('--resub_iteration', dest='itresub', default=0, required=True, type=int, 
                        help='resubmission iteration (the first submission corresponds to zero)')
    FLAGS = parser.parse_args()
    run_all_check_outputs(FLAGS)
