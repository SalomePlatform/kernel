// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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
//  File   : SALOMEDSImpl_AttributeIOR.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeParameter_HeaderFile
#define _SALOMEDSImpl_AttributeParameter_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TDF_Label.hxx> 
#include "SALOMEDSImpl_GenericAttribute.hxx"

#include <vector>
#include <string>
#include <map>

class Standard_GUID;
class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);


DEFINE_STANDARD_HANDLE( SALOMEDSImpl_AttributeParameter, SALOMEDSImpl_GenericAttribute )

enum Parameter_Types {PT_INTEGER, PT_REAL, PT_BOOLEAN, PT_STRING, PT_REALARRAY, PT_INTARRAY, PT_STRARRAY};

/*!
 Class: SALOMEDSImpl_AttributeParameter
 Description : AttributeParameter is a universal container of basic types 
*/

class SALOMEDSImpl_AttributeParameter : public SALOMEDSImpl_GenericAttribute 
{

private:
  
  std::map<std::string, int>                     _ints;
  std::map<std::string, double>                  _reals;
  std::map<std::string, std::string>             _strings;
  std::map<std::string, bool>                    _bools;
  std::map< std::string, vector<double> >        _realarrays;
  std::map< std::string, vector<int> >           _intarrays;
  std::map< std::string, vector<std::string> >   _strarrays;

public:
  Standard_EXPORT static const Standard_GUID& GetID() ;

  Standard_EXPORT  SALOMEDSImpl_AttributeParameter():SALOMEDSImpl_GenericAttribute("AttributeParameter") {}
  Standard_EXPORT  static Handle(SALOMEDSImpl_AttributeParameter) Set (const TDF_Label& L);


  Standard_EXPORT void SetInt(const std::string& theID, const int& theValue);
  Standard_EXPORT int GetInt(const std::string& theID);

  Standard_EXPORT void SetReal(const std::string& theID, const double& theValue);
  Standard_EXPORT double GetReal(const std::string& theID);

  Standard_EXPORT void SetString(const std::string& theID, const std::string& theValue);
  Standard_EXPORT std::string GetString(const std::string& theID);
  
  Standard_EXPORT void SetBool(const std::string& theID, const bool& theValue);
  Standard_EXPORT bool GetBool(const std::string& theID);
  
  Standard_EXPORT void SetRealArray(const std::string& theID, const std::vector<double>& theArray);
  Standard_EXPORT std::vector<double> GetRealArray(const std::string& theID);
  
  Standard_EXPORT void SetIntArray(const std::string& theID, const std::vector<int>& theArray);
  Standard_EXPORT std::vector<int> GetIntArray(const std::string& theID);

  Standard_EXPORT void SetStrArray(const std::string& theID, const std::vector<std::string>& theArray);
  Standard_EXPORT std::vector<std::string> GetStrArray(const std::string& theID);

  Standard_EXPORT bool IsSet(const std::string& theID, const Parameter_Types theType);
  
  Standard_EXPORT bool RemoveID(const std::string& theID, const Parameter_Types theType);

  Standard_EXPORT Handle(SALOMEDSImpl_AttributeParameter) GetFather();
  Standard_EXPORT bool HasFather();
  Standard_EXPORT bool IsRoot();

  Standard_EXPORT std::vector<std::string> GetIDs(const Parameter_Types theType);

  Standard_EXPORT void Clear();

  Standard_EXPORT  virtual TCollection_AsciiString Save();
  Standard_EXPORT  virtual void Load(const TCollection_AsciiString& theValue); 

  Standard_EXPORT  const Standard_GUID& ID() const;
  Standard_EXPORT  void Restore(const Handle(TDF_Attribute)& with) ;
  Standard_EXPORT  Handle_TDF_Attribute NewEmpty() const;
  Standard_EXPORT  void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;
  
  Standard_EXPORT ~SALOMEDSImpl_AttributeParameter() {}

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_AttributeParameter )
};

#endif
