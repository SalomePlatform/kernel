//=============================================================================
// File      : SALOME_LifeCycleCORBA.hxx
// Created   : jeu jui 12 14:55:45 CEST 2001
// Author    : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

#ifndef _SALOME_LIFECYCLECORBA_HXX_
#define _SALOME_LIFECYCLECORBA_HXX_

#include <stdlib.h>
#include <unistd.h>
#include <string>

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_Component)

class SALOME_NamingService;

class SALOME_LifeCycleCORBA
{
public:
  SALOME_LifeCycleCORBA();
  SALOME_LifeCycleCORBA(SALOME_NamingService *ns);
  virtual ~SALOME_LifeCycleCORBA();

  Engines::Container_var FindContainer(const char *containerName);
  Engines::Component_var FindOrLoad_Component(const char *containerName,
					      const char *componentName,
					      const char *implementationPath);
  Engines::Component_var FindOrLoad_Component(const char *containerName,
					      const char *componentName);
protected:
  SALOME_NamingService *_NS;
  Engines::Container_var _FactoryServer ;

private:
  string ContainerName( const char * aComputerContainer ,
                        string * theComputer ,
                        string * theContainer ) ;
  string ComputerPath( const char * theComputer ) ;
  Engines::Container_var FindOrStartContainer(const string aComputerContainer ,
                                              const string theComputer ,
                                              const string theContainer ) ;

} ;

#endif
