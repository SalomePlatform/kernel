//  File      : SALOMEDS_UseCaseIterator_i.hxx
//  Created   : Mon Oct 7 16:20:12 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002

#ifndef __SALOMEDS_USECASEITERATOR_I_H__
#define __SALOMEDS_USECASEITERATOR_I_H__

// std C++ headers
#include <iostream.h>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)



// Cascade headers
#include <TDF_ChildIterator.hxx>
#include <TDataStd_ChildNodeIterator.hxx>
#include <TDataStd_TreeNode.hxx>
#include <Standard_GUID.hxx>
#include <stdio.h>

class SALOMEDS_UseCaseIterator_i: public POA_SALOMEDS::UseCaseIterator,
				public PortableServer::RefCountServantBase {
private:
  Standard_GUID                 _guid;
  Standard_Boolean              _levels;
  CORBA::ORB_ptr                _orb;
  Handle(TDataStd_TreeNode)     _node;
  TDataStd_ChildNodeIterator    _it;

public:

  //! standard constructor  
  SALOMEDS_UseCaseIterator_i(const TDF_Label& theLabel, 
			     const Standard_GUID& theGUID, 
			     const Standard_Boolean allLevels,
			     CORBA::ORB_ptr);
  
  //! standard destructor
  ~SALOMEDS_UseCaseIterator_i();
  
  virtual void Init(CORBA::Boolean);
  virtual CORBA::Boolean More();
  virtual void Next();
  virtual SALOMEDS::SObject_ptr Value();
};
#endif
