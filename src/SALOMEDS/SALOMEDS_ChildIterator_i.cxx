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
//  File   : SALOMEDS_ChildIterator_i.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#include "SALOMEDS_ChildIterator_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include "utilities.h"
using namespace std;

//============================================================================
/*! Function : constructor
 *  Purpose  :
 */
//============================================================================
SALOMEDS_ChildIterator_i::SALOMEDS_ChildIterator_i(TDF_Label lab,
						   CORBA::ORB_ptr orb) 
  : _lab(lab)
{
  _orb = CORBA::ORB::_duplicate(orb);
  _it.Initialize (lab);
}

//============================================================================
/*! Function : destructor
 *  Purpose  :
 */
//============================================================================
SALOMEDS_ChildIterator_i::~SALOMEDS_ChildIterator_i()
{
}

//============================================================================
/*! Function :Init
 * 
 */
//============================================================================
void SALOMEDS_ChildIterator_i::Init()
{ 
  _it.Initialize (_lab);
}

//============================================================================
/*! Function :InitEx
 * 
 */
//============================================================================
void SALOMEDS_ChildIterator_i::InitEx(CORBA::Boolean allLevels)
{ 
  _it.Initialize (_lab, allLevels);
}

//============================================================================
/*! Function : More
 * 
 */
//============================================================================
CORBA::Boolean SALOMEDS_ChildIterator_i::More()
{
  return _it.More();
}

 //============================================================================
/*! Function : Next
 * 
 */
//============================================================================
void SALOMEDS_ChildIterator_i::Next()
{
  _it.Next();
}


//============================================================================
/*! Function :
 *  Purpose  :
 */
//============================================================================

SALOMEDS::SObject_ptr SALOMEDS_ChildIterator_i::Value()
{
  TDF_Label L = _it.Value();
  SALOMEDS_SObject_i *  so_servant = new SALOMEDS_SObject_i (L,_orb);
  SALOMEDS::SObject_var so = SALOMEDS::SObject::_narrow(so_servant->_this());
  return so;
}

