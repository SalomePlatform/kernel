//  File   : SALOMEDSImpl_SComponentIterator.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDSImpl_SComponentIterator.hxx"
#include "SALOMEDSImpl_Study.hxx"

//============================================================================
/*! Function : constructor
 * 
 */
//============================================================================

SALOMEDSImpl_SComponentIterator::SALOMEDSImpl_SComponentIterator(const Handle(TDocStd_Document)& theDocument)
{
  _lab = theDocument->Main();
  _it.Initialize (_lab);
}

//============================================================================
/*! Function : Init
 * 
 */
//============================================================================
void SALOMEDSImpl_SComponentIterator::Init()
{ 
  _it.Initialize (_lab);
}

//============================================================================
/*! Function : More
 * 
 */
//============================================================================
bool SALOMEDSImpl_SComponentIterator::More()
{
  if (!_it.More())
    return false;
  TDF_Label L = _it.Value();
  if (SALOMEDSImpl_SComponent::IsA(L))
     return true;

  return _it.More();
}

 //============================================================================
/*! Function : Next
  */
//============================================================================
void SALOMEDSImpl_SComponentIterator::Next()
{
  _it.Next();
}


//============================================================================
/*! Function : Value
 * 
 */
//============================================================================
Handle(SALOMEDSImpl_SComponent) SALOMEDSImpl_SComponentIterator::Value()
{
  return SALOMEDSImpl_Study::SComponent(_it.Value());
}

