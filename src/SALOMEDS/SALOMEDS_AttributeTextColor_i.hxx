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
//  File   : SALOMEDS_AttributeTextColor_i.hxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#ifndef SALOMEDS_AttributeTextColor_i_HeaderFile
#define SALOMEDS_AttributeTextColor_i_HeaderFile

// IDL headers
#include "SALOMEDS_TextColorAttribute.hxx"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"

class SALOMEDS_AttributeTextColor_i: public virtual POA_SALOMEDS::AttributeTextColor,
				public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeTextColor_i(const Handle(SALOMEDS_TextColorAttribute)& theTextColorAttr, CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = theTextColorAttr;
  };
  ~SALOMEDS_AttributeTextColor_i() {};

  SALOMEDS::Color TextColor();
  void SetTextColor(const SALOMEDS::Color& value);

  char* Store();
  void Restore(const char*);

};


#endif
