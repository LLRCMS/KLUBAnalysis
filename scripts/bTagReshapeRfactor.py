import ROOT

year = '2018'

channels = ['TauTau', 'MuTau', 'ETau']
#channels = ['ETau']

systs = [
    ['central'     , 'totBkg_baseline_SR_DNNoutSM_kl_1'],
#    ['JESUp'       , 'totBkg_baseline_SR_DNNoutSM_kl_1_bTagweightReshapeJESUp'],
#    ['LFUp'        , 'totBkg_baseline_SR_DNNoutSM_kl_1_bTagweightReshapeLFUp'],
#    ['HFUp'        , 'totBkg_baseline_SR_DNNoutSM_kl_1_bTagweightReshapeHFUp'],
#    ['HFSTATS1Up'  , 'totBkg_baseline_SR_DNNoutSM_kl_1_bTagweightReshapeHFSTATS1Up'],
#    ['HFSTATS2Up'  , 'totBkg_baseline_SR_DNNoutSM_kl_1_bTagweightReshapeHFSTATS2Up'],
#    ['LFSTATS1Up'  , 'totBkg_baseline_SR_DNNoutSM_kl_1_bTagweightReshapeLFSTATS1Up'],
#    ['LFSTATS2Up'  , 'totBkg_baseline_SR_DNNoutSM_kl_1_bTagweightReshapeLFSTATS2Up'],
#    ['CFERR1Up'    , 'totBkg_baseline_SR_DNNoutSM_kl_1_bTagweightReshapeCFERR1Up'],
#    ['CFERR2Up'    , 'totBkg_baseline_SR_DNNoutSM_kl_1_bTagweightReshapeCFERR2Up'],
#    ['JESDown'     , 'totBkg_baseline_SR_DNNoutSM_kl_1_bTagweightReshapeJESDown'],
#    ['LFDown'      , 'totBkg_baseline_SR_DNNoutSM_kl_1_bTagweightReshapeLFDown'],
#    ['HFDown'      , 'totBkg_baseline_SR_DNNoutSM_kl_1_bTagweightReshapeHFDown'],
#    ['HFSTATS1Down', 'totBkg_baseline_SR_DNNoutSM_kl_1_bTagweightReshapeHFSTATS1Down'],
#    ['HFSTATS2Down', 'totBkg_baseline_SR_DNNoutSM_kl_1_bTagweightReshapeHFSTATS2Down'],
#    ['LFSTATS1Down', 'totBkg_baseline_SR_DNNoutSM_kl_1_bTagweightReshapeLFSTATS1Down'],
#    ['LFSTATS2Down', 'totBkg_baseline_SR_DNNoutSM_kl_1_bTagweightReshapeLFSTATS2Down'],
#    ['CFERR1Down'  , 'totBkg_baseline_SR_DNNoutSM_kl_1_bTagweightReshapeCFERR1Down'],
#    ['CFERR2Down'  , 'totBkg_baseline_SR_DNNoutSM_kl_1_bTagweightReshapeCFERR2Down'],
]


for channel in channels:

    print '----------------------------------------------------'
    print ' Doing channel', channel, 'for year', year

    # Infile
    infileName = '/gwpool/users/brivio/Hhh_1718/LegacyRun2/May2020/LIMdev/CMSSW_11_1_0_pre6/src/KLUBAnalysis/analysis_20Feb2021_rfactor/'+channel+'_'+year+'/outPlotter.root'
    infile = ROOT.TFile(infileName, "READ")

    # Before histo and integral
    h_b = infile.Get('totBkg_baselineNoBtag_SR_DNNoutSM_kl_1')
    y_b = h_b.Integral() * 1.0

    print ' %-15s : %10s / %-10s --> r = %-10s' % ('syst', 'before', 'after', 'Rfactor')
    for syst in systs:    
        # After
        h_a = infile.Get(syst[1])
        y_a = h_a.Integral() * 1.0

        r = y_b/y_a * 1.0

        print ' %-15s : %10.3f / %-10.3f --> r = %-10.4f' % (syst[0], y_b, y_a, r)

    print '----------------------------------------------------'


#    # CP5 samples
#    # Before histo and integral
#    h_b_CP5 = infile.Get('totBkgCP5_baselineNobtag_SR_DNNoutSM_kl_1')
#    y_b_CP5 = h_b_CP5.Integral() * 1.0
#
#    print '%-15s : %10s / %-10s --> r = %-10s' % ('syst', 'before', 'after', 'Rfactor')
#    print '----------------------------------------------------'
#    for syst in systs:    
#        # After
#        h_a_CP5 = infile.Get(syst[1].replace('totBkg_','totBkgCP5_'))
#        y_a_CP5 = h_a_CP5.Integral() * 1.0
#
#        r_CP5 = y_b_CP5/y_a_CP5 * 1.0
#
#        print '%-15s : %10.3f / %-10.3f --> r = %-10.4f' % (syst[0], y_b_CP5, y_a_CP5, r_CP5)
#
#    print '----------------------------------------------------'
