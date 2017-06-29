import json

filename = 'di-tau/real_taus_cumulative.json'

with open(filename) as file:
    data = json.load(file)

print 'Tight WP parameters'
print '\talpha ', data['TightIso']['alpha']
print '\tm_{0} ', data['TightIso']['m_{0}']
print '\tsigma ', data['TightIso']['sigma']
print '\tnorm  ', data['TightIso']['norm']
print '\tn     ', data['TightIso']['n']

