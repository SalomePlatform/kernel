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
#ifndef _SALOMEDSClient_AttributeParameter_HeaderFile
#define _SALOMEDSClient_AttributeParameter_HeaderFile

#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 
#include <string>
#include <vector>

class SALOMEDSClient_AttributeParameter : public virtual SALOMEDSClient_GenericAttribute 
{

public:

  virtual void SetInt(const std::string& theID, const int theValue) = 0;
  virtual int GetInt(const std::string& theID) = 0;

  virtual void SetReal(const std::string& theID, const double& theValue) = 0;
  virtual double GetReal(const std::string& theID) = 0;

  virtual void SetString(const std::string& theID, const std::string& theValue) = 0;
  virtual std::string GetString(const std::string& theID) = 0;
  
  virtual void SetBool(const std::string& theID, const bool& theValue) = 0;
  virtual bool GetBool(const std::string& theID) = 0;
  
  virtual void SetRealArray(const std::string& theID, const std::vector<double>& theArray) = 0;
  virtual std::vector<double> GetRealArray(const std::string& theID) = 0;
  
  virtual void SetIntArray(const std::string& theID, const std::vector<int>& theArray) = 0;
  virtual std::vector<int> GetIntArray(const std::string& theID) = 0;

  virtual void SetStrArray(const std::string& theID, const std::vector<std::string>& theArray) = 0;
  virtual std::vector<std::string> GetStrArray(const std::string& theID) = 0;

  virtual bool IsSet(const std::string& theID, const int theType) = 0;
  
  virtual bool RemoveID(const std::string& theID, const int theType) = 0;

  virtual _PTR(AttributeParameter) GetFather() = 0;
  virtual bool HasFather() = 0;
  virtual bool IsRoot() = 0;
  
  virtual void Clear() = 0;

  virtual std::vector<std::string> GetIDs(const int theType) = 0;
};

#endif
