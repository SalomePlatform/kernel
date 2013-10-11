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

//  File   : SALOMEDSClient_SimanStudy.hxx
//  Author : Mikhail PONIKAROV
//  Module : SALOME
//
#ifndef __SALOMEDSClient_SIMANSTUDY_H__
#define __SALOMEDSClient_SIMANSTUDY_H__

#include <vector>
#include <string>

#include "SALOMEDSClient_definitions.hxx"
#include "SALOMEDSClient_Study.hxx"
#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOMEDS)

class SALOMEDSClient_SimanStudy
{

public:
  virtual ~SALOMEDSClient_SimanStudy() {}

  virtual void CheckOut(const _PTR(Study) theTarget) = 0;
  virtual void CheckIn(const std::string theModuleName) = 0;
  virtual _PTR(Study) getReferencedStudy() = 0;
  virtual std::string StudyId() = 0;
  virtual void StudyId(const std::string theId) = 0;
  virtual std::string ScenarioId() = 0;
  virtual void ScenarioId(const std::string theId) = 0;
  virtual std::string UserId() = 0;
  virtual void UserId(const std::string theId) = 0;
};

#endif
