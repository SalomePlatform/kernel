// Copyright (C) 2009-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

// Author: André RIBES - EDF R&D
//
#ifndef _LAUNCHER_JOB_SALOME_HXX_
#define _LAUNCHER_JOB_SALOME_HXX_

#include "Launcher_Job.hxx"
#include "Launcher.hxx"

#ifdef WITH_LIBBATCH
#include <libbatch/Job.hxx>
#endif

namespace Launcher
{
  class LAUNCHER_EXPORT Job_SALOME : virtual public Launcher::Job
  {
    public:
      Job_SALOME();
      virtual ~Job_SALOME();

      virtual void setResourceDefinition(const ParserResourcesType & resource_definition);
      virtual void update_job();

#ifdef WITH_LIBBATCH
    public:
      std::string buildSalomeScript(Batch::Parametre params);
      virtual void addJobTypeSpecificScript(std::ofstream & launch_script_stream) = 0;
#endif
  };
}

#endif


