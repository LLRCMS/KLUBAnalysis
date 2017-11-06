/*
    ** class: CfgParser
    ** author: L. Cadamuro (LLR)
    ** date: 26/05/2016
    ** description: parser of txt files for analysis config
*/

#ifndef CFGPARSER_H
#define CFGPARSER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class CfgParser
{
    typedef std::unordered_map<std::string, std::string> optionBlock;
    typedef std::unordered_map<std::string, optionBlock> cfgBlock;
    
    public:
        CfgParser();
        CfgParser(std::string filename);
        ~CfgParser();
        bool init(std::string filename);
        bool extend(CfgParser& cfg);
    
        void setListSetSymb (std::string symb){optListSepSymb_ = symb;}

        // void setStrictLevel (int strict){strict_ = strict;}

        // getting options -- both as section, key separated or section::key
        std::string readStringOpt(std::string section, std::string option);
        std::string readStringOpt(std::string compact);

        int         readIntOpt(std::string section, std::string option);
        int         readIntOpt(std::string compact);

        bool        readBoolOpt(std::string section, std::string option);
        bool        readBoolOpt(std::string compact);

        float       readFloatOpt(std::string section, std::string option);
        float       readFloatOpt(std::string compact);

        std::vector<std::string> readStringListOpt(std::string section, std::string option);
        std::vector<std::string> readStringListOpt(std::string compact);

        std::vector<int>         readIntListOpt(std::string section, std::string option);
        std::vector<int>         readIntListOpt(std::string compact);

        std::vector<bool>        readBoolListOpt(std::string section, std::string option);
        std::vector<bool>        readBoolListOpt(std::string compact);

        std::vector<float>       readFloatListOpt(std::string section, std::string option);
        std::vector<float>       readFloatListOpt(std::string compact);

        bool hasOpt (std::string section, std::string option);
        bool hasOpt (std::string compact);

        bool hasSect (std::string section);
        std::vector<std::string> readListOfOpts(std::string section);

        const cfgBlock& getCfg(){return config_;}
        std::string getCfgName(){return filename_;}

    private:
        std::string filename_;
        std::unordered_map<std::string, optionBlock> config_;
        std::string lSecBlockSymb_;
        std::string rSecBlockSymb_;  // define a new section block
        std::string optAssignSymb_;  // used to assign value to option 
        std::string optListSepSymb_; // separate a list option
        std::string commentSymb_;    // introduces a comment

        // to be implemented, now only error messages
        // int strict_; // severity when errors are raised. More strict behavior as value is higher
        // enum StrLevel {
        //     kSilent        = 0,
        //     kMsgOnly       = 1,
        //     kThrowAll      = 2
        // };

        void trimLine(std::string& line);
        std::string getTrimmedLine(const std::string& line);
        std::pair<std::string, std::string> splitOptionLine(std::string line);
        std::pair<std::string, std::string> splitCompact (std::string compact);
        std::vector<std::string> splitStringInList(std::string line);
        bool endsWith (std::string line, std::string suffix);
};

#endif