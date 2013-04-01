// Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
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

#include "Launcher.hxx"

int main(int argc, char** argv)
{

  try {
    Launcher_cpp *lcpp = new Launcher_cpp();
    ResourcesManager_cpp *rcpp = new ResourcesManager_cpp();
    lcpp->SetResourcesManager(rcpp);
    delete lcpp;
    delete rcpp;
    std::cout << "test OK" << std::endl;
  } catch ( const ResourcesException &ex) {
    std::cout << ex.msg.c_str() << std::endl;
  }
  catch ( const LauncherException &ex) {
    std::cout << ex.msg.c_str() << std::endl;
  }
  catch (...){
    std::cout << "test KO" << std::endl;
  }
  exit(0);
}
