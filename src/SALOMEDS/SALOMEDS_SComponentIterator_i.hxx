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
//  File   : SALOMEDS_SComponentIterator_i.hxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#ifndef __SALOMEDS_SCOMPONENTITERATOR_I_H__
#define __SALOMEDS_SCOMPONENTITERATOR_I_H__

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

// Cascade headers
#include <TDocStd_Document.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_Label.hxx>

class SALOMEDS_Study_i;

class SALOMEDS_SComponentIterator_i:public virtual POA_SALOMEDS::SComponentIterator,
				    public virtual PortableServer::RefCountServantBase 
{
  SALOMEDS_SComponentIterator_i(); // Not implemented
  void operator=(const SALOMEDS_SComponentIterator_i&); // Not implemented

private:
  TDF_ChildIterator        _it;
  TDF_Label                _lab;
  SALOMEDS_Study_i*        _study;

public:
  SALOMEDS_SComponentIterator_i(SALOMEDS_Study_i* theStudy,
				const Handle(TDocStd_Document)& theDocument);
  
  ~SALOMEDS_SComponentIterator_i();
  
  virtual void Init();
  virtual CORBA::Boolean More();
  virtual void Next();
  virtual SALOMEDS::SComponent_ptr Value();  
};


#endif
