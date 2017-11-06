chs = ['MuTau', 'ETau', 'TauTau']

parts = [(0, 499), (500, 999), (1000, 1499), (1500, 1999), (2000, 2499), (2500, 2672)]

for idx, part in enumerate(parts):
    siglist = ['bidimrew' + str(i) for i in range(part[0], part[1]+1)]
    strSigs = ', '.join(siglist)
    # print strSigs
    for ch in chs:
        fin = open('config/mainCfg_%s_templateParallel.cfg' % ch)
        fout = open('config/mainCfg_%s_part%i.cfg' % (ch, idx), 'w')
        for line in fin:
            if 'XXX_SIGNALS_XXX' in line:
                line = line.replace('XXX_SIGNALS_XXX', strSigs)
            if 'XXX_OUTPUT_XXX' in line:
                line = line.replace('XXX_OUTPUT_XXX', 'analysis_%s_7Mar_lims_2dscan_part%i' % (ch, idx))
            fout.write(line)
        fin.close()
        fout.close()
