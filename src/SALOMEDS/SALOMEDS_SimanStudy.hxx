// Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

//  File   : SALOMEDS_SimanStudy.hxx
//  Author : Mikhail PONIKAROV
//  Module : SALOME
//
#ifndef __SALOMEDS_SIMANSTUDY_H__
#define __SALOMEDS_SIMANSTUDY_H__

#include <vector>
#include <string>

#include <DF_definitions.hxx>
#include "SALOMEDSClient.hxx"
//#include "SALOMEDSImpl_SimanStudy.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

class Standard_EXPORT SALOMEDS_SimanStudy: public SALOMEDSClient_SimanStudy
{

private:
  // bool                       _isLocal;
  //SALOMEDSImpl_SimanStudy*   _local_impl; // MPV: no local implementation yet: siman operations works only with CORBA interface
  SALOMEDS::SimanStudy_var   _corba_impl;
  CORBA::ORB_var             _orb;

public:

  //SALOMEDS_SimanStudy(SALOMEDSImpl_SimanStudy* theStudy);
  SALOMEDS_SimanStudy(SALOMEDS::SimanStudy_ptr theStudy);
  ~SALOMEDS_SimanStudy();

  virtual void CheckOut(const _PTR(Study) theTarget);
  virtual void CheckIn(const std::string theModuleName);
  virtual _PTR(Study) getReferencedStudy() ;
  virtual std::string StudyId();
  virtual void StudyId(const std::string theId);
  virtual std::string ScenarioId();
  virtual void ScenarioId(const std::string theId);
  virtual std::string UserId();
  virtual void UserId(const std::string theId);
                
private:
  void init_orb();

};
#endif
