using namespace std;
//  File      : SALOMEDS_ChildIterator_i.cxx
//  Created   : Wed Nov 28 16:15:25 2001
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header$

#include "SALOMEDS_ChildIterator_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include "utilities.h"



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

