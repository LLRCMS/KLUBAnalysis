#!/usr/bin/env python
import os, sys
import pandas as pd

fileNames = pd.read_csv('datasetNames.txt', sep = " ", header = None, comment='#')

fileNames.columns = ["name", "filelist"]

for i, row in fileNames.iterrows():
    print row["name"], row["filelist"]

    #filepath = "../../inputFiles/Files_June2018/"+row["filelist"]
    filepath = "../../inputFiles/Files_January2019/finals/"+row["filelist"]
    name  = row["name"]
    #command = "root -l -b \"ratioPU.C(0, 50000000, \\\""+filepath+"\\\", \\\""+name+"\\\")\" -q ; root -l \"makeRatio.C(\\\""+name+"\\\")\" > outputs_June2018/"+name+".txt -q"
    command = "root -l -b \"ratioPU.C(0, 50000000, \\\""+filepath+"\\\", \\\""+name+"\\\")\" -q ; root -l \"makeRatio.C(\\\""+name+"\\\")\" > outputs_January2019/"+name+".txt -q"
    
    print command

    os.system(command)
    #os.system("rm "+name+".root")

    #with  open("outputs_June2018/"+name+".txt", 'r') as fin:
    with  open("outputs_January2019/"+name+".txt", 'r') as fin:
        data = fin.read().splitlines(True)
        for k in range(len(data)):
            weight= []
            if k < 3: continue;
            weight= data[k].split("=")
            data[k] = weight[1].replace(";","")
            data[k] = data[k].replace(" ","")


    #with open("outputs_June2018/"+name+".txt", 'w') as fout:
    with open("outputs_January2019/"+name+".txt", 'w') as fout:
        fout.writelines(data[3:])
