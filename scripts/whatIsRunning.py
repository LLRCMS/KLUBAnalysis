import os, re
rootdir = '../Samples2016_29Giu_4Preapp/'

# list all jobs available
allJobs = {}
for root, dirs, files in os.walk(rootdir):
    if root == rootdir : continue
    jobs = []
    for f in files:
        m = re.search('skimJob_(.+)\.sh', f)
        if m:
            jobs.append(m.group(1))
        # print jobs
    allJobs[root] = jobs

# search for done_XXX
allnotdone = {}
for root, dirs, files in os.walk(rootdir):
    if root == rootdir : continue
    notdone = []
    for idx in allJobs[root]:
        if not 'done_'+idx in files:
            notdone.append(idx)

    allnotdone[root] = notdone

# print allnotdone

print 'STILL RUNNING'
for skim in allnotdone:
    if allnotdone[skim]:
        print skim , allnotdone[skim]