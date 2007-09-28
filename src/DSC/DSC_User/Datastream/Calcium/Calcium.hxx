#include "Superv_Component_i.hxx"

class PySupervCompo:public Superv_Component_i
{
  public:
    PySupervCompo(CORBA::ORB_ptr orb,
         PortableServer::POA_ptr poa,
         Engines::Container_ptr contain,
         const char *instanceName,
         const char *interfaceName,
         bool notif = false);
    virtual ~PySupervCompo();
    CORBA::Boolean init_service(const char * service_name){return true;};
};


extern "C" void create_calcium_port(Superv_Component_i* compo,char* name,char* type,char *mode,char* depend);
