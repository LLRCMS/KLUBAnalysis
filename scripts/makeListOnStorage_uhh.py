# coding: utf-8

import os
import sys
import re
import glob
import fnmatch
from subprocess import Popen, PIPE


thisdir = os.path.dirname(os.path.abspath(__file__))

dcache_mount = "/pnfs/desy.de/cms/tier2/store/user/{user}/hbt_resonant_run2/HHNtuples"

sample_kinds = {
    "background": {"user": "tokramer", "campaign_cre": re.compile(r"^RunII[^\/]+$")},
    "signal": {"user": "mrieger", "campaign_cre": re.compile(r"^RunII[^\/]+$")},
    "data": {"user": "mrieger", "campaign_cre": re.compile(r"^Run201[^\/]+$")},
}


def print_err(*msg):
    sys.stderr.write(" ".join(map(str, msg)) + "\n")
    sys.stderr.flush()


def get_sample_info(sample_name, sample_kind, tag, campaign_pattern=None):
    user = sample_kinds[sample_kind]["user"]
    sample_dir = os.path.join(dcache_mount.format(user=user), tag, sample_name)

    # get campaign names (the original ones, plus extensions)
    campaign_names = []
    for elem in os.listdir(sample_dir):
        if sample_kinds[sample_kind]["campaign_cre"].match(elem):
            campaign_names.append(elem)
    if not campaign_names:
        raise Exception("could not determine campaign_names for sample '{}'".format(sample_name))

    # apply an additional campaign name filtering
    if campaign_pattern:
        campaign_names = [c for c in campaign_names if fnmatch.fnmatch(c, campaign_pattern)]
        if not campaign_names:
            raise Exception("no campaign found after applying pattern '{}'".format(campaign_pattern))

    if sample_kind == "data":
        # simply sort
        campaign_names.sort()
    else:
        # sort such that the leading one is not a dataset extension (unless there are only extensions)
        campaign_names.sort(key=lambda c: bool(re.match(r"^(.+)_ext\d+-v.+$", c)))

    return sample_dir, campaign_names


def get_sample_files(sample_name, sample_kind, tag, campaign_pattern=None, dry=False):
    sample_dir, campaign_names = get_sample_info(sample_name, sample_kind, tag, campaign_pattern=campaign_pattern)

    # complain when more than one campaign is found
    if len(campaign_names) > 1:
        msg = "found more than one campaign for sample {}: {}".format(sample_name, campaign_names)
        if sample_kind == "data":
            raise Exception(msg)
        print_err(msg)

    # get timestamps
    timestamps = {}
    for campaign_name in campaign_names:
        campaign_dir = os.path.join(sample_dir, campaign_name)

        # get the correct timestamp
        _timestamps = [ts for ts in os.listdir(campaign_dir) if re.match(r"^\d+_\d+$", ts)]
        if not _timestamps:
            raise Exception("found no timestamp in campaign directory {}".format(campaign_dir))
        if len(_timestamps) > 1:
            print_err("found more than one timestamp in campaign directory {}, using latest one".format(campaign_dir))

        # store it
        timestamps[campaign_name] = sorted(_timestamps)[-1]

    # create a list of file names
    sample_files = []
    xrd_door = ""  # "root://dcache-cms-xrootd.desy.de:1094/"
    for campaign_name in campaign_names:
        ts_dir = os.path.join(sample_dir, campaign_name, timestamps[campaign_name])
        for file_name in glob.glob(os.path.join(ts_dir, "00??", "*.root")):
            sample_files.append((xrd_door or "") + file_name)

    return sample_files


def print_sample_files(*args, **kwargs):
    print("\n".join(get_sample_files(*args, **kwargs)))


def write_sample_files(sample_name, sample_kind, tag, campaign_pattern=None, output_dir=None, dry=False):
    """
    For legacy support.
    """
    # default output dir
    if output_dir is None:
        output_dir = os.path.normpath(os.path.join(thisdir, "..", "inputFiles", "UL17_" + sample_kind))

    # define the output file
    campaign_names = get_sample_info(sample_name, sample_kind, tag, campaign_pattern=campaign_pattern)[1]
    output_file = os.path.join(output_dir, "{}__{}.txt".format(sample_name, campaign_names[0]))

    if not dry:
        # get sample files
        sample_files = get_sample_files(sample_name, sample_kind, tag, campaign_pattern=campaign_pattern)

        # write them
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)
        with open(output_file, "w") as f:
            f.write("\n".join(sample_files) + "\n")

        print_err("found {} files for sample {}, campaigns {}, written to {}".format(
            len(sample_files), sample_name, campaign_names, output_file))

    # finally, print the output file
    print(output_file)


if __name__ == "__main__":
    from argparse import ArgumentParser

    parser = ArgumentParser(description="prints input file lists for small ntuple production")
    parser.add_argument("--sample", help="the sample to list files for")
    parser.add_argument("--kind", choices=list(sample_kinds), help="the sample kind")
    parser.add_argument("--campaign", default=None, help="an additional pattern for filtering campaigns, mandatory for data")
    parser.add_argument("--tag", default="uhh_2017_v4", help="the big ntuple production tag")
    parser.add_argument("--write", action="store_true", help="write the files names to file")
    parser.add_argument("--dry", action="store_true", help="dry run")
    args = parser.parse_args()

    # get the function to call and prepare kwargs
    func = print_sample_files
    kwargs = {
        "sample_name": args.sample, "sample_kind": args.kind, "campaign_pattern": args.campaign,
        "tag": args.tag, "dry": args.dry,
    }
    if args.write:
        func = write_sample_files

    # call it
    func(**kwargs)