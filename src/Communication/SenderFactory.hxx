#ifndef _SENDERFACTORY_HXX_
#define _SENDERFACTORY_HXX_

#include "MultiCommException.hxx"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Comm)

class SALOMEMultiComm;

class SALOME_SenderDouble_i;
class SALOME_SenderInt_i;

/*!
  This class implements the factory pattern of GoF by making a sender by giving an array and a communicator.It completely hides the type of sender from the user.
 */
class SenderFactory
{
public:
  static SALOME::SenderDouble_ptr buildSender(SALOMEMultiComm &multiCommunicator,const double *tab,long lgr,bool ownTab=false) throw(MultiCommException);
  static SALOME::SenderInt_ptr buildSender(SALOMEMultiComm &multiCommunicator,const int *tab,long lgr,bool ownTab=false) throw(MultiCommException);
  static SALOME::SenderDouble_ptr buildSender(SALOME::TypeOfCommunication NewType,SALOME_SenderDouble_i *src);
  static SALOME::SenderInt_ptr buildSender(SALOME::TypeOfCommunication NewType,SALOME_SenderInt_i *src);
};

template<class T>
struct mapCppSender {
  typedef T SenderVarType;
};

template<>
struct mapCppSender<int>
{
  typedef SALOME::SenderInt_var SenderVarType;
};

template<>
struct mapCppSender<double>
{
  typedef SALOME::SenderDouble_var SenderVarType;
};

#endif

