#ifndef _RECEIVER_HXX_
#define _RECEIVER_HXX_

#include "SALOME_Comm_i.hxx"

/*! Abstract class factorizing common methods of all the receivers. All of the receivers have to inheritate from it.
 */
class Receiver
{
public:
  virtual void *getValue(long &size)=0;
  virtual ~Receiver() {}
protected:
  virtual void *getValue(long &size,SALOME::Sender_ptr sender);
  static inline void *getLocalValue(long &size,SALOME_Sender_i* servant);
  virtual void *getDistValue(long &size)=0;
};

#endif

