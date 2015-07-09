// enum for gen particle saved flags (used in global flag word creation)

class GenFlags {
    public:
        enum genFlags {
            // flags telling from where a particle comes
            fromH = 0,
            fromTop = 1,
            fromTau = 2,
            fromZ = 3,
            
            // not used, see interface/GenHelper.h only 
            /*
            // for H/Z bosons only, tell how they decayed
            HZToMuHad  = 4,
            HZToEHad   = 5,
            HZToHadHad = 6,
            HZToMuMu   = 7,
            HZToEE     = 8,
            HZToEMu    = 9,
            HZToEEPrompt = 10, // prompt Z->ee/mumu decays
            HZToMuMuPrompt = 11,
            HZToOther  = 12 // for e.g. h->bb
            */
        };
};