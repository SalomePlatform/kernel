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
//  File   : SALOMEDS_SComponentIterator_i.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#include "SALOMEDS_SComponentIterator_i.hxx"
using namespace std;

//============================================================================
/*! Function : constructor
 * 
 */
//============================================================================

SALOMEDS_SComponentIterator_i::SALOMEDS_SComponentIterator_i(const Handle(TDocStd_Document) aDoc, 
							     CORBA::ORB_ptr orb) 
{
  _orb = CORBA::ORB::_duplicate(orb);
  _lab = aDoc->Main();
  _it.Initialize (_lab);
}

//============================================================================
/*! Function : destructor
 * 
 */
//============================================================================
SALOMEDS_SComponentIterator_i::~SALOMEDS_SComponentIterator_i()
{
}

//============================================================================
/*! Function : Init
 * 
 */
//============================================================================
void SALOMEDS_SComponentIterator_i::Init()
{ 
  _it.Initialize (_lab);
}

//============================================================================
/*! Function : More
 * 
 */
//============================================================================
CORBA::Boolean SALOMEDS_SComponentIterator_i::More()
{
  if (!_it.More())
    return false;
  TDF_Label L = _it.Value();
  if (SALOMEDS_SComponent_i::IsA(L))
     return true;

  return _it.More();
}

 //============================================================================
/*! Function : Next
  */
//============================================================================
void SALOMEDS_SComponentIterator_i::Next()
{
  _it.Next();
}


//============================================================================
/*! Function : Value
 * 
 */
//============================================================================
SALOMEDS::SComponent_ptr SALOMEDS_SComponentIterator_i::Value()
{
  SALOMEDS_SComponent_i *  so_servant = new SALOMEDS_SComponent_i (_it.Value(),_orb);
  SALOMEDS::SComponent_var so  = SALOMEDS::SComponent::_narrow(so_servant->SComponent::_this());   //pb d'heritage??
  return so;
}

