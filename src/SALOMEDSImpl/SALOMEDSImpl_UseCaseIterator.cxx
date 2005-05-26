//  File   : SALOMEDSImpl_UseCaseIterator.cxx
//  Author : Serge RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDSImpl_UseCaseIterator.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_Study.hxx"

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_UseCaseIterator, MMgt_TShared )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_UseCaseIterator, MMgt_TShared )

//============================================================================
/*! Function : constructor
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_UseCaseIterator::SALOMEDSImpl_UseCaseIterator(const TDF_Label& theLabel, 
						           const Standard_GUID& theGUID,
						           const bool allLevels)
:_guid(theGUID), _levels(allLevels)
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
SALOMEDSImpl_UseCaseIterator::~SALOMEDSImpl_UseCaseIterator()
{
}

//============================================================================
/*! Function :Init
 * 
 */
//============================================================================
void SALOMEDSImpl_UseCaseIterator::Init(bool allLevels)
{ 
  _it.Initialize (_node, allLevels);
}

//============================================================================
/*! Function : More
 * 
 */
//============================================================================
bool SALOMEDSImpl_UseCaseIterator::More()
{
  return _it.More();
}

 //============================================================================
/*! Function : Next
 * 
 */
//============================================================================
void SALOMEDSImpl_UseCaseIterator::Next()
{
  _it.Next();
}


//============================================================================
/*! Function :
 *  Purpose  :
 */
//============================================================================

Handle(SALOMEDSImpl_SObject) SALOMEDSImpl_UseCaseIterator::Value()
{
  TDF_Label L = _it.Value()->Label();
  return SALOMEDSImpl_Study::SObject(L);
}

