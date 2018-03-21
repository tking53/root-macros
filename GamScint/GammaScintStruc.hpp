#ifndef PAASS_GAMMASCINTSTRUC_HPP
#define PAASS_GAMMASCINTSTRUC_HPP
struct SINGLES {
    bool HasLowResBeta = false;
    double Energy = -999;
    int DetNum = -999;
    double Time = -999;
    double BetaGammaTDiff = -999;
    double BetaEnergy = -999;
    double BetaMulti = -999;
    double BetaTime = -999;
    double EvtNum = -999;
    double BunchNum = -999;
    double LastBunchTime = -999;


    int NumGroup = -1; // group numbers (0-3) for small hag, 0-2 for nai.
    // REQUIRES a Type condition to separate the types

    std::string Type = "";
    int NumType = -1; //order in decreasing mass, 0 = nai, 1 = big hag, 2= small hag
    std::string Group = "";
} ;


#endif