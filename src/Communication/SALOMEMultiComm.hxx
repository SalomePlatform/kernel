#ifndef _SALOMEMULTICOMM_HXX_
#define _SALOMEMULTICOMM_HXX_

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Comm)

#if defined WNT && defined COMMUNICATION_EXPORTS
#define COMMUNICATION_EXPORT __declspec( dllexport )
#else
#define COMMUNICATION_EXPORT
#endif

/*!
  Class is designed to ease the use of multi communication.\n
  Simply inherite from it your servant class you want to emit data with senders.
 */
class COMMUNICATION_EXPORT SALOMEMultiComm : public virtual POA_SALOME::MultiCommClass {
protected:
  SALOME::TypeOfCommunication _type;
public:
  SALOMEMultiComm();
  SALOMEMultiComm(SALOME::TypeOfCommunication type);
  virtual void setProtocol(SALOME::TypeOfCommunication type);
  SALOME::TypeOfCommunication getProtocol() const;
};

#endif
