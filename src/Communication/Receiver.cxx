#include "Receiver.hxx"
#include <string.h>

/*!
  return a deep copy of the array contained in the servant.
 */
void *Receiver::getLocalValue(long &size,SALOME_Sender_i* servant)
{
  const void *src=servant->getData(size);
  long lgr=size*servant->getSizeOf();
  void *ret=new char[lgr];
  memcpy(ret,src,lgr);
  return ret;
  //return (void *)servant->getData(size);
}

void *Receiver::getValue(long &size,SALOME::Sender_ptr sender)
{
  SALOME_Sender_i* data=SALOME_Sender_i::find(sender);
  if(data)
    return getLocalValue(size,data);
  else
    return getDistValue(size);
}

