//=============================================================================
// File      : SALOME_MPILifeCycleCORBA.hxx
// Created   : mar jui 03 14:55:45 CEST 2003
// Author    : Bernard SECHER, CEA
// Project   : SALOME
// Copyright : CEA 2003
// $Header$
//=============================================================================

#ifndef _SALOME_MPILIFECYCLECORBA_HXX_
#define _SALOME_MPILIFECYCLECORBA_HXX_

#include <stdlib.h>
#include <unistd.h>
#include <string>

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_MPIContainer)
#include "SALOME_LifeCycleCORBA.hxx"

class SALOME_MPILifeCycleCORBA : public SALOME_LifeCycleCORBA
{
public:
  SALOME_MPILifeCycleCORBA();
  SALOME_MPILifeCycleCORBA(SALOME_NamingService *ns);
  virtual ~SALOME_MPILifeCycleCORBA();

//   Engines::Component_var FindOrLoad_MPIComponent(const char *MPIcontainerName,
// 						 const char *MPIcomponentName,
// 						 const char *implementationPath,
// 						 const int nbproc);
  Engines::Component_var FindOrLoad_MPIComponent(const char *MPIcontainerName,
						 const char *MPIcomponentName,
						 const int nbproc);
protected:
  Engines::MPIContainer_var _MPIFactoryServer ;

private:
  Engines::MPIContainer_var FindOrStartMPIContainer(const string theComputer ,
						    const string theMPIContainerRoot,
						    const int nbproc) ;

} ;

#endif
