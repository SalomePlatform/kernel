#ifndef __SALOME_MATRIX_I_HXX__
#define __SALOME_MATRIX_I_HXX__

#include <string>
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Comm)
#include "SALOMEMultiComm.hxx"

class SALOME_Matrix_i : public virtual POA_SALOME::Matrix,
			public PortableServer::RefCountServantBase {
private:
  const double *_tabToSend;
  int _nbOfRow;
  int _nbOfColumn;
  bool _ownTabToSend;
  SALOMEMultiComm _type;
protected:
  ~SALOME_Matrix_i();
public:
  SALOME_Matrix_i(const SALOMEMultiComm& multiCommunicator,const double *tabToSend,int nbOfRow,int nbOfColumn,bool ownTabToSend=false);
  SALOME::SenderDouble_ptr getData();
  CORBA::Long getSizeOfColumn();
  void release();
};

#endif
