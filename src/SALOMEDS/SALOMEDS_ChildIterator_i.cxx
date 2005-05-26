//  File   : SALOMEDS_ChildIterator_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDS_ChildIterator_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include "SALOMEDS.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_Study.hxx"
#include "utilities.h"

//============================================================================
/*! Function : constructor
 *  Purpose  :
 */
//============================================================================
SALOMEDS_ChildIterator_i::SALOMEDS_ChildIterator_i(const Handle(SALOMEDSImpl_ChildIterator)& theImpl,
						   CORBA::ORB_ptr orb) 
  : _it(theImpl)
{
  SALOMEDS::Locker lock;
  _orb = CORBA::ORB::_duplicate(orb);
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
  SALOMEDS::Locker lock;
  _it->Init();
}

//============================================================================
/*! Function :InitEx
 * 
 */
//============================================================================
void SALOMEDS_ChildIterator_i::InitEx(CORBA::Boolean allLevels)
{ 
  SALOMEDS::Locker lock;
  _it->InitEx (allLevels);
}

//============================================================================
/*! Function : More
 * 
 */
//============================================================================
CORBA::Boolean SALOMEDS_ChildIterator_i::More()
{
  SALOMEDS::Locker lock;
  return _it->More();
}

 //============================================================================
/*! Function : Next
 * 
 */
//============================================================================
void SALOMEDS_ChildIterator_i::Next()
{
  SALOMEDS::Locker lock;
  _it->Next();
}


//============================================================================
/*! Function : Value
 *  Purpose  :
 */
//============================================================================

SALOMEDS::SObject_ptr SALOMEDS_ChildIterator_i::Value()
{
  SALOMEDS::Locker lock;
  Handle(SALOMEDSImpl_SObject) aSO = _it->Value();
  SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (aSO, _orb);
  return so._retn();
}

