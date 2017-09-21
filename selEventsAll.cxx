#include <iostream>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
using namespace std;


inline std::string mid_num_str(const std::string& s) {
  std::string::size_type in_pos = s.size() - 11; 
  std::string::size_type p  = s.find('_',in_pos);  
  std::string::size_type pp = s.find('\n', p + 2); 
  return s.substr(p + 1, pp - p - 1);
}

void selEventsAll(){
  gROOT->ProcessLine(".L selEvents.cc+");
  ifstream filelist;
  string line;
  string flag = "output_";
  //   filelist.open("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_10Gen/SKIM_TT_semiLep/goodfiles.txt", ios::in);
  //   filelist.open("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_10Gen/SKIM_TT_fullyLep/goodfiles.txt", ios::in);
  //DATA
  filelist.open("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_5Mar/SKIM_SingleMuon/goodfiles.txt", ios::in);
  //filelist.open("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_5Mar/SKIM_SingleElectron/goodfiles.txt", ios::in);
  //filelist.open("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_5Mar/SKIM_SingleTau/goodfiles.txt", ios::in);
  int num = 0;
  string num_string;
  //    int i  = 0;
  if (filelist.is_open())
    {
      //	cout<<"open"<<endl;
      while (std::getline(filelist,line))
	{
	  //i++;
	  if(line.size()>2){
	    //	    cout<<" line "<< i<<endl;
	    num_string = mid_num_str(line);
	    //cout<<num_string<<endl;
	    num = atoi(num_string.c_str()); 
	    //cout<<num<<endl;
	    
	    gROOT->ProcessLine(Form("selEvents(%d)",num));
	  }
	}  
      filelist.close();
    }
  
}

