//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SALOMEDS_GenericAttribute_i.hxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#ifndef _GENERIC_ATTRIBUTE_I_HXX_
#define _GENERIC_ATTRIBUTE_I_HXX_

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include <TDF_Attribute.hxx>
#include "SALOMEDS_SObject_i.hxx"
#include "SALOMEDS_IORAttribute.hxx"

class SALOMEDS_GenericAttribute_i: public POA_SALOMEDS::GenericAttribute,
				   public PortableServer::RefCountServantBase {
protected:
  Handle(TDF_Attribute) _myAttr;
  CORBA::ORB_ptr        _myOrb;
public:
  SALOMEDS_GenericAttribute_i() {};
  
  void CheckLocked() throw (SALOMEDS::GenericAttribute::LockProtection);
  
  ~SALOMEDS_GenericAttribute_i() {};

//  CORBA::Boolean  GetPtr(SALOMEDS::SObject_out so) {
//    SALOMEDS_SObject_i* sob_impl = new  SALOMEDS_SObject_i(_myAttr->Label(), _myOrb);
//    SALOMEDS::SObject_var sob = sob_impl->_this();
//    return sob._retn();
//  };


};

#endif
