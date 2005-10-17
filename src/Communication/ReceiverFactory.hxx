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
  static double *getValue(SALOME::SenderDouble_ptr sender,long &size)throw(MultiCommException);
  static int *getValue(SALOME::SenderInt_ptr sender,long &size)throw(MultiCommException);
private:
  static double *getValueOneShot(SALOME::SenderDouble_ptr sender,long &size)throw(MultiCommException);
  static int *getValueOneShot(SALOME::SenderInt_ptr sender,long &size)throw(MultiCommException);
};

#endif

