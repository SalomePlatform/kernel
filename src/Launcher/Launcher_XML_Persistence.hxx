// Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

#ifndef __LAUNCHER_XML_PERSISTENCE_HXX__
#define __LAUNCHER_XML_PERSISTENCE_HXX__

#include <list>

#include "Launcher_Utils.hxx"
#include "Launcher_Job.hxx"

namespace Launcher
{
  typedef struct _xmlNode xmlNode;
  typedef xmlNode *xmlNodePtr;

  class LAUNCHER_EXPORT XML_Persistence
  {
  public:
    virtual ~XML_Persistence() {}

    /*! Load the jobs from the XML file "jobs_file".
     *  Return a list with the jobs that were successfully loaded.
     *  The ownership of the created jobs is transferred to the caller.
     */
    static std::list<Job *> loadJobs(const char* jobs_file);

    //! Save the jobs in the list "jobs_list" to the XML file "jobs_file".
    static void saveJobs(const char* jobs_file, const std::list<const Job *> & jobs_list);

  private:
    // This class is static only, not instanciable
    XML_Persistence() {}

    static void addJobToXmlDocument(xmlNodePtr root_node, const Job & job);

  };

}

#endif