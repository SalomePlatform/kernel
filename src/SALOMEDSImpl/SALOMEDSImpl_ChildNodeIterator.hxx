
#ifndef _SALOMEDSImpl_ChildNodeIterator_HeaderFile
#define _SALOMEDSImpl_ChildNodeIterator_HeaderFile

#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif

#include "SALOMEDSImpl_AttributeTreeNode.hxx"


#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

class SALOMEDSImpl_ChildNodeIterator  
{

public:

Standard_EXPORT SALOMEDSImpl_ChildNodeIterator();
Standard_EXPORT SALOMEDSImpl_ChildNodeIterator(const Handle(SALOMEDSImpl_AttributeTreeNode)& aTreeNode,
					       const Standard_Boolean allLevels = Standard_False);
Standard_EXPORT void Initialize(const Handle(SALOMEDSImpl_AttributeTreeNode)& aTreeNode,
				const Standard_Boolean allLevels = Standard_False) ;
Standard_EXPORT Standard_Boolean More() const { return !myNode.IsNull(); }
Standard_EXPORT void Next() ;
Standard_EXPORT void NextBrother() ;
Standard_EXPORT Handle_SALOMEDSImpl_AttributeTreeNode Value() const { return myNode; }

private: 

Handle_SALOMEDSImpl_AttributeTreeNode myNode;
Standard_Integer myFirstLevel;

};


#endif
