// Copyright (C) 2018  CEA/DEN, EDF R&D, OPEN CASCADE
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

#include "SALOMESDS_Sha1Keeper.hxx"
#include "SALOMESDS_Exception.hxx"

#include <sstream>

void SALOMESDS::Sha1Keeper::checkSha1(const std::string& varName, const std::vector<unsigned char>& sha1) const
{
  if(sha1!=_sha1)
    {
      std::ostringstream oss; oss << "PickelizedPyObjRdExtFreeStyleServer::checkSha1 : SHA1 check fails ! Attempt of corruption of rdext data ! It means that var \"" << varName << "\" has been created but with an initial value different from the new value !";
      throw Exception(oss.str());
    }
}
