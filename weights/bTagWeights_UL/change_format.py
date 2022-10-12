import csv

wp_dict = { "L" : "0", "M" : "1", "T" : "2", "shape" : "3" }
jf_dict = { "5" : "0", "4" : "1", "0" : "2" }

periods = ['16preVFP','16postVFP','17','18']
algo = ['deepJet','deepCSV']

for alg in algo:
    for period in periods:
        input_file = 'weights/bTagWeights_UL/UL{period}/merged_{alg}.csv'.format(period=period,alg=alg)
        output_file = 'weights/bTagWeights_UL/UL{period}/merged_{alg}.legacyFormat.csv'.format(period=period,alg=alg)
        
        print('generating {}'.format(output_file))
        with open(input_file) as csv_ul:
            with open(output_file,'w') as csv_out:
                writer = csv.writer(csv_out, delimiter=',')
                for line in csv_ul:
                    if "OperatingPoint" in line:
                        csv_out.write(line)
                    else:
                        if "pow(x,1.5)" in line:
                            line=line.replace("pow(x,1.5)","x**1.5")
                            print(line)
                        args = line.split(',')
                        args[0] = wp_dict[args[0]]
                        args[3] = jf_dict[args[3]]
                        csv_out.write(','.join(args))

