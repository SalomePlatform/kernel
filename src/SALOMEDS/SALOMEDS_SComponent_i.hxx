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
//  File   : SALOMEDS_SComponent_i.hxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#ifndef __SALOMEDS_SCOMPONENT_I_H__
#define __SALOMEDS_SCOMPONENT_I_H__

#include "SALOMEDS_SObject_i.hxx"

class SALOMEDS_SComponent_i: public virtual POA_SALOMEDS::SComponent,
			     public virtual SALOMEDS_SObject_i
{
  SALOMEDS_SComponent_i(); // Not implemented
  void operator=(const SALOMEDS_SComponent_i&); // Not implemented

  SALOMEDS_SComponent_i(SALOMEDS_Study_i* theStudy,
			const TDF_Label& theLabel);
  
public:
  static SALOMEDS_SComponent_i* New(SALOMEDS_Study_i* theStudy,
				    const TDF_Label& theLabel);
  
  ~SALOMEDS_SComponent_i();
  
  virtual char* ComponentDataType();

  virtual CORBA::Boolean ComponentIOR(CORBA::String_out theID);
  
  static Standard_Boolean IsA(const TDF_Label& Lab);

};


#endif
