#include "Calcium.hxx"
#include <CalciumInterface.hxx>
#include <calcium.h>
#include <iostream>
#include <string>
#include <exception>

//#define _DEBUG_

PySupervCompo::PySupervCompo( CORBA::ORB_ptr orb,
                              PortableServer::POA_ptr poa,
                              Engines::Container_ptr contain,
                              const char *instanceName,
                              const char *interfaceName,
                              bool notif) :
  Superv_Component_i(orb, poa,contain, instanceName, interfaceName)
{
}

PySupervCompo::~PySupervCompo()
{
}


extern "C" 
{
  void cp_exit(int err)
    {
      throw CalciumException(err,LOC("Abort coupling"));
    }

  void setDependency(provides_port * port,char* type,CalciumTypes::DependencyType depend)
  {
    if(std::string(type)=="CALCIUM_real")
      {
        dynamic_cast<calcium_real_port_provides *>(port)->setDependencyType(depend);
      }
    else if(std::string(type)=="CALCIUM_double")
      {
        dynamic_cast<calcium_double_port_provides *>(port)->setDependencyType(depend);
      }
    else if(std::string(type)=="CALCIUM_integer")
      {
        dynamic_cast<calcium_integer_port_provides *>(port)->setDependencyType(depend);
      }
    else if(std::string(type)=="CALCIUM_string")
      {
        dynamic_cast<calcium_string_port_provides *>(port)->setDependencyType(depend);
      }
    else if(std::string(type)=="CALCIUM_logical")
      {
        dynamic_cast<calcium_logical_port_provides *>(port)->setDependencyType(depend);
      }
    else if(std::string(type)=="CALCIUM_complex")
      {
        dynamic_cast<calcium_complex_port_provides *>(port)->setDependencyType(depend);
      }
    else
      {
        std::cerr << "unknown type:" << std::endl;
      }
  }

  void create_calcium_port(Superv_Component_i* compo,char* name,char* type,char *mode,char* depend)
  {
#ifdef _DEBUG_
    std::cerr << "create_calcium_port: " << name << " " << type << " " << mode << " " << depend << std::endl;
#endif

    if(std::string(mode) == "IN")
      {
        provides_port * port ;
        //provides port
        try
          {
            port = compo->create_provides_data_port(type);
            compo->add_port(port, name);
            if(std::string(depend) == "I")
              setDependency(port,type,CalciumTypes::ITERATION_DEPENDENCY);
            else if(std::string(depend) == "T")
              setDependency(port,type,CalciumTypes::TIME_DEPENDENCY);
            else
              {
                std::cerr << "create_calcium_port:unknown dependency: " << depend << std::endl;
              }
          }
        catch(const Superv_Component_i::PortAlreadyDefined& ex)
          {
            //Port already defined : we use the old one
            delete port;
            std::cerr << "create_calcium_port: " << ex.what() << std::endl;
          }
        catch ( ... )
          {
            std::cerr << "create_calcium_port: unknown exception" << std::endl;
          }
      }
    else if(std::string(mode) == "OUT")
      {
        uses_port * uport ;
        try
          {
            uport = compo->create_uses_data_port(type);
            compo->add_port(uport, name);
          }
        catch(const Superv_Component_i::PortAlreadyDefined& ex)
          {
            //Port already defined : we use the old one
            delete uport;
            std::cerr << "create_calcium_port: " << ex.what() << std::endl;
          }
        catch ( ... )
          {
            std::cerr << "create_calcium_port: unknown exception" << std::endl;
          }
      }
    else
      {
        //Unknown mode
        std::cerr << "create_calcium_port:Unknown mode: " << mode << std::endl;
      }
  }

}


