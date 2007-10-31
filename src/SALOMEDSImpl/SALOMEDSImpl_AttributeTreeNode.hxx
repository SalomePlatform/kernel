// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDSImpl_AttributeTreeNode.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeTreeNode_HeaderFile
#define _SALOMEDSImpl_AttributeTreeNode_HeaderFile

#include "DF_Attribute.hxx"
#include "DF_Label.hxx"
#include <string>
#include "SALOMEDSImpl_GenericAttribute.hxx"

class SALOMEDSImpl_AttributeTreeNode : public SALOMEDSImpl_GenericAttribute 
{
public:

Standard_EXPORT const static std::string& GetDefaultTreeID();
Standard_EXPORT static SALOMEDSImpl_AttributeTreeNode* Set(const DF_Label& L, const std::string& ID);

Standard_EXPORT SALOMEDSImpl_AttributeTreeNode();
Standard_EXPORT void SetFather(const SALOMEDSImpl_AttributeTreeNode* value);
Standard_EXPORT void SetPrevious(const SALOMEDSImpl_AttributeTreeNode* value);
Standard_EXPORT void SetNext(const SALOMEDSImpl_AttributeTreeNode* value);
Standard_EXPORT void SetFirst(const SALOMEDSImpl_AttributeTreeNode* value);
Standard_EXPORT void SetTreeID(const std::string& value);

Standard_EXPORT bool Append(SALOMEDSImpl_AttributeTreeNode* value);  
Standard_EXPORT bool Prepend(SALOMEDSImpl_AttributeTreeNode* value);
Standard_EXPORT bool Remove();
Standard_EXPORT bool InsertBefore(SALOMEDSImpl_AttributeTreeNode* value);
Standard_EXPORT bool InsertAfter(SALOMEDSImpl_AttributeTreeNode* value);

Standard_EXPORT SALOMEDSImpl_AttributeTreeNode* GetFather() const { return myFather; }
Standard_EXPORT SALOMEDSImpl_AttributeTreeNode* GetPrevious() const { return myPrevious; }
Standard_EXPORT SALOMEDSImpl_AttributeTreeNode* GetNext() const { return myNext; }
Standard_EXPORT SALOMEDSImpl_AttributeTreeNode* GetFirst() const { return myFirst; }

Standard_EXPORT bool HasFather() { return (myFather); }
Standard_EXPORT bool HasPrevious() { return (myPrevious); }
Standard_EXPORT bool HasNext() {return (myNext); }
Standard_EXPORT bool HasFirst() { return (myFirst); }
Standard_EXPORT const std::string& GetTreeID() { return ID(); }
Standard_EXPORT int Depth() const;
Standard_EXPORT bool IsRoot() const ;
Standard_EXPORT SALOMEDSImpl_AttributeTreeNode* Root() const;
Standard_EXPORT bool IsAscendant (const SALOMEDSImpl_AttributeTreeNode* ofTN) const;
Standard_EXPORT bool IsDescendant(const SALOMEDSImpl_AttributeTreeNode* value) const;
Standard_EXPORT bool IsFather(const SALOMEDSImpl_AttributeTreeNode* value) const;
Standard_EXPORT bool IsChild(const SALOMEDSImpl_AttributeTreeNode* value) const;

Standard_EXPORT virtual std::string Save();
Standard_EXPORT virtual void Load(const std::string&); 
Standard_EXPORT virtual std::string Type(); 

Standard_EXPORT  const std::string& ID() const;
Standard_EXPORT  void Restore(DF_Attribute* with) ;
Standard_EXPORT  void Paste(DF_Attribute* into);
Standard_EXPORT  DF_Attribute* NewEmpty() const;
Standard_EXPORT ~SALOMEDSImpl_AttributeTreeNode() {;}

Standard_EXPORT virtual void AfterAddition();
Standard_EXPORT virtual void BeforeForget();

private:

std::string myTreeID; 
SALOMEDSImpl_AttributeTreeNode* myFather;
SALOMEDSImpl_AttributeTreeNode* myPrevious;
SALOMEDSImpl_AttributeTreeNode* myNext;
SALOMEDSImpl_AttributeTreeNode* myFirst;

friend class SALOMEDSImpl_ChildNodeIterator;

};

#endif
