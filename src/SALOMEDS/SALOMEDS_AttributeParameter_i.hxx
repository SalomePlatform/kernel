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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDS_AttributeParameter_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//  $Header:

#ifndef SALOMEDS_AttributeParameter_i_HeaderFile
#define SALOMEDS_AttributeParameter_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeParameter.hxx"

/*!
 Class: SALOMEDS_AttributeParameter
 Description : AttributeParameter is a universal container of basic types 
*/

class SALOMEDS_AttributeParameter_i: public virtual POA_SALOMEDS::AttributeParameter,
				     public virtual SALOMEDS_GenericAttribute_i 
{
public:
  
  SALOMEDS_AttributeParameter_i(const Handle(SALOMEDSImpl_AttributeParameter)& theAttr, CORBA::ORB_ptr orb) 
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {}; 

  ~SALOMEDS_AttributeParameter_i() {};


  virtual void SetInt(const char* theID, CORBA::Long theValue);
  virtual CORBA::Long GetInt(const char* theID);

  virtual void SetReal(const char* theID, CORBA::Double theValue);
  virtual CORBA::Double GetReal(const char* theID);

  virtual void SetString(const char* theID, const char* theValue);
  virtual char* GetString(const char* theID);
  
  virtual void SetBool(const char* theID, CORBA::Boolean theValue);
  virtual CORBA::Boolean GetBool(const char* theID);
  
  virtual void SetRealArray(const char* theID, const SALOMEDS::DoubleSeq& theArray);
  virtual SALOMEDS::DoubleSeq* GetRealArray(const char* theID);

  virtual void SetIntArray(const char* theID, const SALOMEDS::LongSeq& theArray);
  virtual SALOMEDS::LongSeq* GetIntArray(const char* theID);

  virtual void SetStrArray(const char* theID, const SALOMEDS::StringSeq& theArray);
  virtual SALOMEDS::StringSeq* GetStrArray(const char* theID);
  
  virtual CORBA::Boolean IsSet(const char* theID, CORBA::Long theType);
  
  virtual CORBA::Boolean RemoveID(const char* theID, CORBA::Long theType);

  virtual SALOMEDS::AttributeParameter_ptr GetFather();
  virtual CORBA::Boolean HasFather();
  virtual CORBA::Boolean IsRoot();

  virtual void Clear();

  virtual SALOMEDS::StringSeq* GetIDs(CORBA::Long theType);

};




#endif
