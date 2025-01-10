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

//  SALOME Utils : general SALOME's definitions and tools
//  File   : OpUtil.cxx
//  Module : SALOME
//
#include "OpUtil.hxx"
#include <mutex>

#if !defined(SALOME_LIGHT)

#include "Utils_SINGLETON.hxx"
#include "Utils_ORB_INIT.hxx"

namespace
{
  std::recursive_mutex mutex; //!< mutex to protect access to static data
}

UTILS_EXPORT CORBA::ORB_var KERNEL::GetRefToORB()
{
  std::lock_guard<std::recursive_mutex> g(mutex);  
  ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance();
  CORBA::ORB_var orb = init();
  return orb;
}

#endif
