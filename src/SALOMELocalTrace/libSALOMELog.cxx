// Copyright (C) 2007-2023  CEA/DEN, EDF R&D, OPEN CASCADE
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

//  Author : Konstantin Leontev (OpenCascade)
//  Module : KERNEL
//  $Header$
//

#include "libSALOMELog.hxx"

#include <string>
#include <iostream>

namespace SALOME
{

// ============================================================================
/*!
 *  Called by any log message macros to decide about log output in Release and
 *  Debug mode dynamically rely on SALOME_VERBOSE environment variable.
 *  Checks SALOME_VERBOSE only on the very first call and returns cached result
 *  for all followed calls.
 *  Returns true if SALOME_VERBOSE is positioned and not empty and if its
 *  numeric value greater than 0.
 */
// ============================================================================

  bool VerbosityActivated()
  {
    auto isEnvVarSet = []() -> bool
    {
      const char* envVar = std::getenv("SALOME_VERBOSE");

      if (envVar && (envVar[0] != '\0'))
      {
        try
        {
          const long long numValue = std::stoll(envVar);
          return numValue > 0;
        }
        catch(const std::exception& e)
        {
          std::cerr << e.what() << '\n';
        }
      }

      return false;
    };

    static const bool isActivated = isEnvVarSet();
    return isActivated;
  }
}
