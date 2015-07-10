// H4configfileline-.h
//
// A class representing a (continued) line in the configuration
// file, that is a pair of an option name and a (list of) value(s) 
//
// last change : $Date: 2013/02/05 11:22:36 $
// by          : $Author: rgerosa $
//

#ifndef CONFIGFILELINE_H
#define CONFIGFILELINE_H

#include <string>
#include <list>

using namespace std;

class ConfigFileLine {
 public:

  //! Constructor with name of the option and a single option value (no list!)
  ConfigFileLine(const string &option, const string &value="");

  //! Destructor: Does currently nothing
  virtual ~ConfigFileLine();

  //! Append the option name of another ConfigFileLine as value
  void append(const ConfigFileLine &other);

  //! Append the values of another ConfigFileLine
  void appendList(const ConfigFileLine &other);

  //! Check whether name of option is "name"
  bool isOption(const string &name) const{
    return name==option;
  }

  //! Check whether the option line has a name, i.e. is defined
  bool isDefined() const{
    return option.length();
  }

  //! Return the list of configuration values
  list<string> getValues() const{
    // This operation is somewhat time-consuming!
    return values;
  }

  //! Sets the list of configuration values
  void setValues(const list<string> &v){
    values=v;
  }

  //! Get option name
  const string &getOptionName() const{
    return option;
  }

  //! Sets the scope by prepending "scope":: to the option name
  void setScope(const string &scope);
  
  //! Print the contents of the rule to stdout
  void print() const;

  //! stream operator
  friend ostream & operator<< (ostream & out, const ConfigFileLine & line) ;

 private:

  string option;                //!< The name of the option
  list<string> values;     //!< The list of option values

  //! For convinience a ValueIterator type is defined which walks along
  //! the list of option strings
  typedef list<string>::const_iterator ValueIterator;

};

#endif // #ifndef CONFIGFILELINE
