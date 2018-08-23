import os

names = [
'1j0b',
'1j1b',
'2j0b',
'2j1b',
'2j2b',
'3j0b',
'3j1b',
'3j2b',
'3j3b',
'4j0b',
'4j1b',
'4j2b',
'4j3b',
'4j4b'
]


for name in names:
    #os.system('cp DY_0j0b.txt DY_'+name+'.txt')
    os.system('rm DY_'+name)