#ifndef _SALOMEMULTICOMM_HXX_
#define _SALOMEMULTICOMM_HXX_

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Comm)

/*!
  Class is designed to ease the use of multi communication.\n
  Simply inherite from it your servant class you want to emit data with senders.
 */
class SALOMEMultiComm : public virtual POA_SALOME::MultiCommClass {
protected:
  SALOME::TypeOfCommunication _type;
public:
  SALOMEMultiComm();
  SALOMEMultiComm(SALOME::TypeOfCommunication type);
  virtual void setProtocol(SALOME::TypeOfCommunication type);
  SALOME::TypeOfCommunication getProtocol() const;
};

#endif
