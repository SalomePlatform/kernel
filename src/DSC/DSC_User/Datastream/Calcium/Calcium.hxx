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
    CORBA::Boolean init_service_with_multiple(const char* service_name,
                                              const Engines::Superv_Component::seq_multiple_param & params)
    {
      return true;
    }
};


extern "C" void create_calcium_port(Superv_Component_i* compo,char* name,char* type,char *mode,char* depend);

// This method permits to help a service developer to create multiple calcium ports
extern "C" char** create_multiple_calcium_port(Superv_Component_i* compo,char* name,char* type,char *mode,char* depend, int number);
