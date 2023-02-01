from argparse import ArgumentParser
import os
import sys
from subprocess import Popen, PIPE

import modules.ConfigReader as cfgr

def make_parser():
    parser = ArgumentParser(
        description="Run the plotter for all variables.")
    parser.add_argument("-t", "--tag", type=str, 
    help="Tag used in main_cfg") 
    parser.add_argument("-c", "--channel", type=str, 
    help="TauTau or MuTau or ETau")
    parser.add_argument("-l", "--lumi", type=float, 
    help="Lumi. Should match the one from main cfg.")
    parser.add_argument("-r", "--region", type=str,
    help="for example: SR")
    parser.add_argument("-s", "--selection", type=str,
    default="baseline", help="Selection.")
    parser.add_argument("-o", "--others", type=str, 
    default="--quit --ratio --no-sig",
    help="Plotting Arguments provided at the end of the plotter call.")
    return parser


def main(tag, channel, lumi, region, selection, others):
    cfgName = tag + "/mainCfg_"+ channel+"_UL18.cfg"
    cfg = cfgr.ConfigReader (cfgName)
    variables = cfg.readListOption("general::variables")
    outdir = "./"+tag+"/"+selection+"_"+region
    if not os.path.exists("./"+tag):
        raise ValueError("{} does not exist. \
Make sure you pass the tag that was also used \
in submitHistoFiller.py --tag <tag>".format("./"+tag))
    if not os.path.exists(outdir):
        print("{} does not exist.".format(outdir))
        print("Creating it now.")
        os.makedirs(outdir)
    # defing what the daughter particles are
    daughter_dict = {"tautau": ('#tau_{h1}','#tau_{h2}'),
                     "mutau": ("#mu", "#tau_{h}"),
                     "etau": ("e", "#tau_{h}")}
    obj1, obj2 = daughter_dict[channel.lower()] 
    label_suffixes = {
        'pt': " p_{T} [GeV]",
        'mass': " M [GeV]",
        'eta': " #eta",
        'deltaR': " #DeltaR"
    }
    label_prefixes = {
        "dau1": obj1, 
        "dau2": obj2,
        "tauH": "H_{#tau#tau}",
        "bH": "H_{bb}",
        "ditau": "#tau#tau",
        "bjet1": "b_{1}",
        "bjet2": "b_{2}",
        "HH": "HH"
    } 
    for var in variables:
        cmd = "python scripts/makeFinalPlots_lxplus.py --dir "+tag
        cmd += " --var "+var+" --reg "+region+" --sel "+selection
        cmd += " --channel "+channel+" --lymin 0.7 --lumi "+str(lumi)
        cmd += " --tag "+tag
        try:
            label = label_prefixes[var.split('_')[0]]
            label += label_suffixes[var.split('_')[-1]]
        except KeyError:
            label = var
        cmd += " --label \"{}\"".format(label)
        cmd += " "+others
        cmd_log = cmd + " --log"
        print("Now running: {}".format(cmd_log))
        prcs = Popen(cmd_log, 
                    stderr=PIPE, stdout=PIPE, shell=True,)
        out, err = prcs.communicate()
        print("Now running: {}".format(cmd))
        prcs = Popen(cmd, 
                        stderr=PIPE, stdout=PIPE, shell=True,)
        out, err = prcs.communicate()
        #if err:
        #    print("There were the following errors:")
        #    print(err)
        #    print("Stdout:")
        #    print(out)
        #else:
        #    print(out)


if __name__ == "__main__":
    parser = make_parser()
    args = parser.parse_args()
    main(tag=args.tag,
         channel=args.channel,
         lumi=args.lumi,
         region=args.region,
         selection=args.selection,
         others=args.others)
        
        



#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var dau1_pt         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "${obj1} p_{T} [GeV]"  $others 
