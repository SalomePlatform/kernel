//  File   : SALOMEDSClient_AttributePythonObject.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDSClient_AttributePythonObject_HeaderFile
#define SALOMEDSClient_AttributePythonObject_HeaderFile

#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 
#include <string> 

class SALOMEDSClient_AttributePythonObject: public virtual SALOMEDSClient_GenericAttribute
{
public:

  virtual void SetObject(const std::string& theSequence, bool IsScript) = 0;
  virtual std::string GetObject() = 0;
  virtual bool IsScript() = 0;

};




#endif
