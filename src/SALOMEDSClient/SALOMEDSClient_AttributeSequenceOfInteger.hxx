//  File   : SALOMEDSClient_AttributeSequenceOfInteger.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDSClient_AttributeSequenceOfInteger_HeaderFile
#define SALOMEDSClient_AttributeSequenceOfInteger_HeaderFile

#include <vector>
#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 

class SALOMEDSClient_AttributeSequenceOfInteger: public virtual SALOMEDSClient_GenericAttribute
{    
public:
  
  virtual void Assign(const std::vector<int>& other) = 0;
  virtual std::vector<int> CorbaSequence() = 0;
  virtual void Add(int value) = 0;
  virtual void Remove(int index) = 0;
  virtual void ChangeValue(int index, int value) = 0;
  virtual int Value(int index) = 0;
  virtual int Length() = 0;

};

#endif
