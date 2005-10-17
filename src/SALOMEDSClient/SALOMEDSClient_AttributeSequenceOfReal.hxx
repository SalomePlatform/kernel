//  File   : SALOMEDSClient_AttributeSequenceOfSequenceOfReal.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDSClient_AttributeSequenceOfSequenceOfReal_HeaderFile
#define SALOMEDSClient_AttributeSequenceOfSequenceOfReal_HeaderFile

#include <vector>
#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 

class SALOMEDSClient_AttributeSequenceOfReal: public virtual SALOMEDSClient_GenericAttribute
{
public:

  virtual void Assign(const std::vector<double>& other) = 0;
  virtual std::vector<double> CorbaSequence() = 0;
  virtual void Add(double value) = 0;
  virtual void Remove(int index) = 0;
  virtual void ChangeValue(int index, double value) = 0;
  virtual double Value(int index) = 0;
  virtual int Length() = 0;

};



#endif
