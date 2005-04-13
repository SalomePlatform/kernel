#ifndef _RECEIVER_HXX_
#define _RECEIVER_HXX_

#include "SALOME_Comm_i.hxx"

/*! Abstract class factorizing common methods of all the receivers. All of the receivers have to inheritate from it.
 */
template<class T,class senderPtr, class senderSrv>
class Receiver
{
public:
  virtual T *getValue(long &size)=0;
  virtual ~Receiver() {}
protected:
  virtual T *getValue(long &size,senderPtr sender);
  static inline T *getLocalValue(long &size,senderSrv* servant);
  virtual T *getDistValue(long &size)=0;
};

template<class T,class senderPtr, class senderSrv>
T *Receiver<T,senderPtr,senderSrv>::getLocalValue(long &size,senderSrv* servant)
{
  const T *src=(const T *)servant->getData(size);
  long lgr=size*servant->getSizeOf();
  void *ret=new char[lgr];
  memcpy(ret,src,lgr);
  return (T *)ret;
  //return (void *)servant->getData(size);
}

template<class T,class senderPtr, class senderSrv>
T *Receiver<T,senderPtr,senderSrv>::getValue(long &size,senderPtr sender)
{
  senderSrv* data=senderSrv::find(sender);
  if(data)
    return getLocalValue(size,data);
  else
    return getDistValue(size);
}

#endif

