#ifndef _RECEIVERFACTORY_HXX_
#define _RECEIVERFACTORY_HXX_

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Comm)
#include "MultiCommException.hxx"

/*!
  This class internally builds a receiver associated with the sender given. It also performs transfert completely and clean up the objects.
  This is the only class used client side of an array.
 */
class ReceiverFactory
{
public:
  static void *getValue(SALOME::Sender_ptr sender,long &size)throw(MultiCommException);
private:
  static void *getValueOneShot(SALOME::Sender_ptr sender,long &size)throw(MultiCommException);
};

#endif

