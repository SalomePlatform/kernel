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

//  File   : SALOMEDS_SimanStudy_i.hxx
//  Author : Mikhail PONIKAROV
//  Module : SALOME
//
#ifndef __SALOMEDS_SIMANSTUDY_I_H__
#define __SALOMEDS_SIMANSTUDY_I_H__

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

//SALOMEDS headers
#include "SALOMEDS_Study_i.hxx"

class SimanIO_Configuration;
class SALOME_NamingService;

class Standard_EXPORT SALOMEDS_SimanStudy_i: public POA_SALOMEDS::SimanStudy
{
private:
  CORBA::ORB_var                 _orb;
  SALOME_NamingService*          _name_service;
  //SALOMEDSImpl_SimanStudy*       _impl;  
  SALOMEDSImpl_Study*          _study;

  std::string              _studyId; // SIMAN ID
  std::string              _scenarioId; // SIMAN ID
  std::string              _userId; // SIMAN ID
      
  SimanIO_Configuration* _checkedOut; // pointer to information about checked out data                                                                        std::map<int, std::map<std::string, int> > _filesId; // map from document id to component identifiers to SIMAN file ID
  std::map<int, std::map<std::string, int> > _filesId; // map from document id to component identifiers to SIMAN file ID
         
public:

  //! constructor create global Siman stduy (is called in StudyManager only)
  SALOMEDS_SimanStudy_i(/*SALOMEDSImpl_SimanStudy*,*/ CORBA::ORB_ptr);
  
  //! standard destructor
  virtual ~SALOMEDS_SimanStudy_i(); 
  
   /*! \brief Get data from SIMAN and put it to the given study

     This method is called for just created study to make it referenced
     by this Siman study. Also it fills this study by the checked out
     data from SIMAN server.
     Warning: StudyID, ScenarioID and User Id must be set before calling of this method!
    
     \param theTarget referenced study for checking out
  */
  virtual void CheckOut(SALOMEDS::Study_ptr theTarget);

  /*! \brief Get data from all modules and calls the corresponding SimanIO methods to store it.
    
    It works only with already checked out study
   */
  virtual void CheckIn(const char* themoduleName);
  
  /*! \brief Returns the %Study with checked out data

    If study is null, there is no such study in this application instance, or Salome was
    launched without siman option.
  */
  virtual SALOMEDS::Study_ptr getReferencedStudy();
  
  /*! \brief The ID of the study in SIMAN server
  
    Returns the Id of SIMAN study, used for CheckOut before.
  */
  virtual char* StudyId();

  /*! \brief The ID of the study in SIMAN server
  
    Defines the Id of SIMAN study, used for CheckOut after.
  */
  virtual void StudyId(const char* theId);

  /*! \brief The ID of the scenario in SIMAN server
  
    Returns the Id of SIMAN scenario, used for CheckOut before.
  */
  virtual char* ScenarioId();

  /*! \brief The ID of the scenario in SIMAN server
  
    Defines the Id of SIMAN scenario, used for CheckOut after.
  */
  virtual void ScenarioId(const char* theId);

  /*! \brief The ID of the user in SIMAN server
  
    Returns the Id of SIMAN user, used for CheckOut before.
  */
  virtual char* UserId();

  /*! \brief The ID of the user in SIMAN server
  
    Defines the Id of SIMAN user, used for CheckOut after.
  */
  virtual void UserId(const char* theId);
  
  //virtual CORBA::LongLong GetLocalImpl(const char* theHostname, CORBA::Long thePID, CORBA::Boolean& isLocal);

  static SALOMEDS_SimanStudy_i* GetSimanServant(CORBA::ORB_ptr orb);
};
#endif
