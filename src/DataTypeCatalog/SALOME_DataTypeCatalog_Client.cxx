//  SALOME DataTypeCatalog : catalog of data types
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
//  File   : SALOME_DataTypeCatalog_Client.cxx
//  Module : SALOME

/* $Header$ */

#include <iostream.h>
#include "SALOME_NamingService.hxx"
#include "SALOME_DataTypeCatalog.hh"
#include <string>
#include "utilities.h"
using namespace std;

int main(int argc,char **argv)
{
  CORBA::ORB_var orb;
  CosNaming::NamingContext_var _rootContext;
  CORBA::Object_var objVar, objVarN;
  try {

  	// initialize the ORB

  orb = CORBA::ORB_init (argc, argv);

 
  // Get CORBA reference of the catalog
  SALOME_NamingService NS(orb);
  CORBA::Object_var objVarN = NS.Resolve("/Kernel/DataTypeCatalog");

   SALOME_DataTypeCatalog::DataCatalog_var Catalogue  = SALOME_DataTypeCatalog::DataCatalog::_narrow(objVarN); 
   MESSAGE("Distant catalog of data type found")

     // Get component list
   SALOME_DataTypeCatalog::ListOfDataTypeName_var list_data_type = Catalogue->GetDataTypeList();
   int indice =-1;
   for (unsigned int ind = 0; ind < list_data_type->length();ind++)
     {
       MESSAGE("Data Type list : " << list_data_type[ind]);
     }

 

   // obtain interface_read of the first data type defined in the catalog
   char* interf;
   interf =Catalogue->GetDataInterfaceRead(list_data_type[0]);
   MESSAGE("Interface read pour " << list_data_type[0] << " : " << interf);



   // obtain interface_read of the last data type defined in the catalog
   interf =Catalogue->GetDataInterfaceRead(list_data_type[list_data_type->length() -1]);
   MESSAGE("Interface read pour " << list_data_type[list_data_type->length() -1] << " : " << interf);

   // obtain interface_write of the first data type defined in the catalog
   interf =Catalogue->GetDataInterfaceWrite(list_data_type[0]);
   MESSAGE("Interface write pour " << list_data_type[0] << " : " << interf);



   // obtain interface_read of the last data type defined in the catalog
   interf =Catalogue->GetDataInterfaceWrite(list_data_type[list_data_type->length() -1]);
   MESSAGE("Interface write pour " << list_data_type[list_data_type->length() -1] << " : " << interf);

   //obtain parents name of the last data type defined in the catalog
   SALOME_DataTypeCatalog::ListOfDataTypeName_var list_data_type1 = Catalogue->GetDataTypeParents(list_data_type[list_data_type->length() -1]);

   // Test derivation
   if (Catalogue->isDerivedFrom(list_data_type[0],list_data_type[list_data_type->length() -1]))
     {
     MESSAGE("The type " << list_data_type[list_data_type->length() -1] << " derived from " << list_data_type[0] );
     }
   else
     { 
     MESSAGE("The type " << list_data_type[list_data_type->length() -1] << " is not derived from " << list_data_type[0]);
     }

  }
  catch(SALOME_DataTypeCatalog::NotFound &){
    INFOS("SALOME_DataTypeCatalog::NotFound");
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
