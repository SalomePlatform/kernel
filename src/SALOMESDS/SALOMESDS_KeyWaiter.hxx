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

#ifndef __SALOMESDS_KEYWAITER_HXX__
#define __SALOMESDS_KEYWAITER_HXX__

#include "SALOMEconfig.h"
#include CORBA_SERVER_HEADER(SALOME_SDS)

#include "SALOMESDS_Defines.hxx"
#include "SALOMESDS_AutoRefCountPtr.hxx"
#include "SALOMESDS_DataScopeServer.hxx"
#include "SALOMESDS_PickelizedPyObjServer.hxx"

#include <Python.h>

#include <semaphore.h>

namespace SALOMESDS
{
  class DataScopeServerTransaction;
  
  class SALOMESDS_EXPORT KeyWaiter : public virtual POA_SALOME::KeyWaiter, public POAHolder
  {
  public:
    KeyWaiter(PickelizedPyObjServer *var, const SALOME::ByteVec& keyVal);
    PyObject *getKeyPyObj() const { return _ze_key; }
    virtual ~KeyWaiter();
    PortableServer::POA_var getPOA() const;
    SALOME::ByteVec *waitFor();
    void valueJustCome(PyObject *val);
    void go();
    std::string getVarName() const { return _var->getVarNameCpp(); }
    SALOME::ByteVec *waitForMonoThr();
  private:
    DataScopeServerTransaction *getDSS() const { return static_cast<DataScopeServerTransaction *>(_var->getFather()); }//thanks to dynamic_cast in constructor
  private:
    PickelizedPyObjServer *_var;
    PyObject *_ze_key;
    PyObject *_ze_value;
    sem_t _sem;
  };
}

#endif
