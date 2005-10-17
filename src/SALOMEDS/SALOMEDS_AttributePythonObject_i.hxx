//  File   : SALOMEDS_AttributePythonObject_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributePythonObject_i_HeaderFile
#define SALOMEDS_AttributePythonObject_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributePythonObject.hxx"

class SALOMEDS_AttributePythonObject_i: public virtual POA_SALOMEDS::AttributePythonObject,
                                        public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributePythonObject_i(const Handle(SALOMEDSImpl_AttributePythonObject)& theAttr, CORBA::ORB_ptr orb) 
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {}; 

  ~SALOMEDS_AttributePythonObject_i() {};

  virtual void SetObject(const char* theSequence, CORBA::Boolean IsScript);
  virtual char* GetObject();
  virtual CORBA::Boolean IsScript();

};




#endif
