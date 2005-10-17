// File:	SALOMEDSImpl_ChildNodeIterator.cxx
// Created:	Wed Jan 26 16:43:08 2000
// Author:	Denis PASCAL
//		<dp@dingox.paris1.matra-dtv.fr>


#include <SALOMEDSImpl_ChildNodeIterator.hxx>

#define ChildNodeIterator_UpToBrother \
{ \
    while (!myNode.IsNull() && (myNode->Depth() > myFirstLevel) && myNode->myNext == NULL) \
      myNode = myNode->myFather; \
	if (!myNode.IsNull() && (myNode->Depth() > myFirstLevel) && myNode->myFather != NULL) \
	  myNode = myNode->myNext; \
	else \
	  myNode = NULL; \
}

//=======================================================================
//function : SALOMEDSImpl_ChildNodeIterator
//purpose  : 
//=======================================================================
SALOMEDSImpl_ChildNodeIterator::SALOMEDSImpl_ChildNodeIterator()
     : myFirstLevel(0)
{}

//=======================================================================
//function : SALOMEDSImpl_ChildNodeIterator
//purpose  : 
//=======================================================================

SALOMEDSImpl_ChildNodeIterator::SALOMEDSImpl_ChildNodeIterator (const Handle(SALOMEDSImpl_AttributeTreeNode)& aNode,
								const Standard_Boolean allLevels)
: myNode(aNode->myFirst),
  myFirstLevel(allLevels ? aNode->Depth() : -1)
{}

//=======================================================================
//function : Initialize
//purpose  : 
//=======================================================================

void SALOMEDSImpl_ChildNodeIterator::Initialize(const Handle(SALOMEDSImpl_AttributeTreeNode)& aNode,
						const Standard_Boolean allLevels)
{
  myNode = aNode->myFirst;
  myFirstLevel = allLevels ? aNode->Depth() : -1;
}

//=======================================================================
//function : Next
//purpose  : 
//=======================================================================

void SALOMEDSImpl_ChildNodeIterator::Next() 
{
  if (myFirstLevel == -1) {
    myNode = myNode->myNext;
  }
  else {
    if (myNode->myFirst != NULL) myNode = myNode->myFirst;
    else ChildNodeIterator_UpToBrother;
  }
}

//=======================================================================
//function : NextBrother
//purpose  : 
//=======================================================================

void SALOMEDSImpl_ChildNodeIterator::NextBrother() 
{
  if (myNode->myNext != NULL) myNode = myNode->myNext;
  else ChildNodeIterator_UpToBrother;
}
