// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

#ifndef __BatchTest_HXX__
#define __BatchTest_HXX__

#include "SALOME_Launcher_defs.hxx"

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_ContainerManager)

#include <string>

class SALOMELAUNCHER_EXPORT BatchTest 
{
  public:
    BatchTest(const Engines::ResourceDefinition& batch_descr);
    virtual ~BatchTest();

    bool test();

    std::string test_connection();
    std::string test_filecopy();
    std::string test_getresult();
    std::string test_jobsubmit_simple();
    std::string test_jobsubmit_mpi();
    std::string test_appli();

  protected:
    std::string get_home(std::string * home);
  
  private:
    Engines::ResourceDefinition _batch_descr;
    std::string _test_filename;
    std::string _base_filename;
    std::string _date;
};

#endif
