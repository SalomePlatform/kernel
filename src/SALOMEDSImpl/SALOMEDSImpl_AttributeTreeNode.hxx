//  File   : SALOMEDSImpl_AttributeTreeNode.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeTreeNode_HeaderFile
#define _SALOMEDSImpl_AttributeTreeNode_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <Standard_GUID.hxx>     
#include <TDF_Label.hxx>
#include <TDF_AttributeDelta.hxx>
#include <TCollection_AsciiString.hxx>
#include "SALOMEDSImpl_GenericAttribute.hxx"

class Handle_TDF_DataSet;
class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);
class SALOMEDSImpl_ChildNodeIterator;

DEFINE_STANDARD_HANDLE( SALOMEDSImpl_AttributeTreeNode, SALOMEDSImpl_GenericAttribute )

class SALOMEDSImpl_AttributeTreeNode : public SALOMEDSImpl_GenericAttribute 
{
public:

Standard_EXPORT const static Standard_GUID& GetDefaultTreeID();
Standard_EXPORT static  Handle_SALOMEDSImpl_AttributeTreeNode Set(const TDF_Label& L, const Standard_GUID& ID);

Standard_EXPORT SALOMEDSImpl_AttributeTreeNode();
Standard_EXPORT void SetFather(const Handle(SALOMEDSImpl_AttributeTreeNode)& value);
Standard_EXPORT void SetPrevious(const Handle(SALOMEDSImpl_AttributeTreeNode)& value);
Standard_EXPORT void SetNext(const Handle(SALOMEDSImpl_AttributeTreeNode)& value);
Standard_EXPORT void SetFirst(const Handle(SALOMEDSImpl_AttributeTreeNode)& value);
Standard_EXPORT void SetTreeID(const Standard_GUID& value);

Standard_EXPORT Standard_Boolean Append(const Handle(SALOMEDSImpl_AttributeTreeNode)& value);  
Standard_EXPORT Standard_Boolean Prepend(const Handle(SALOMEDSImpl_AttributeTreeNode)& value);
Standard_EXPORT Standard_Boolean Remove();
Standard_EXPORT Standard_Boolean InsertBefore(const Handle(SALOMEDSImpl_AttributeTreeNode)& value);
Standard_EXPORT Standard_Boolean InsertAfter(const Handle(SALOMEDSImpl_AttributeTreeNode)& value);

Standard_EXPORT Handle(SALOMEDSImpl_AttributeTreeNode) GetFather() { return myFather; }
Standard_EXPORT Handle(SALOMEDSImpl_AttributeTreeNode) GetPrevious() { return myPrevious; }
Standard_EXPORT Handle(SALOMEDSImpl_AttributeTreeNode) GetNext() { return myNext; }
Standard_EXPORT Handle(SALOMEDSImpl_AttributeTreeNode) GetFirst() { return myFirst; }

Standard_EXPORT Standard_Boolean HasFather() { return (!myFather.IsNull()); }
Standard_EXPORT Standard_Boolean HasPrevious() { return (!myPrevious.IsNull()); }
Standard_EXPORT Standard_Boolean HasNext() {return (!myNext.IsNull()); }
Standard_EXPORT Standard_Boolean HasFirst() { return (!myFirst.IsNull()); }
Standard_EXPORT const Standard_GUID& GetTreeID() { return ID(); }
Standard_EXPORT Standard_Integer Depth() const;
Standard_EXPORT Standard_Boolean IsRoot() const ;
Standard_EXPORT Handle(SALOMEDSImpl_AttributeTreeNode) Root() const;
Standard_EXPORT Standard_Boolean IsAscendant (const Handle(SALOMEDSImpl_AttributeTreeNode)& ofTN) const;
Standard_EXPORT Standard_Boolean IsDescendant(const Handle(SALOMEDSImpl_AttributeTreeNode)& value) const;
Standard_EXPORT Standard_Boolean IsFather(const Handle(SALOMEDSImpl_AttributeTreeNode)& value) const;
Standard_EXPORT Standard_Boolean IsChild(const Handle(SALOMEDSImpl_AttributeTreeNode)& value) const;

Standard_EXPORT virtual TCollection_AsciiString Save();
Standard_EXPORT virtual void Load(const TCollection_AsciiString&); 
Standard_EXPORT virtual TCollection_AsciiString Type(); 

Standard_EXPORT virtual  void AfterAddition() ;
Standard_EXPORT virtual  void BeforeForget() ;
Standard_EXPORT virtual  void AfterResume() ;
Standard_EXPORT virtual  Standard_Boolean BeforeUndo(const Handle(TDF_AttributeDelta)& anAttDelta,
						     const Standard_Boolean forceIt = Standard_False) ;
Standard_EXPORT virtual  Standard_Boolean AfterUndo(const Handle(TDF_AttributeDelta)& anAttDelta,
						    const Standard_Boolean forceIt = Standard_False) ;   
Standard_EXPORT  const Standard_GUID& ID() const;
Standard_EXPORT  void Restore(const Handle(TDF_Attribute)& with) ;
Standard_EXPORT  void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;
Standard_EXPORT  Handle(TDF_Attribute) NewEmpty() const;
Standard_EXPORT  void References(const Handle(TDF_DataSet)& aDataSet) const;  
Standard_EXPORT ~SALOMEDSImpl_AttributeTreeNode() {}

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_AttributeTreeNode )

private:

Standard_GUID myTreeID; 
Handle(SALOMEDSImpl_AttributeTreeNode) myFather;
Handle(SALOMEDSImpl_AttributeTreeNode) myPrevious;
Handle(SALOMEDSImpl_AttributeTreeNode) myNext;
Handle(SALOMEDSImpl_AttributeTreeNode) myFirst;

friend class SALOMEDSImpl_ChildNodeIterator;

};

#endif
