// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

#ifndef SALOMEDSClient_IParameters_H
#define SALOMEDSClient_IParameters_H

#include <string>
#include <vector>
#include <map>

#include "SALOMEDSClient_definitions.hxx"
#include "SALOMEDSClient_AttributeParameter.hxx"

/*! 
  Class which an interface to store the parameters of the objects
*/
class SALOMEDSClient_IParameters
{
public:
  
   virtual ~SALOMEDSClient_IParameters() {}

  /*!
    Appends a string value to a named list. 
    Returns a number of the added value.
    Note: the name of the list MUST be unique
   */
  virtual int append(const std::string& listName, const std::string& value) = 0; 

  /*!
    Returns a number elements in the list
   */
  virtual int nbValues(const std::string& listName) = 0;

  /*!
    Returns a list of values in the list
   */
  virtual std::vector<std::string> getValues(const std::string& listName) = 0;

  /*!
    Returns a value with given %index, where %index is in range [0:nbValues-1]
   */
  virtual std::string getValue(const std::string& listName, int index) = 0;

  /*!
    Returns a list all entries lists
   */
  virtual std::vector<std::string> getLists() = 0;

  /*!
    Sets a new named parameter value for the given entry
   */
  virtual void setParameter(const std::string& entry, const std::string& parameterName, const std::string& value) = 0;

  /*!
    Gets a named parameter value for the given entry
   */
  virtual std::string getParameter(const std::string& entry, const std::string& parameterName) = 0;

  /*!
    Sets an id parameter value for the given entry.
   */
  virtual void setIdParameter(const std::string& entry, const std::string& value) = 0;

  /*!
    Gets an id parameter value for the given entry
   */
  virtual std::string getIdParameter(const std::string& entry) = 0;

  /*!
    Returns all parameter names of the given entry
   */
  virtual std::vector<std::string> getAllParameterNames(const std::string& entry) = 0;

  /*!
    Returns all parameter  values of the given entry
   */
  virtual std::vector<std::string> getAllParameterValues(const std::string& entry) = 0;

  /*!
    Returns a number of parameters of the given entry
   */
  virtual int getNbParameters(const std::string& entry) = 0;

  /*!
    Returns a list all entries 
   */
  virtual std::vector<std::string> getEntries() = 0;

  /*!
    Sets a global named property value
   */
  virtual void setProperty(const std::string& name, const std::string& value) = 0;

  /*!
    Gets a value of global named property
   */
  virtual std::string getProperty(const std::string& name) = 0;

  /*!
    Returns a list all properties 
   */
  virtual std::vector<std::string> getProperties() = 0;

  /*!
    Breaks a value string in two parts which is divided by %separator.
    If fromEnd is True the search of separator starts from the end of the string
   */
  virtual std::vector<std::string> parseValue(const std::string& value, const char separator, bool fromEnd = true) = 0;


  /*!
    Returns encoded entry that is a relative entry for the component
   */
  virtual std::string encodeEntry(const std::string& entry, const std::string& compName) = 0;

  /*!
    Returns decoded entry that is an absolute entry
   */
  virtual std::string decodeEntry(const std::string& entry) = 0;

  /*!
    Enables/Disables the dumping visual parameters, static implementation is supposed
   */
  virtual void setDumpPython(_PTR(Study) study, const std::string& theID = "") = 0;

  /*!
    Returns whether there is the dumping visual parameters, static implementation is supposed
   */
  virtual bool isDumpPython(_PTR(Study) study, const std::string& theID = "") = 0;  

  /*!
    Returns a default name of the component where the visula parameters are stored.
    Static implementation is supposed
  */
  virtual std::string getDefaultVisualComponent() = 0;

};


#endif
