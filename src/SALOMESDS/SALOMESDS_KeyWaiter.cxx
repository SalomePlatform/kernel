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

#include "SALOMESDS_KeyWaiter.hxx"
#include "SALOMESDS_DataScopeServer.hxx"
#include "SALOMESDS_PickelizedPyObjServer.hxx"

#include <sstream>

using namespace SALOMESDS;

KeyWaiter::KeyWaiter(PickelizedPyObjServer *var, const SALOME::ByteVec& keyVal):_var(var),_ze_key(0),_ze_value(0)
{
  if(sem_init(&_sem,0,0)!=0)// put value to 0 to lock by default
    throw Exception("KeyWaiter constructor : Error on initialization of semaphore !");
  if(!var)
    throw Exception("KeyWaiter constructor : Invalid glob var is NULL !");
  if(!dynamic_cast<DataScopeServerTransaction *>(var->getFather()))
    throw Exception("KeyWaiter constructor : Invalid glob var ! Invalid DataScope hosting it ! DataScopeServerTransaction excpected !");
  std::string st;
  PickelizedPyObjServer::FromByteSeqToCpp(keyVal,st);
  _ze_key=PickelizedPyObjServer::GetPyObjFromPickled(st,getDSS());
  PyObject *selfMeth(PyObject_GetAttrString(_var->getPyObj(),"__contains__"));//new ref
  PyObject *args(PyTuple_New(1));
  PyTuple_SetItem(args,0,_ze_key); Py_XINCREF(_ze_key); // _ze_key is stolen by PyTuple_SetItem
  PyObject *retPy(PyObject_CallObject(selfMeth,args));
  Py_XDECREF(args);
  Py_XDECREF(selfMeth);
  //
  if(retPy!=Py_False && retPy!=Py_True)
    throw Exception("KeyWaiter constructor : unexpected return of dict.__contains__ !");
  if(retPy==Py_True)
    {
      PyObject *retPy2(PyDict_GetItem(_var->getPyObj(),_ze_key));
      if(retPy2==NULL)
        throw Exception("KeyWaiter constructor : dict.getitem fails !");
      Py_XINCREF(retPy2);
      _ze_value=retPy2;
      go();//notify that value already arrives -> unlock
    }
  else
    {
      getDSS()->addWaitKey(this);// key not present let the DataScope managing it !
    }
  Py_XDECREF(retPy);
}

KeyWaiter::~KeyWaiter()
{
  Py_XDECREF(_ze_key);
  if(_ze_value)
    Py_XDECREF(_ze_value);
}

PortableServer::POA_var KeyWaiter::getPOA() const
{
  return getDSS()->getPOA4KeyWaiter();
}

/*!
 * WARNING : here it is the single method that can be invoked in non SINGLE_THREAD POA.
 * So take care to do avoid collapses (especially in python).
 */
SALOME::ByteVec *KeyWaiter::waitFor()
{
  sem_wait(&_sem);
  if(!_ze_value)
    throw Exception("KeyWaiter::waitFor : internal error 1 !");
  SALOME::ByteVec *ret(0);
  {// this section is to guarantee that no concurrent threads are doing python stuff at the same time
    // Here a pickelization is needed so to guarantee to be alone doing python action the idea is to invoke using monothread POA.
    DataScopeServerTransaction *dss(getDSS());
    PortableServer::POA_var poa(dss->getPOA());
    CORBA::Object_var dssPtr(poa->servant_to_reference(dss));
    SALOME::DataScopeServerTransaction_var dssPtr2(SALOME::DataScopeServerTransaction::_narrow(dssPtr));
    if(CORBA::is_nil(dssPtr2))
      throw Exception("KeyWaiter::waitFor : internal error 2 !");
    CORBA::Object_var thisPtr(getPOA()->servant_to_reference(this));
    SALOME::KeyWaiter_var thisPtr2(SALOME::KeyWaiter::_narrow(thisPtr));
    if(CORBA::is_nil(thisPtr2))
      throw Exception("KeyWaiter::waitFor : internal error 3 !");
    ret=dssPtr2->waitForMonoThrRev(thisPtr2);//<- this invokation through SINGLE_THREAD POA here will guarantee thread safety
  }
  enforcedRelease();
  return ret;
}

/*!
 * this method is supposed to be performed in alone.
 */
SALOME::ByteVec *KeyWaiter::waitForMonoThr()
{
  if(!_ze_value)
    throw Exception("KeyWaiter::waitForMonoThr : no value ! invalid call of this method !");
  Py_XINCREF(_ze_value);
  std::string st(PickelizedPyObjServer::Pickelize(_ze_value,_var->getFather()));
  return PickelizedPyObjServer::FromCppToByteSeq(st);
}

/*!
 * WARNING call this method before calling go !
 */
void KeyWaiter::valueJustCome(PyObject *val)
{
  if(_ze_value==val)
    return ;
  if(_ze_value)
    Py_XDECREF(_ze_value);
  _ze_value=val;
  Py_XINCREF(_ze_value);
}

void KeyWaiter::go()
{
  if(sem_post(&_sem)!=0)
    {
      std::ostringstream oss; oss << "KeyWaiter::go : error on post of semaphore ! ";
      throw Exception(oss.str());
    }
}
