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

//  File   : SALOMEDSImpl_AttributeIOR.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef _SALOMEDSImpl_AttributeParameter_HeaderFile
#define _SALOMEDSImpl_AttributeParameter_HeaderFile

#include "SALOMEDSImpl_Defines.hxx"
#include "DF_Attribute.hxx"
#include "DF_Label.hxx"
#include "SALOMEDSImpl_GenericAttribute.hxx"

#include <vector>
#include <string>
#include <map>

enum Parameter_Types {PT_INTEGER, PT_REAL, PT_BOOLEAN, PT_STRING, PT_REALARRAY, PT_INTARRAY, PT_STRARRAY};

/*!
Class: SALOMEDSImpl_AttributeParameter
Description : AttributeParameter is a universal container of basic types 
*/

class SALOMEDSIMPL_EXPORT SALOMEDSImpl_AttributeParameter : 
  public SALOMEDSImpl_GenericAttribute 
{

private:

  std::map<std::string, int>                          _ints;
  std::map<std::string, double>                       _reals;
  std::map<std::string, std::string>                  _strings;
  std::map<std::string, bool>                         _bools;
  std::map< std::string, std::vector<double> >        _realarrays;
  std::map< std::string, std::vector<int> >           _intarrays;
  std::map< std::string, std::vector<std::string> >   _strarrays;

public:

  static const std::string& GetID() ;
  
  SALOMEDSImpl_AttributeParameter():SALOMEDSImpl_GenericAttribute("AttributeParameter") {}
  static SALOMEDSImpl_AttributeParameter* Set (const DF_Label& L);
      
  void SetInt(const std::string& theID, const int& theValue);
  int GetInt(const std::string& theID);
          
  void SetReal(const std::string& theID, const double& theValue);
  double GetReal(const std::string& theID);
              
  void SetString(const std::string& theID, const std::string& theValue);
  std::string GetString(const std::string& theID);
                  
  void SetBool(const std::string& theID, const bool& theValue);
  bool GetBool(const std::string& theID);
                      
  void SetRealArray(const std::string& theID, const std::vector<double>& theArray);
  std::vector<double> GetRealArray(const std::string& theID);
                          
  void SetIntArray(const std::string& theID, const std::vector<int>& theArray);
  std::vector<int> GetIntArray(const std::string& theID);
                              
  void SetStrArray(const std::string& theID, const std::vector<std::string>& theArray);
  std::vector<std::string> GetStrArray(const std::string& theID);
                                  
  bool IsSet(const std::string& theID, const Parameter_Types theType);
 
  bool RemoveID(const std::string& theID, const Parameter_Types theType);
                      
  SALOMEDSImpl_AttributeParameter* GetFather();
  bool HasFather();
  bool IsRoot();
                                            
  std::vector<std::string> GetIDs(const Parameter_Types theType);
                                              
  void Clear();
                                                
  virtual std::string Save();
  virtual void Load(const std::string& theValue);
                                                    
  const std::string& ID() const;
  void Restore(DF_Attribute* with) ;
  DF_Attribute* NewEmpty() const;
  void Paste(DF_Attribute* into);
                                                            
  ~SALOMEDSImpl_AttributeParameter() {}
};

#endif
