// Copyright (C) 2021  CEA/DEN, EDF R&D, OPEN CASCADE
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

#include "SALOME_NamingService_Abstract.hxx"

// ============================================================================
/*! \brief provide a default container name if empty.
 *
 *  the given container name is returned unchanged, unless it is empty.
 * \param  containerName
 * \return container name, where empty input is replaced by "FactoryServer",
 *         without the path.
 * \sa BuildContainerNameForNS(const char *containerName, const char *hostname)
 */
// ============================================================================

std::string SALOME_NamingService_Abstract::ContainerName(const char *containerName)
{
  std::string ret,containerNameCpp(containerName);

  if (containerNameCpp.empty())
    ret = "FactoryServer";
  else
    ret = containerName;

  return ret;
}

// ============================================================================
/*! \brief build a string representing a container in Naming Service.
 *
 *  Build a string representing the absolute pathname of a container in
 *  SALOME_NamingService. This form gives a suffixed containerName in case of
 *  multi processor machine.
 * \param containerName name of the container in which the component is
                        instantiated.
 * \param hostname name of the host of the container, without domain names.
 * \return the path under the form /Containers/hostname/containerName
 * \sa ContainerName(const Engines::MachineParameters& params)
 */
// ============================================================================

std::string SALOME_NamingService_Abstract::BuildContainerNameForNS(const char *containerName, const char *hostname)
{
  std::string ret("/Containers/");
  ret += hostname;
  ret += "/";
  ret += ContainerName(containerName);

  return ret;
}
