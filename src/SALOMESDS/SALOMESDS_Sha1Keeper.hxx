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
// Author : Anthony GEAY (EDF R&D)

#pragma once

#include <vector>
#include <string>

namespace SALOMESDS
{
  class Sha1Keeper
  {
  public:
    Sha1Keeper(std::vector<unsigned char>&& sha1):_sha1(std::move(sha1)) { }
    void checkSha1(const std::string& varName, const std::vector<unsigned char>& sha1) const;
    virtual ~Sha1Keeper() { }
  protected:
    //! This sha1 is a discriminant key that allows RdExt "FreeStyle" methods to ensure that a "creation" over an already present var is legal or illegal.
    std::vector<unsigned char> _sha1;
  };
}

