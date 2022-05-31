// Copyright (C) 2007-2022  CEA/DEN, EDF R&D, OPEN CASCADE
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

#include "SALOME_DataScopeServer_Common.cxx"

#include "SALOME_NamingService.hxx"

int main(int argc, char *argv[])
{
  CORBA::ORB_var orb = GetCustomORB();
  std::string scopeName;
  if(argc<=1)
    throw SALOMESDS::Exception("In the main of SALOME_DataScopeServer.cxx !");
  scopeName=argv[1];
  std::istringstream isTransacSS(argv[2]);
  int isTransac(0);
  isTransacSS >> isTransac;
  return CommonDataScopeServerMain(argc,argv,orb,scopeName,isTransac,[](CORBA::ORB_ptr orb2) { return new SALOME_NamingService(orb2); });
}
