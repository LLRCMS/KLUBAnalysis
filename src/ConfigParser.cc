// File ConfigParser.C
//
/*! \class ConfigParser
 * \brief A class which keeps track of the entire configuration of the
 * analysis
 *
 * $Date: 2013/02/05 11:24:28 $
 * $Author: rgerosa $
 *
 * The class is initialized by calling the init() proceedure which
 * takes the command line arguments. Afterwards a configuration file
 * is opened and parsed. Subsequently, the user may ask for the values
 * of the the configuration options or whether options were provided
 * at all. The type of the values is left entirely to the user who
 * must know which datatype they contain and convert them to (lists)
 * of these dataypes by calling the various readXXXOption() or
 * readXXXOptionList() functions.
 *
 */
#include "ConfigParser.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;
// Declaration of a global configuration parser:
ConfigParser *gConfigParser = 0;
int config_file_line_number = 1;
int last_error_line_number = 0;
extern int yyparse();
extern int yydebug;
//! Constructor which does basically nothing
ConfigParser::ConfigParser()
{
  config_file = 0;
}
//! Initialize configuration using the command line arguments
bool ConfigParser::init(const char *configFileName)
{
  // Open configuration file and set the global file-descriptor
  if(configFileName)
    config_file = fopen (configFileName, "r");
  else
    config_file = fopen ("config", "r");
  if(!config_file) return false;
  // yydebug=1; // Set this to 1 for debugging purposes
  int r=0;
  config_file_line_number=1;
  while (!r){
    // cerr << " r = " << r << endl;
    r=yyparse();
  }
  // Close config_file
  fclose(config_file);
  if(r!=-1) return false;
  return true;
}
//! Destructor: Deletes the entries in the list of configuration file lines
ConfigParser::~ConfigParser()
{
  // The "option" string and the list are "self deleting"
  // we do not need to do anything therefore, here.
  list<ConfigFileLine *>::iterator I=configLines.begin();
  while (I != configLines.end()) {
    delete *I;
    *I++=0;
  }
  configLines.clear();
}
//! Append the option name of another ConfigFileLine as value
void ConfigParser::appendLine(ConfigFileLine *line)
{
  configLines.push_back(line);
}
//! Deletes a line from the already parsed configuration file lines
bool ConfigParser::deleteLine(const char *name, const char *scope)
{
  string s;
  if(scope)
    s=string(scope).append("::").append(name);
  else
    s=string(name);
  list<ConfigFileLine *>::iterator I=configLines.begin();
  while (I != configLines.end()) {
    if((*I)->isOption(s)){
      configLines.erase(I);
      return true;
    }
    I++;
  }
  return false;
}
//! Override an option
bool ConfigParser::overrideOption(const char *name,
				  list<string> &values,
				  const char *scope)
{
  bool result=deleteLine(name, scope);
  ConfigFileLine *line=new ConfigFileLine(name);
  if(scope) line->setScope(scope);
  line->setValues(values);
  configLines.push_back(line);
  return result;
}
//! Is this option defined?
bool ConfigParser::isDefined(const char *name) const
{
  string s(name);
  LineIterator I=configLines.begin();
  while (I != configLines.end()) {
    if((*I)->isOption(s)) return true;
    I++;
  }
  return false;
}
//! Read integer option
int ConfigParser::readIntOption(const char *name) const
  throw(const char *){
  string s(name);
  LineIterator I=configLines.begin();
  while (I != configLines.end()) {
    if((*I)->isOption(s)){
      list<string> values=(*I)->getValues();
      if(!values.size())
	throw "ConfigParser::readIntOption: No integer option!";
      list<string>::const_iterator I2=values.begin();
      return atoi((*I2).c_str());
    }
    I++;
  }
  cerr << "ConfigParser::readIntOption: No \""
	    << name << "\" option provided!\n" ;
  throw "ConfigParser::readIntOption: No such option provided!";
}
//! Read double option
double ConfigParser::readDoubleOption(const char *name) const
  throw(const char *){
  string s(name);
  LineIterator I=configLines.begin();
  while (I != configLines.end()) {
    if((*I)->isOption(s)){
      list<string> values=(*I)->getValues();
      if(!values.size())
	throw "ConfigParser::readDoubleOption: No double option!";
      list<string>::const_iterator I2=values.begin();
      return atof((*I2).c_str());
    }
    I++;
  }
  cerr << "ConfigParser::readDoubleOption: No \""
	    << name << "\" option provided!\n" ;
  throw "ConfigParser::readDoubleOption: No such option provided!";
}
//! Read float option
float ConfigParser::readFloatOption(const char *name) const
  throw(const char *){
  string s(name);
  LineIterator I=configLines.begin();
  while (I != configLines.end()) {
    if((*I)->isOption(s)){
      list<string> values=(*I)->getValues();
      if(!values.size())
	throw "ConfigParser::readFloatOption: No float option!";
      list<string>::const_iterator I2=values.begin();
      return atof((*I2).c_str());
    }
    I++;
  }
  cerr << "ConfigParser::readFloatOption: No \""
	    << name << "\" option provided!\n" ;
  throw "ConfigParser::readFloatOption: No such option provided!";
}
//! Read double list option
const char *ConfigParser::readStringOption(const char *name) const
  throw(const char *){
  string s(name);
  LineIterator I=configLines.begin();
  while (I != configLines.end()) {
    if((*I)->isOption(s)){
      list<string> values=(*I)->getValues();
      if(!values.size())
	throw "ConfigParser::readStringOption: No string option!";
      list<string>::const_iterator I2=values.begin();
      return (*I2).c_str();
    }
    I++;
  }
  cerr << "ConfigParser::readStringOption: No \""
	    << name << "\" option provided!\n" ;
  throw "ConfigParser::readStringOption: No such option provided!";
}
//! Read bool option
const bool ConfigParser::readBoolOption(const char *name) const
  throw(const char *){
  return (string(readStringOption(name))=="true") ? true : false;
  cerr << "ConfigParser::readBoolOption: No \""
	    << name << "\" option provided!\n" ;
  throw "ConfigParser::readBoolOption: No such option provided!";
}
//! Read integer list option
vector<int> ConfigParser::readIntListOption(const char *name) const
  throw(const char *){
  string s(name);
  LineIterator I=configLines.begin();
  while (I != configLines.end()) {
    if((*I)->isOption(s)){
      list<string> values=(*I)->getValues();
      list<string>::const_iterator I2=values.begin();
      vector<int> v(values.size());
      int i=0;
      while(I2!=values.end()){
	v[i]=atoi((*I2).c_str());
	i++; I2++;
      }
      return v;
    }
    I++;
  }
  cerr << "ConfigParser::readIntListOption: No \""
	    << name << "\" option provided!\n" ;
  throw "ConfigParser::readIntListOption: No such option provided!";
}
//! Read double list option
vector<double> ConfigParser::readDoubleListOption(const char *name) const
  throw(const char *){
  string s(name);
  LineIterator I=configLines.begin();
  while (I != configLines.end()) {
    if((*I)->isOption(s)){
      list<string> values=(*I)->getValues();
      list<string>::const_iterator I2=values.begin();
      vector<double> v(values.size());
      int i=0;
      while(I2!=values.end()){
	v[i]=atof((*I2).c_str());
	i++; I2++;
      }
      return v;
    }
    I++;
  }
  cerr << "ConfigParser::readDoubleListOption: No \""
	    << name << "\" option provided!\n" ;
  throw "ConfigParser::readDoubleListOption: No such option provided!";
}

