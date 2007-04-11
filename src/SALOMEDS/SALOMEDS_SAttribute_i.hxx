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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : SALOMEDS_SAttribute_i.hxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#ifndef __SALOMEDS_SATTRIBUTE_I_H__
#define __SALOMEDS_SATTRIBUTE_I_H__

// std C++ headers
#include <iostream>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)



// Cascade headers
#include <TDocStd_Document.hxx>
#include <stdio.h>

class SALOMEDS_SAttribute_i: public POA_IStudy::SAttribute,
			public PortableServer::ServantBase {
private:
  CORBA::ORB_ptr           _orb;
  char*                    _Type;
  char
public:
  
  SALOMEDS_SAttribute_i(const Handle(TDocStd_Document), CORBA::ORB_ptr);
  
  ~SALOMEDS_SAttribute_i();

   virtual char* GetID() ;
   virtual SObject_ptr GetOwner();
   virtual char* GetType();
   virtual char* GetValue();
  
};
#endif
