using namespace std;
/* $Header$ */

#include <iostream.h>
#include "SALOME_NamingService.hxx"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_RessourcesCatalog)
#include <string>
#include "utilities.h"

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
  CORBA::Object_var objVarN = NS.Resolve("/Kernel/RessourcesCatalog");

  SALOME_RessourcesCatalog::RessourcesCatalog_var Catalogue  = SALOME_RessourcesCatalog::RessourcesCatalog::_narrow(objVarN); 
  MESSAGE("Ressources distant catalog found");

  //Get Computer list information
  SALOME_RessourcesCatalog::ListOfComputer_var _list = Catalogue->GetComputerList();
  for (unsigned int ind = 0; ind < _list->length();ind++)
    {
      MESSAGE("Ressources list : " << _list[ind]);
    }

  //Get container type list of computer eri
  SALOME_RessourcesCatalog::ListOfContainerType_var list = Catalogue->GetContainerTypeList("eri");
  for (unsigned int ind = 0; ind < list->length();ind++)
    {
      MESSAGE("Container type list of eri : " << list[ind]);
    }

  // Get processors information of eri
  SALOME_RessourcesCatalog::computer_info_var computer = Catalogue->GetComputerInfo("eri");
  MESSAGE("Ressource name : " << computer->name);
  MESSAGE("Ressource OS : " << computer->OS);
  MESSAGE("Ressource OS version : " << computer->OS_version);
  for (unsigned int ind = 0; ind < computer->procs.length();ind++)
    {
      MESSAGE("Processor number : " << computer->procs[ind].number);
      MESSAGE("Processor model name : " << computer->procs[ind].model_name);
      MESSAGE("Processor cpu : " << computer->procs[ind].cpu_mhz);
      MESSAGE("Processor cache : " << computer->procs[ind].cache_size);
    }

  }
  catch(SALOME_RessourcesCatalog::NotFound &){
    INFOS("SALOME_RessourcesCatalog::NotFound");
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
