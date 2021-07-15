// Copyright (C) 2021  CEA/DEN, EDF R&D
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

#include "KernelLauncher.hxx"

#include "SALOME_Launcher.hxx"
#include "SALOME_ContainerManager.hxx"
#include "SALOME_Fake_NamingService.hxx"
#include "SALOME_KernelServices.hxx"
#include "SALOME_ResourcesManager.hxx"

#include <cstring>

std::string GetContainerManagerInstance()
{
  SALOME_Launcher *launcher = KERNEL::getLauncherSA();
  SALOME_Fake_NamingService ns;
  CORBA::Object_var cm = ns.Resolve(SALOME_ContainerManager::_ContainerManagerNameInNS);
  CORBA::ORB_ptr orb = KERNEL::getORB();
  CORBA::String_var ior = orb->object_to_string(cm);
  return std::string(ior.in());
}

std::string GetResourcesManagerInstance()
{
  SALOME_Launcher *launcher = KERNEL::getLauncherSA();
  SALOME_Fake_NamingService ns;
  CORBA::Object_var cm = ns.Resolve(SALOME_ResourcesManager::_ResourcesManagerNameInNS);
  CORBA::ORB_ptr orb = KERNEL::getORB();
  CORBA::String_var ior = orb->object_to_string(cm);
  return std::string(ior.in());
}
