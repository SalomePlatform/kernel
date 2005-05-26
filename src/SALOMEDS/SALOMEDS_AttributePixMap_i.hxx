//  File   : SALOMEDS_AttributePixMap_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributePixMap_i_HeaderFile
#define SALOMEDS_AttributePixMap_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributePixMap.hxx"

class SALOMEDS_AttributePixMap_i: public virtual POA_SALOMEDS::AttributePixMap,
				  public virtual SALOMEDS_GenericAttribute_i 
{
public:
  
  SALOMEDS_AttributePixMap_i(const Handle(SALOMEDSImpl_AttributePixMap)& theAttr, CORBA::ORB_ptr orb) 
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {};

  ~SALOMEDS_AttributePixMap_i() {};

  CORBA::Boolean HasPixMap();
  char* GetPixMap();
  void SetPixMap(const char* value);

};


#endif
