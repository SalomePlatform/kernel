#ifndef _SALOME_POBJECT_I_H_
#define _SALOME_POBJECT_I_H_ 

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(MPIObject)

class MPIObject_i: public POA_Engines::MPIObject
{
 public:
  MPIObject_i();
  MPIObject_i(int nbproc, int numproc);
  ~MPIObject_i();
    
  Engines::IORTab* tior();
  void tior(const Engines::IORTab& ior);

 protected:
  // Numero du processus
  int _numproc;
  // Nombre de processus
  int _nbproc;
  // IOR des objets paralleles sur tous les process mpi
  Engines::IORTab* _tior;
  // Echange des IOR de l'objet entre process
  void BCastIOR(CORBA::ORB_ptr orb,Engines::MPIObject_var pobj,bool amiCont);
} ;

#endif
