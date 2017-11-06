def parseFile(filename, CL='50.0', exp=True):
    f = open(filename)
    matches = []
    for line in f:
        s = ('Expected %s%%: r <'%CL)
        if not exp: s = 'Observed Limit: r <'
        
        if not s in line:
            continue
        val = line.replace(s, '')
        val = float(val)
        matches.append(val)
    
    if len(matches) == 0:
        print "did not find any expected in file: " , filename, 'CL=', CL, 'exp?=', exp
        return -1.0
    else:
        return matches[-1]

