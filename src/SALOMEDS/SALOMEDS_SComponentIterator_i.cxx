using namespace std;
//  File      : SALOMEDS_SComponentIterator_i.cxx
//  Created   : Wed Nov 28 16:23:49 2001
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header$

#include "SALOMEDS_SComponentIterator_i.hxx"


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

