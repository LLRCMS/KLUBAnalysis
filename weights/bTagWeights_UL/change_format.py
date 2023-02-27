import csv

wp_dict = { "L" : "0", "M" : "1", "T" : "2", "shape" : "3" }
jf_dict = { "5" : "0", "4" : "1", "0" : "2" }

periods = ['16preVFP','16postVFP','17','18']
algo = ['deepJet','deepCSV']

def translate(key, dictionary):
    """
    Translate from *key* to a value in a *dictionary*, allowing cases where *key* already refers to
    a valid value.
    """
    if key in dictionary:
        return dictionary[key]
    if key in dictionary.values():
        return key
    raise Exception("no entry {} in dictionary {}".format(key, dictionary))

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
                        args[0] = translate(args[0], wp_dict)
                        args[3] = translate(args[3], jf_dict)
                        csv_out.write(','.join(args))
