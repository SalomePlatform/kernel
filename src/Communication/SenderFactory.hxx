#ifndef _SENDERFACTORY_HXX_
#define _SENDERFACTORY_HXX_

#include "MultiCommException.hxx"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Comm)

class SALOMEMultiComm;

class SALOME_Sender_i;

/*!
  This class implements the factory pattern of GoF by making a sender by giving an array and a communicator.It completely hides the type of sender from the user.
 */
class SenderFactory
{
public:
  static SALOME::Sender_ptr buildSender(SALOMEMultiComm &multiCommunicator,const double *tab,long lgr) throw(MultiCommException);
  static SALOME::Sender_ptr buildSender(SALOMEMultiComm &multiCommunicator,const int *tab,long lgr) throw(MultiCommException);
  static SALOME::Sender_ptr buildSender(SALOME::TypeOfCommunication NewType,SALOME_Sender_i *src);
};

#endif

