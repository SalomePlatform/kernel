//  File   : SALOMEDS_SComponentIterator_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDS_SComponentIterator_i.hxx"
#include "SALOMEDS.hxx"
#include "SALOMEDSImpl_SComponent.hxx"

//============================================================================
/*! Function : constructor
 * 
 */
//============================================================================

SALOMEDS_SComponentIterator_i::SALOMEDS_SComponentIterator_i(const SALOMEDSImpl_SComponentIterator& theImpl, 
							     CORBA::ORB_ptr orb) 
:_impl(theImpl)
{
  _orb = CORBA::ORB::_duplicate(orb);
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
  SALOMEDS::Locker lock; 
  _impl.Init();
}

//============================================================================
/*! Function : More
 * 
 */
//============================================================================
CORBA::Boolean SALOMEDS_SComponentIterator_i::More()
{
  SALOMEDS::Locker lock; 
  return _impl.More();
}

 //============================================================================
/*! Function : Next
  */
//============================================================================
void SALOMEDS_SComponentIterator_i::Next()
{ 
  SALOMEDS::Locker lock; 
  _impl.Next();
}


//============================================================================
/*! Function : Value
 * 
 */
//============================================================================
SALOMEDS::SComponent_ptr SALOMEDS_SComponentIterator_i::Value()
{
  SALOMEDS::Locker lock; 
  SALOMEDS::SComponent_var sco = SALOMEDS_SComponent_i::New (_impl.Value(), _orb);
  return sco._retn();
}

