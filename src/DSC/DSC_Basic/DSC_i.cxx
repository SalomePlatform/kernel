// Copyright (C) 2007-2025  CEA, EDF, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

//  File   : DSC_i.cxx
//  Author : Andre RIBES (EDF)
//  Module : KERNEL
//
#include "DSC_i.hxx"


Engines_DSC_i::
Engines_DSC_i(CORBA::ORB_ptr orb,
                        PortableServer::POA_ptr poa,
                        PortableServer::ObjectId * contId,
                        const char *instanceName,
                        const char *interfaceName,
                        bool notif) : Engines_Component_i(orb,
                                                              poa, 
                                                              contId, 
                                                              instanceName, 
                                                              interfaceName,
                                                              notif,
                                                              false
                                                         )
{
  if (SALOME::VerbosityActivated())
    std::cerr << "--Engines_DSC_i: MARK 1 --" << instanceName << "----" << std::endl;
}

Engines_DSC_i::
Engines_DSC_i(CORBA::ORB_ptr orb,
              PortableServer::POA_ptr poa,
              Engines::Container_ptr container, 
              const char *instanceName,
              const char *interfaceName,
              bool notif,
              bool regist) : 
    Engines_Component_i(orb, poa, container, instanceName, interfaceName,notif,regist) 
{
  if (SALOME::VerbosityActivated())
    std::cerr << "--Engines_DSC_i: MARK 1 --" << instanceName << "----" << std::endl;
}

Engines_DSC_i::~Engines_DSC_i() {}

