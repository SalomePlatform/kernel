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
//  File   : SALOMEDS_AttributeLocalID_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//  $Header:

#ifndef SALOMEDS_AttributeLocalID_i_HeaderFile
#define SALOMEDS_AttributeLocalID_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeLocalID.hxx"

class SALOMEDS_AttributeLocalID_i: public virtual POA_SALOMEDS::AttributeLocalID,
                                   public virtual SALOMEDS_GenericAttribute_i 
{
public:
  
  SALOMEDS_AttributeLocalID_i(SALOMEDSImpl_AttributeLocalID* theAttr, CORBA::ORB_ptr orb) 
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {}; 

  ~SALOMEDS_AttributeLocalID_i() {};

  CORBA::Long Value();
  void SetValue(CORBA::Long value);

};



#endif
