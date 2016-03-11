// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
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
// Author: Guillaume Boulant (EDF/R&D) 

#include "SALOME_KernelServices.hxx"

namespace KERNEL {
  
  /**
   * This function returns a static reference to the orb. The orb can
   * be used for example to initialize CORBA variables or to serialize
   * and unserialize the CORBA objet to/from an IOR string.
   */
  CORBA::ORB_ptr getORB() {
    static CORBA::ORB_ptr orb;
    if(CORBA::is_nil(orb)){
      int argc=0;
      orb = CORBA::ORB_init(argc,0);
    }
    return orb;
  }
  
  /**
   * This function returns a static reference to the SALOME naming service.
   */
  SALOME_NamingService * getNamingService() {
    static SALOME_NamingService * namingService;
    if ( namingService == NULL ) {
      namingService = new SALOME_NamingService(getORB());
    }
    return namingService;
  }
  
  /**
   * This function returns a static reference to the SALOME life cycle CORBA.
   */
  SALOME_LifeCycleCORBA * getLifeCycleCORBA() {
    static SALOME_LifeCycleCORBA * lifeCycleCORBA;
    if ( lifeCycleCORBA == NULL ) {
      SALOME_NamingService *aNamingService = getNamingService();
      lifeCycleCORBA = new SALOME_LifeCycleCORBA(aNamingService);
    }
    return lifeCycleCORBA;
  }


  /**
   * This returns a static reference to the SALOME study manager. The
   * study manager can be used to retrieve a study or to get
   * informations about a study.
   */
  SALOMEDS::StudyManager_ptr getStudyManager() {
    static SALOMEDS::StudyManager_ptr aStudyManager;
    if(CORBA::is_nil(aStudyManager)){
      SALOME_NamingService *aNamingService = getNamingService();
      CORBA::Object_ptr anObject = aNamingService->Resolve("/myStudyManager");
      aStudyManager = SALOMEDS::StudyManager::_narrow(anObject);
    }
    return aStudyManager;
  }

  /**
   * This returns a static reference to the SALOME session. The
   * SALOME session can be used to retrieve some objects of the
   * current session, as the SALOME study.
   */
  SALOME::Session_ptr getSalomeSession() {
    static SALOME::Session_ptr salomeSession;
    if(CORBA::is_nil(salomeSession)){
      SALOME_NamingService *aNamingService = getNamingService();
      CORBA::Object_ptr obj = aNamingService->Resolve("/Kernel/Session");
      salomeSession = SALOME::Session::_narrow(obj);
    }
    return salomeSession;
  }

  /**
   * This returns a static reference to the SALOME launcher. The
   * SALOME launcher can be used to schedule jobs, local or remote,
   * using a batch system or not (see SALOME documentation).
   */
  Engines::SalomeLauncher_ptr getSalomeLauncher() {
    //LOG("KERNEL_services::getSalomeLauncher()");
    static Engines::SalomeLauncher_ptr salomeLauncher;
    if(CORBA::is_nil(salomeLauncher)){
      //LOG("KERNEL_services::getSalomeLauncher(): creating the static instance");
      SALOME_NamingService *aNamingService = getNamingService();
      CORBA::Object_ptr obj = aNamingService->Resolve("/SalomeLauncher");
      salomeLauncher = Engines::SalomeLauncher::_narrow(obj);
    }
    return salomeLauncher;
  }

  Engines::ResourcesManager_ptr getResourcesManager() {
    static Engines::ResourcesManager_ptr resourcesManager;
    if(CORBA::is_nil(resourcesManager)){
      SALOME_NamingService *aNamingService = getNamingService();
      CORBA::Object_ptr obj = aNamingService->Resolve("/ResourcesManager");
      resourcesManager = Engines::ResourcesManager::_narrow(obj);
    }
    return resourcesManager;
  }

  /**
   * This returns the study with the specified id if it's defined in
   * the SALOME study manager. Returns null otherwise.
   * Please not that it is just a shortcut, and you may prefer use
   * directly the study manager:
   *    KERNEL::getStudyManager()->GetStudyByID(aStudyId)
   */
  SALOMEDS::Study_ptr getStudyById(int aStudyId) {
    if ( aStudyId < 0 ) {
      INFOS("ERR: trying to get a study with ID<0");
      return SALOMEDS::Study::_nil();
    }
    return getStudyManager()->GetStudyByID(aStudyId);
  }

  int getStudyId(SALOMEDS::Study_ptr study) {
    if( CORBA::is_nil(study) ) return -1;
    return study->StudyId();
  }


  /**
   * This function retrieve the CORBA object reference from the study
   * object wrapping it.
   */
  CORBA::Object_ptr SObjectToObject(SALOMEDS::SObject_ptr theSObject) {

    SALOMEDS::GenericAttribute_var anAttr;
    CORBA::Object_var anObject;
    if(CORBA::is_nil(theSObject))
      return anObject;
    try{
      if(theSObject->FindAttribute(anAttr, "AttributeIOR")){
	SALOMEDS::AttributeIOR_var anIOR  = SALOMEDS::AttributeIOR::_narrow(anAttr);
	CORBA::String_var aValue = anIOR->Value();
	CORBA::ORB_ptr anORB = getORB();
	if(strcmp(aValue,"") != 0)
	  anObject = anORB->string_to_object(aValue);
      }
    }catch(...){
      INFOS("SObjectToObject - Unknown exception was occured!!!");
    }
    return anObject._retn();
  }

  /*!
   * This function provides a CORBA pointer to a servant from its IOR
   * given as a string of characters.
   */
  CORBA::Object_ptr IORToObject(char * IOR) {
    return getORB()->string_to_object(IOR);
  }

  //
  // __GBO__ See the file ./src/SMESHGUI/SMESHGUI_Utils.h of SMESH_SRC
  // for other helper functions
  //

  SALOME::SALOME_Exception createSalomeException(const char * text) {
    SALOME::ExceptionStruct es;
    es.type = SALOME::INTERNAL_ERROR;
    es.text = CORBA::string_dup(text);
    return SALOME::SALOME_Exception(es);
  }

}
