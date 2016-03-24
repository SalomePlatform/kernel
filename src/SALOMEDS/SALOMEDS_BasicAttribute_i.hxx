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

//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//  File   : SALOMEDS_BasicAttribute_i.hxx
//  Author : Estelle Deville, CEA
//  Module : SALOME
//  $Header$
//
#ifndef _BASIC_ATTRIBUTE_I_HXX_
#define _BASIC_ATTRIBUTE_I_HXX_

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include <DF_Label.hxx>

class SALOMEDS_BasicAttribute_i: public POA_SALOMEDS::BasicAttribute,
                                 public PortableServer::ServantBase {
protected:
  DF_Label _Lab;

public:
  SALOMEDS_BasicAttribute_i() {};

  ~SALOMEDS_BasicAttribute_i() {};

  virtual char *  Save() =0;
  virtual void    Load () =0;
  CORBA::Boolean    GetPtr(SALOMEDS::SObject so);  
  void SetLabel(const DF_Label& Lab);
  virtual char* GetType() = 0;

};

#endif
