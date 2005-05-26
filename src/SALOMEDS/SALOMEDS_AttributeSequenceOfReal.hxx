//  File   : SALOMEDS_AttributeSequenceOfReal.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeSequenceOfReal_HeaderFile
#define SALOMEDS_AttributeSequenceOfReal_HeaderFile

#include "SALOMEDSClient_AttributeSequenceOfReal.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeSequenceOfReal.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include <vector>

class SALOMEDS_AttributeSequenceOfReal: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeSequenceOfReal
{
public:  
  SALOMEDS_AttributeSequenceOfReal(const Handle(SALOMEDSImpl_AttributeSequenceOfReal)& theAttr);
  SALOMEDS_AttributeSequenceOfReal(SALOMEDS::AttributeSequenceOfReal_ptr theAttr);
  ~SALOMEDS_AttributeSequenceOfReal();

  virtual void Assign(const std::vector<double>& other);
  virtual std::vector<double> CorbaSequence();
  virtual void Add(double value);
  virtual void Remove(int index);
  virtual void ChangeValue(int index, double value);
  virtual double Value(int index);
  virtual int Length();

};

#endif
