import csv

wp_dict = { "L" : "0", "M" : "1", "T" : "2", "shape" : "3" }
jf_dict = { "5" : "0", "4" : "1", "0" : "2" }

with open("deepJet_106XUL16preVFP_v2.csv") as csv_ul:
    with open("deepJet_106XUL16preVFP_v2.legacyFormat.csv",'w') as csv_out:
        writer = csv.writer(csv_out, delimiter=',')
        for line in csv_ul:
            if "OperatingPoint" in line:
                csv_out.write(line)
            else:
                args = line.split(',')
                print("{}, {}".format(args[0], args[3]))
                args[0] = wp_dict[args[0]]
                args[3] = jf_dict[args[3]]
                csv_out.write(','.join(args))

