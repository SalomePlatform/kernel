//  SALOME ModuleCatalog : implementation of ModuleCatalog server which parsers xml description of modules
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SALOME_ModuleCatalog_Client.cxx
//  Module : SALOME

/* $Header$ */

#include <iostream.h>
#include "SALOME_NamingService.hxx"
#include "SALOME_ModuleCatalog.hh"
#include <string>
#include "utilities.h"
using namespace std;

int main(int argc,char **argv)
{
  char* name_to_find = "Geometry";

  CORBA::ORB_var orb;
  CosNaming::NamingContext_var _rootContext;
  CORBA::Object_var objVar, objVarN;
  try {

  	// initialize the ORB

  orb = CORBA::ORB_init (argc, argv);

 
  // Get CORBA reference of the catalog
  SALOME_NamingService NS(orb);
  CORBA::Object_var objVarN = NS.Resolve("/Kernel/ModulCatalog");

   SALOME_ModuleCatalog::ModuleCatalog_var Catalogue  = SALOME_ModuleCatalog::ModuleCatalog::_narrow(objVarN); 
   MESSAGE("Distant catalog of component found")

     // Get component list
   SALOME_ModuleCatalog::ListOfComponents_var list_composants = Catalogue->GetComponentList();


   // Typed component list
   MESSAGE("Get Typed Component list (GEOM Type)");
   SALOME_ModuleCatalog::ListOfComponents_var list_typed_composants = Catalogue->GetTypedComponentList(SALOME_ModuleCatalog::GEOM);
   for (unsigned int ind = 0; ind < list_typed_composants->length();ind++)
     MESSAGE("Component GEOM list : " << list_typed_composants[ind]);      

   MESSAGE("Get Typed Component list (SUPERV Type)");
   SALOME_ModuleCatalog::ListOfComponents_var list_styped_composants = Catalogue->GetTypedComponentList(SALOME_ModuleCatalog::SUPERV);
   for (unsigned int ind = 0; ind < list_styped_composants->length();ind++)
      MESSAGE("Component SUPERV list : " << list_styped_composants[ind]);

   // Get list of couple (component name, component icone)
   SALOME_ModuleCatalog::ListOfIAPP_Affich_var list_composants_icone = Catalogue->GetComponentIconeList();
   for (unsigned int ind = 0; ind < list_composants_icone->length();ind++)
     {
       MESSAGE("Component name: " << list_composants_icone[ind].modulename);
       MESSAGE("Component icone: " << list_composants_icone[ind].moduleicone);
     }
  
   // obtain a component (specified as parameter of the client)
   SALOME_ModuleCatalog::Acomponent_ptr Geom = Catalogue->GetComponent("Geometry");
   if (CORBA::is_nil (Geom)) 
   {
     INFOS("Catalog Error : Component Geometry not found in the catalog")
      exit (-1);
   }
 
   MESSAGE("Component of name : " <<  Geom->componentname() << " created ");
   MESSAGE("Component type : " << Geom->component_type() << " multistudy : " <<Geom->multistudy());
   MESSAGE("Component constraint : " << Geom->constraint());
   MESSAGE("Component icone : " << Geom->component_icone());

     // Obtain another component
   SALOME_ModuleCatalog::Acomponent_ptr Superv = Catalogue->GetComponent("Supervision");
   if (CORBA::is_nil (Superv)) 
   {
     INFOS("Catalog Error : Component Supervision not found in the catalog")
      exit (-1);
   }
 
   MESSAGE("Component of name : " <<  Superv->componentname() << " created ");
   MESSAGE("Component type : " << Superv->component_type() << " multistudy : " <<Superv->multistudy());
   MESSAGE("Component constraint : " << Superv->constraint());
   MESSAGE("Component icone : " << Superv->component_icone());

     // obtain prefix path for a computer
   char* path;
   path =Superv->GetPathPrefix("omote");
   MESSAGE("Path prefix pour omote : " << path);

   // obtain prefix path for a computer
   path =Geom->GetPathPrefix("eri");
   MESSAGE("Path prefix pour eri : " << path);

   // obtain interfaces list of the component
   SALOME_ModuleCatalog::ListOfInterfaces_var _list = new SALOME_ModuleCatalog::ListOfInterfaces;
   _list = Geom->GetInterfaceList();
   for (unsigned int ind = 0; ind < _list->length();ind++)
     MESSAGE("Interface name : " << _list[ind]);

   // obtain interfaces list of the component
   SALOME_ModuleCatalog::ListOfInterfaces_var _list1 = new SALOME_ModuleCatalog::ListOfInterfaces;
   _list1 = Superv->GetInterfaceList();
   for (unsigned int ind = 0; ind < _list1->length();ind++)
     MESSAGE("Interface name : " << _list1[ind])

   	
   // Obtain one interface of a component
   SALOME_ModuleCatalog::DefinitionInterface_var _interf = new SALOME_ModuleCatalog::DefinitionInterface;
   _interf = Geom->GetInterface("Geometry");
   MESSAGE ("Interface " << _interf->interfacename << " getted")
   for (unsigned int ind = 0; ind < _interf->interfaceservicelist.length();ind++)
     MESSAGE("Service name : " << _interf->interfaceservicelist[ind].ServiceName)


   // obtain services list of the component
   	
   SALOME_ModuleCatalog::ListOfServices_var list_services_component = new SALOME_ModuleCatalog::ListOfServices;
   list_services_component = Geom->GetServiceList("Geometry");
   for (unsigned int ind = 0; ind < list_services_component->length();ind++)
       MESSAGE("Service name : " << list_services_component[ind])


    // Get a service of the component

   SALOME_ModuleCatalog::Service_var second_service = new SALOME_ModuleCatalog::Service;
    second_service = Geom->GetDefaultService("Geometry");

    MESSAGE("Default Service name: " << second_service->ServiceName)
   for (unsigned int ind1 = 0; ind1 <second_service->ServiceinParameter.length();ind1++)
     {
       MESSAGE("Type of the in Parameter of this service : " << second_service->ServiceinParameter[ind1].Parametertype)
       MESSAGE("Name of the in Parameter of this service : " << second_service->ServiceinParameter[ind1].Parametername)
     }
   for (unsigned int ind1 = 0; ind1 <second_service->ServiceoutParameter.length();ind1++)
     {
       MESSAGE("Type of the out Parameter of this service : " << second_service->ServiceoutParameter[ind1].Parametertype)
       MESSAGE("Name of the out Parameter of this service : " << second_service->ServiceoutParameter[ind1].Parametername)
     }



    second_service = Geom->GetDefaultService("Geometry");

    MESSAGE("Default Service name: " << second_service->ServiceName)
   for (unsigned int ind1 = 0; ind1 <second_service->ServiceinParameter.length();ind1++)
     {
       MESSAGE("Type of the in Parameter of this service : " << second_service->ServiceinParameter[ind1].Parametertype)
       MESSAGE("Name of the in Parameter of this service : " << second_service->ServiceinParameter[ind1].Parametername)
     }
   for (unsigned int ind1 = 0; ind1 <second_service->ServiceoutParameter.length();ind1++)
     {
       MESSAGE("Type of the out Parameter of this service : " << second_service->ServiceoutParameter[ind1].Parametertype)
       MESSAGE("Name of the out Parameter of this service : " << second_service->ServiceoutParameter[ind1].Parametername)
     }



   // Get service of the component
   	
   SALOME_ModuleCatalog::Service_var service_1 = new SALOME_ModuleCatalog::Service;

   service_1 = Geom->GetService("Geometry","MakeSphere") ;

   MESSAGE("Service name: " << service_1->ServiceName)
   for (unsigned int ind1 = 0; ind1 <service_1->ServiceinParameter.length();ind1++)
     {
       MESSAGE("Type of the in Parameter of this service : " << service_1->ServiceinParameter[ind1].Parametertype)
       MESSAGE("Name of the in Parameter of this service : " << service_1->ServiceinParameter[ind1].Parametername)
     }

   for (unsigned int ind1 = 0; ind1 <service_1->ServiceoutParameter.length();ind1++)
     {
       MESSAGE("Type of the out Parameter of this service : " << service_1->ServiceoutParameter[ind1].Parametertype)
       MESSAGE("Name of the out Parameter of this service : " << service_1->ServiceoutParameter[ind1].Parametername)
     }

   // Test levée exception
   service_1 = Geom->GetService("BOX","makeBox") ;


    }
  catch(SALOME_ModuleCatalog::NotFound &ex){
    INFOS("SALOME_ModuleCatalog::NotFound")
      cerr << ex.what << endl;
  }
    catch(CORBA::SystemException&) {
      INFOS("Caught CORBA::SystemException.")
  }
    catch (CosNaming::NamingContext::CannotProceed &) {
      INFOS("CosNaming::NamingContext::CannotProceed")
  }
    catch (CosNaming::NamingContext::NotFound &) {
      INFOS("CosNaming::NamingContext::NotFound")
  }
    catch (CosNaming::NamingContext::InvalidName &) {
      INFOS("CosNaming::NamingContext::InvalidName")
  }
    catch (CosNaming::NamingContext::AlreadyBound &) {
      INFOS("CosNaming::NamingContext::AlreadyBound")
  }
    catch (CosNaming::NamingContext::NotEmpty &) {
      INFOS("CosNaming::NamingContext::NotEmpty")
  }

  catch(CORBA::Exception &sysEx) {
    INFOS("Caught CORBA::Exception.")
  }


  return 0;
}














