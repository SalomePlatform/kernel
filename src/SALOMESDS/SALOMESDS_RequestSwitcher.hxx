// Copyright (C) 2018-2021  CEA/DEN, EDF R&D, OPEN CASCADE
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

#ifndef __SALOMESDS_REQUESTSWITCHER_HXX__
#define __SALOMESDS_REQUESTSWITCHER_HXX__

#include "SALOMEconfig.h"
#include CORBA_SERVER_HEADER(SALOME_SDS)
#include "SALOMESDS_AutoRefCountPtr.hxx"
#include "SALOMESDS_Defines.hxx"

namespace SALOMESDS
{
  /*!
   * Servant activated by a specific POA (single thread) having itself its specific POA_manager.
   * This class is able to hold/active the default POA_manager shared by other POA than this.
   */
  class SALOMESDS_EXPORT RequestSwitcherBase : public virtual POA_SALOME::RequestSwitcherBase, public POAHolder
  {
  public:
    RequestSwitcherBase(CORBA::ORB_ptr orb);
    void holdRequests();
    void activeRequests();
    PortableServer::POA_var getPOA() const { return _poa_for_request_control; }
  private:
    PortableServer::POA_var _poa_for_request_control;
    PortableServer::POAManager_var _poa_manager_under_control;
  };
}

#endif