//! Read float list option
vector<float> ConfigParser::readFloatListOption(const char *name) const
  throw(const char *){
  string s(name);
  LineIterator I=configLines.begin();
  while (I != configLines.end()) {
    if((*I)->isOption(s)){
      list<string> values=(*I)->getValues();
      list<string>::const_iterator I2=values.begin();
      vector<float> v(values.size());
      int i=0;
      while(I2!=values.end()){
	v[i]=atof((*I2).c_str());
	i++; I2++;
      }
      return v;
    }
    I++;
  }
  cerr << "ConfigParser::readFloatListOption: No \""
	    << name << "\" option provided!\n" ;
  throw "ConfigParser::readFloatListOption: No such option provided!";
}
//! Read string list option
vector<string> ConfigParser::readStringListOption(const char *name) const
  throw(const char *){
  string s(name);
  LineIterator I=configLines.begin();
  while (I != configLines.end()) {
    if((*I)->isOption(s)){
      list<string> values=(*I)->getValues();
      list<string>::const_iterator I2=values.begin();
      vector<string> v(values.size());
      int i=0;
      while(I2!=values.end()){
	v[i]=*I2;
	i++; I2++;
      }
      return v;
    }
    I++;
  }
  cerr << "ConfigParser::readStringListOption: No \""
	    << name << "\" option provided!\n" ;
  throw "ConfigParser::readStringListOption: No such option provided!";
}
//! Print all options to cout
void ConfigParser::print() const{
  cout << "Current configuration:" << endl;
  LineIterator I=configLines.begin();
  while (I != configLines.end()) {
    (*I)->print();
    I++;
  }
}
//! stream operator
ostream & operator<< (ostream & out, const ConfigParser & conf)
{
  out << "Current configuration:" << endl;
  list<ConfigFileLine *>::const_iterator I=conf.configLines.begin();
  while (I != conf.configLines.end()) {
    out << *(*I) ;
    // (*I)->print();
    I++;
  }
  return out ;
}
