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

//  File   : SALOMEDS_AttributeParameter.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef SALOMEDS_AttributeParameter_HeaderFile
#define SALOMEDS_AttributeParameter_HeaderFile

#include "SALOMEDSClient_AttributeParameter.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeParameter.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEDS_AttributeParameter: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeParameter
{
public:  
  SALOMEDS_AttributeParameter(SALOMEDSImpl_AttributeParameter* theAttr);
  SALOMEDS_AttributeParameter(SALOMEDS::AttributeParameter_ptr theAttr);
  ~SALOMEDS_AttributeParameter();

  virtual void SetInt(const std::string& theID, const int theValue);
  virtual int GetInt(const std::string& theID);

  virtual void SetReal(const std::string& theID, const double& theValue);
  virtual double GetReal(const std::string& theID);

  virtual void SetString(const std::string& theID, const std::string& theValue);
  virtual std::string GetString(const std::string& theID);
  
  virtual void SetBool(const std::string& theID, const bool& theValue);
  virtual bool GetBool(const std::string& theID);
  
  virtual void SetRealArray(const std::string& theID, const std::vector<double>& theArray);
  virtual std::vector<double> GetRealArray(const std::string& theID);
  
  virtual void SetIntArray(const std::string& theID, const std::vector<int>& theArray);
  virtual std::vector<int> GetIntArray(const std::string& theID);

  virtual void SetStrArray(const std::string& theID, const std::vector<std::string>& theArray);
  virtual std::vector<std::string> GetStrArray(const std::string& theID);

  virtual bool IsSet(const std::string& theID, const int theType);
  
  virtual bool RemoveID(const std::string& theID, const int theType);

  virtual _PTR(AttributeParameter) GetFather();
  virtual bool HasFather();
  virtual bool IsRoot();
  
  virtual void Clear();

  virtual std::vector<std::string> GetIDs(const int theType);
};

#endif
