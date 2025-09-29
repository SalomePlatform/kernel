// Copyright (C) 2007-2025  CEA, EDF, OPEN CASCADE
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
// Author : Anthony GEAY (EDF R&D)

#ifndef __SALOMESDS_EXCEPTION_HXX__
#define __SALOMESDS_EXCEPTION_HXX__

#include "SALOMEconfig.h"
#include CORBA_SERVER_HEADER(SALOME_Exception)

#include "SALOMESDS_Defines.hxx"

#include <string>

namespace SALOMESDS
{
  class SALOMESDS_EXPORT Exception : public SALOME_CMOD::SALOME_Exception
  {
  public:
    Exception(const std::string& reason);
    Exception(const char *reason);
  private:
    void assign(const char *reason);
  };

   class SALOMESDS_EXPORT NotSameException : public Exception
   {
   public:
     NotSameException(const std::string& reason):Exception(reason) { }
     NotSameException(const char *reason):Exception(reason) { }
   };
}

#endif
