// Copyright (C) 2007-2014  CEA/DEN, EDF R&D, OPEN CASCADE
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

#ifndef __SALOMESDS_REFCOUNTSERV_HXX__
#define __SALOMESDS_REFCOUNTSERV_HXX__

#include <omniORB4/CORBA.h>

namespace SALOMESDS
{
  class RefCountServ : public virtual PortableServer::ServantBase
  {
  public:
    void incrRef() const;
    bool decrRef() const;
    virtual PortableServer::POA_var getPOA() = 0;
  protected:
    void enforcedRelease();
    RefCountServ();
    RefCountServ(const RefCountServ& other);
    virtual ~RefCountServ();
  private:
    mutable unsigned int _cnt;
  };
}

#endif
