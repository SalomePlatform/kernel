//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEDS_UseCaseIterator_i.cxx
//  Author : Yves FRICAUD
//  Module : SALOME

using namespace std;
#include "SALOMEDS_UseCaseIterator_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include "utilities.h"



//============================================================================
/*! Function : constructor
 *  Purpose  :
 */
//============================================================================
SALOMEDS_UseCaseIterator_i::SALOMEDS_UseCaseIterator_i(SALOMEDS_Study_i* theStudy,
						       const TDF_Label& theLabel, 
						       const Standard_GUID& theGUID, 
						       const Standard_Boolean theIsAllLevels):
  _guid(theGUID), 
  _levels(theIsAllLevels),
  _study(theStudy)
{
  if(theLabel.FindAttribute(_guid, _node)) {
    _it.Initialize (_node, _levels);
  }
}

//============================================================================
/*! Function : destructor
 *  Purpose  :
 */
//============================================================================
SALOMEDS_UseCaseIterator_i::~SALOMEDS_UseCaseIterator_i()
{
}

//============================================================================
/*! Function :Init
 * 
 */
//============================================================================
void SALOMEDS_UseCaseIterator_i::Init(CORBA::Boolean allLevels)
{ 
  _it.Initialize (_node, allLevels);
}

//============================================================================
/*! Function : More
 * 
 */
//============================================================================
CORBA::Boolean SALOMEDS_UseCaseIterator_i::More()
{
  return _it.More();
}

 //============================================================================
/*! Function : Next
 * 
 */
//============================================================================
void SALOMEDS_UseCaseIterator_i::Next()
{
  _it.Next();
}


//============================================================================
/*! Function :
 *  Purpose  :
 */
//============================================================================

SALOMEDS::SObject_ptr SALOMEDS_UseCaseIterator_i::Value()
{
  TDF_Label L = _it.Value()->Label();
  return SALOMEDS_SObject_i::New(_study,L)->_this();
}

