//  File   : SALOMEDS_AttributeSequenceOfInteger.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeSequenceOfInteger.hxx"

#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TColStd_HSequenceOfInteger.hxx>

SALOMEDS_AttributeSequenceOfInteger
::SALOMEDS_AttributeSequenceOfInteger(const Handle(SALOMEDSImpl_AttributeSequenceOfInteger)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeSequenceOfInteger::SALOMEDS_AttributeSequenceOfInteger(SALOMEDS::AttributeSequenceOfInteger_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeSequenceOfInteger::~SALOMEDS_AttributeSequenceOfInteger()
{}

void SALOMEDS_AttributeSequenceOfInteger::Assign(const std::vector<int>& other)
{
  int i, aLength = other.size();
  if(_isLocal) {
    Handle(TColStd_HSequenceOfInteger) aSeq = new TColStd_HSequenceOfInteger;
    for(i = 0; i < aLength; i++) aSeq->Append(other[i]);
    Handle(SALOMEDSImpl_AttributeSequenceOfInteger)::DownCast(_local_impl)->Assign(aSeq);
  }
  else {
    SALOMEDS::LongSeq_var aSeq = new SALOMEDS::LongSeq();
    aSeq->length(aLength);
    for(i = 0; i < aLength; i++) aSeq[i] = other[i];
    SALOMEDS::AttributeSequenceOfInteger::_narrow(_corba_impl)->Assign(aSeq);
  }
}

std::vector<int> SALOMEDS_AttributeSequenceOfInteger::CorbaSequence()
{
  std::vector<int> aVector;
  int i, aLength;
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeSequenceOfInteger) aSeqAttr;
    aSeqAttr = Handle(SALOMEDSImpl_AttributeSequenceOfInteger)::DownCast(_local_impl);
    aLength = aSeqAttr->Length();
    for(i = 1; i <=aLength; i++) aVector.push_back(aSeqAttr->Value(i));
  }
  else {
    SALOMEDS::AttributeSequenceOfInteger_var aSeqAttr = SALOMEDS::AttributeSequenceOfInteger::_narrow(_corba_impl);
    aLength = aSeqAttr->Length();
    for(i = 1; i <=aLength; i++) aVector.push_back(aSeqAttr->Value(i));
  }
  return aVector;
}
 
void SALOMEDS_AttributeSequenceOfInteger::Add(int value)
{
  if(_isLocal) Handle(SALOMEDSImpl_AttributeSequenceOfInteger)::DownCast(_local_impl)->Add(value);
  else SALOMEDS::AttributeSequenceOfInteger::_narrow(_corba_impl)->Add(value);
}

void SALOMEDS_AttributeSequenceOfInteger::Remove(int index)
{
  if(_isLocal) Handle(SALOMEDSImpl_AttributeSequenceOfInteger)::DownCast(_local_impl)->Remove(index);
  else SALOMEDS::AttributeSequenceOfInteger::_narrow(_corba_impl)->Remove(index);
}
 
void SALOMEDS_AttributeSequenceOfInteger::ChangeValue(int index, int value)
{
  if(_isLocal) Handle(SALOMEDSImpl_AttributeSequenceOfInteger)::DownCast(_local_impl)->ChangeValue(index, value);
  else SALOMEDS::AttributeSequenceOfInteger::_narrow(_corba_impl)->ChangeValue(index, value);
}
 
int SALOMEDS_AttributeSequenceOfInteger::Value(int index)
{
  int aValue;
  if(_isLocal) aValue = Handle(SALOMEDSImpl_AttributeSequenceOfInteger)::DownCast(_local_impl)->Value(index);
  else aValue = SALOMEDS::AttributeSequenceOfInteger::_narrow(_corba_impl)->Value(index);
  return aValue;
}
 
int SALOMEDS_AttributeSequenceOfInteger::Length()
{
  int aValue;
  if(_isLocal) aValue = Handle(SALOMEDSImpl_AttributeSequenceOfInteger)::DownCast(_local_impl)-> Length();
  else aValue = SALOMEDS::AttributeSequenceOfInteger::_narrow(_corba_impl)-> Length();
  return aValue;
}
