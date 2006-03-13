// Copyright (C) 2005  OPEN CASCADE, CEA/DEN, EDF R&D, PRINCIPIA R&D
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/
//
#ifndef SALOMEDS_IParameters_H
#define SALOMEDS_IParameters_H

#include <string>
#include <vector>
#include <map>

#include "SALOMEDSClient.hxx"

/*! 
  Class which an interface to store the parameters of the objects
*/
class SALOMEDS_IParameters
{
public:
  SALOMEDS_IParameters(const _PTR(AttributeParameter)& ap); 

  virtual ~SALOMEDS_IParameters();

  /*!
    Appends a string value to a named list. 
    Returns a number of the added value.
    Note: the name of the list MUST be unique
   */
  virtual int append(const std::string& listName, const std::string& value); 

  /*!
    Returns a number elements in the list
   */
  virtual int nbValues(const std::string& listName);

  /*!
    Returns a list of values in the list
   */
  virtual std::vector<std::string> getValues(const std::string& listName);

  /*!
    Returns a value with given %index, where %index is in range [0:nbValues-1]
   */
  virtual std::string getValue(const std::string& listName, int index);

  /*!
    Returns a list all entries lists
   */
  virtual std::vector<std::string> getLists();

  /*!
    Sets a new named parameter value for the given entry
   */
  virtual void setParameter(const std::string& entry, const std::string& parameterName, const std::string& value);

  /*!
    Gets a named parameter value for the given entry
   */
  virtual std::string getParameter(const std::string& entry, const std::string& parameterName);

  /*!
    Returns all parameter names of the given entry
   */
  virtual std::vector<std::string> getAllParameterNames(const std::string& entry);

  /*!
    Returns all parameter  values of the given entry
   */
  virtual std::vector<std::string> getAllParameterValues(const std::string& entry);

  /*!
    Returns a number of parameters of the given entry
   */
  virtual int getNbParameters(const std::string& entry);

  /*!
    Returns a list all entries 
   */
  virtual std::vector<std::string> getEntries();

  /*!
    Sets a global named property value
   */
  virtual void setProperty(const std::string& name, const std::string& value);

  /*!
    Gets a value of global named property
   */
  virtual std::string getProperty(const std::string& name);

  /*!
    Returns a list all properties 
   */
  virtual std::vector<std::string> getProperties();

  /*!
    Breaks a value string in two parts which is divided by %separator.
    If fromEnd is True the search of separator starts from the end of the string
   */
  virtual std::vector<std::string> parseValue(const std::string& value, const char separator, bool fromEnd = true);


  /*!
    Returns encoded entry that is a relative entry for the component
   */
  virtual std::string encodeEntry(const std::string& entry, const std::string& compName);

  /*!
    Returns decoded entry that is an absolute entry
   */
  virtual std::string decodeEntry(const std::string& entry);

  /*!
    Enables/Disables the dumping visual parameters
   */
  static void setDumpPython(_PTR(Study) study, const std::string& theID = "");

  /*!
    Returns whether there is the dumping visual parameters
   */
  static bool isDumpPython(_PTR(Study) study, const std::string& theID = "");  

  /*!
    Returns a default name of the component where the visula parameters are stored.
  */
  static std::string getDefaultVisualComponent();

private:
  _PTR(AttributeParameter) _ap;
  _PTR(Study) _study;
  std::map<std::string, std::string> _compNames;
};


#endif
