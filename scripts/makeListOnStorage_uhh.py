# coding: utf-8

import os
import re
import glob
from subprocess import Popen, PIPE

thisdir = os.path.dirname(os.path.abspath(__file__))


tag = "uhh_2017_v2"
kind = "backgrounds"
output_dir = os.path.normpath(os.path.join(thisdir, "..", "inputFiles", "UL17_" + kind))
xrd_door = "root://dcache-cms-xrootd.desy.de:1094/"

store_path = "/pnfs/desy.de/cms/tier2/store/user/{}/hbt_resonant_run2/HHNtuples".format({
    "backgrounds": "tokramer",
    "signals": "mrieger",
    "data": "mrieger",
}[kind])

sample_dir_filter = {
    "backgrounds": r"^.+\/RunII[^\/]+$",
    "signals": r"^.+\/RunII[^\/]+$",
    "data": r"^.+\/Run201[^\/]+$",
}[kind]

# find produced samples and store a map "output file name" -> ["absolute samples dirs"]
for sample_dir in glob.glob(os.path.join(store_path, tag, "*", "*")):
    if not os.path.isdir(sample_dir) or not re.match(sample_dir_filter, sample_dir):
        continue
    sample_name = os.path.basename(os.path.dirname(sample_dir))
    campaign_name = os.path.basename(sample_dir)

    # get all timestamps
    timestamps = [
        os.path.basename(ts_dir)
        for ts_dir in glob.glob(os.path.join(sample_dir, "*"))
        if os.path.isdir(ts_dir)
    ]
    assert timestamps
    if len(timestamps) > 1:
        print("found more than one timestamp in {}, choose the latest one".format(sample_dir))
        timestamp = sorted(timestamps)[-1]
    else:
        timestamp = timestamps[0]

    # get all sample dirs
    abs_sample_dirs = [
        abs_sample_dir
        for abs_sample_dir in glob.glob(os.path.join(sample_dir, timestamp, "*"))
        if os.path.isdir(abs_sample_dir)
    ]
    
    # create a list of file names
    file_names = []
    for abs_sample_dir in abs_sample_dirs:
        for file_name in glob.glob(os.path.join(abs_sample_dir, "*.root")):
            file_names.append((xrd_door or "") + file_name)

    # define the output file
    output_file = os.path.join(output_dir, "{}__{}.txt".format(sample_name, campaign_name))
    # extended datasets should be added to the original one when existing
    is_ext = re.match(r"_ext\d+-v.+$", campaign_name)
    if is_ext:
        print("\nfound dataset extension being saved in output file {}".format(output_file))
        print("please merge it with the original file when existting!\n")

    # write them
    if not os.path.exists(os.path.dirname(output_file)):
        os.makedirs(os.path.dirname(output_file))
    with open(output_file, "w") as f:
        f.write("\n".join(file_names) + "\n")
    print("found {} files for sample {}".format(sample_name, len(file_names)))
