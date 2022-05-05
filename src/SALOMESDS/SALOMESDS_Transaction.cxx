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

#include "SALOMESDS_Transaction.hxx"
#include "SALOMESDS_Exception.hxx"
#include "SALOMESDS_PickelizedPyObjServer.hxx"
#include "SALOMESDS_PickelizedPyObjRdWrServer.hxx"
#include "SALOMESDS_PickelizedPyObjRdExtServer.hxx"
#include "SALOMESDS_TrustTransaction.hxx"

#include <sstream>

using namespace SALOMESDS;

void Transaction::FromByteSeqToVB(const SALOME::ByteVec& bsToBeConv, std::vector<unsigned char>& ret)
{
  std::size_t sz(bsToBeConv.length());
  ret.resize(sz);
  unsigned char *buf(const_cast<unsigned char *>(&ret[0]));
  for(std::size_t i=0;i<sz;i++)
    buf[i]=bsToBeConv[(CORBA::ULong)i]; //!< TODO: size_t to CORBA::ULong
}

void Transaction::FromVBToByteSeq(const std::vector<unsigned char>& bsToBeConv, SALOME::ByteVec& ret)
{
  std::size_t sz(bsToBeConv.size());
  ret.length((CORBA::ULong)sz); //!< TODO: size_t to CORBA::ULong
  for(std::size_t i=0;i<sz;i++)
    ret[(CORBA::ULong)i]=bsToBeConv[i]; //!< TODO: size_t to CORBA::ULong
}

Transaction::~Transaction()
{
}

TransactionVarCreate::TransactionVarCreate(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME::ByteVec& constValue):Transaction(dsct,varName)
{
  FromByteSeqToVB(constValue,_data);
}

void TransactionVarCreate::prepareRollBackInCaseOfFailure()
{
  checkNotAlreadyExisting();
}

void TransactionVarCreate::rollBack()
{
  if(_dsct->existVar(_var_name.c_str()))
    _dsct->deleteVar(_var_name.c_str());
}

/*!
 * no implementation it is not a bug !
 */
void TransactionVarCreate::notify()
{
}

void TransactionRdOnlyVarCreate::perform()
{
  SALOME::ByteVec data2;
  FromVBToByteSeq(_data,data2);
  _dsct->createRdOnlyVarInternal(_var_name,data2);
}

void TransactionRdExtVarCreate::perform()
{
  SALOME::ByteVec data2;
  FromVBToByteSeq(_data,data2);
  _dsct->createRdExtVarInternal(_var_name,data2);
}

TransactionRdExtVarFreeStyleCreate::TransactionRdExtVarFreeStyleCreate(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME::ByteVec& constValue, const char *compareFuncContent):TransactionRdExtVarCreate(dsct,varName,constValue),_cmp_func_content(compareFuncContent),_cmp_func(nullptr)
{
  constexpr char EXPECTED_COMPARE_FUNC_NAME[]="comptchev";
  SALOME::AutoPyRef context(PyDict_New());
  SALOME::AutoPyRef res(PyRun_String(compareFuncContent,Py_file_input,_dsct->getGlobals(),context));
  if(res.isNull())
    {
      std::ostringstream oss; oss << "TransactionRdExtVarFreeStyleCreate ctor : Fail to parse and evaluate \"" << compareFuncContent << "\" as python function !";
      throw Exception(oss.str());
    }
  PyObject *func(PyDict_GetItemString(context,EXPECTED_COMPARE_FUNC_NAME));//borrowed
  if(!func)
    {
      std::ostringstream oss; oss << "TransactionRdExtVarFreeStyleCreate ctor : Parsing of func is OK but not func called \"" << EXPECTED_COMPARE_FUNC_NAME << "\" is the given parsed string !";
      throw Exception(oss.str());
    }
  _cmp_func = func; Py_XINCREF(func);
  if(PyDict_DelItemString(context,EXPECTED_COMPARE_FUNC_NAME)!=0)
    {
      std::ostringstream oss; oss << "TransactionRdExtVarFreeStyleCreate ctor : Internal error during suppression of \"" << EXPECTED_COMPARE_FUNC_NAME << "\" key that exepect to be present ! Smells bad !";
      throw Exception(oss.str());
    }
}

void TransactionRdExtVarFreeStyleCreate::prepareRollBackInCaseOfFailure()
{//nothing it is not a bug
}

void TransactionRdExtVarFreeStyleCreate::rollBack()
{// nothing to be done here. Fress style means I don t care. Do not remove var.
  if(!_null_rollback)
    TransactionRdExtVarCreate::rollBack();
}

void TransactionRdExtVarFreeStyleCreate::perform()
{
  SALOME::ByteVec data2;
  FromVBToByteSeq(_data,data2);
  try
    {
      _dsct->createRdExtVarFreeStyleInternal(_var_name,data2,std::move(_cmp_func_content),std::move(_cmp_func));
    }
  catch(NotSameException& e)
    {
      _null_rollback = true;
      throw e;
    }
}

void TransactionRdExtInitVarCreate::perform()
{
  SALOME::ByteVec data2;
  FromVBToByteSeq(_data,data2);
  _dsct->createRdExtInitVarInternal(_var_name,data2);
}

void TransactionRdWrVarCreate::perform()
{
  SALOME::ByteVec data2;
  FromVBToByteSeq(_data,data2);
  _dsct->createRdWrVarInternal(_var_name,data2);
}

TransactionKillVar::TransactionKillVar(DataScopeServerTransaction *dsct, const std::string& varName):Transaction(dsct,varName)
{
}

/*!
 * TODO
 */
void TransactionKillVar::prepareRollBackInCaseOfFailure()
{
  checkVarExisting();
  //BasicDataServer *bds(_dsct->retrieveVarInternal2(_var_name);
  //bds->clone();
}

void TransactionKillVar::perform()
{
  _dsct->deleteVar(_var_name.c_str());
}

/*!
 * TODO
 */
void TransactionKillVar::rollBack()
{
}

/*!
 * no implementation it is not a bug ! killVar is not an event.
 */
void TransactionKillVar::notify()
{
}

TransactionDictModify::TransactionDictModify(DataScopeServerTransaction *dsct, const std::string& varName):Transaction(dsct,varName),_varc(0)
{
  _varc=checkVarExistingAndDict();
}

void TransactionDictModify::prepareRollBackInCaseOfFailure()
{
  _zeDataBefore.clear();
  PyObject *zeDictPy(_varc->getPyObj());
  Py_XINCREF(zeDictPy);
  _zeDataBefore=_varc->pickelize(zeDictPy);
}

void TransactionDictModify::rollBack()
{
  PyObject *obj(_varc->getPyObjFromPickled(_zeDataBefore));
  _varc->setNewPyObj(obj);
  _zeDataBefore.clear();
}

TransactionAddKeyValue::TransactionAddKeyValue(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME::ByteVec& key, const SALOME::ByteVec& value):TransactionDictModify(dsct,varName)
{
  std::vector<unsigned char> key2,value2;
  FromByteSeqToVB(key,key2);
  FromByteSeqToVB(value,value2);
  _key=PickelizedPyObjServer::GetPyObjFromPickled(key2,_dsct);
  _value=PickelizedPyObjServer::GetPyObjFromPickled(value2,_dsct);
}

void TransactionAddKeyValue::prepareRollBackInCaseOfFailure()
{
  TransactionDictModify::prepareRollBackInCaseOfFailure();
  _dsct->pingKey(_key);// check that key is OK with all waiting keys
}

void TransactionAddKeyValue::notify()
{
  _dsct->notifyKey(getVarName(),_key,_value);
}

TransactionAddKeyValue::~TransactionAddKeyValue()
{
  Py_XDECREF(_key);
  Py_XDECREF(_value);
}

TransactionAddKeyValueHard::TransactionAddKeyValueHard(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME::ByteVec& key, const SALOME::ByteVec& value):TransactionAddKeyValue(dsct,varName,key,value)
{
}

void TransactionAddKeyValueHard::perform()
{
  _varc->addKeyValueHard(_key,_value);
}

TransactionAddKeyValueErrorIfAlreadyExisting::TransactionAddKeyValueErrorIfAlreadyExisting(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME::ByteVec& key, const SALOME::ByteVec& value):TransactionAddKeyValue(dsct,varName,key,value)
{
  _varc->checkKeyNotAlreadyPresent(_key);
}

void TransactionAddKeyValueErrorIfAlreadyExisting::perform()
{
  _varc->addKeyValueErrorIfAlreadyExisting(_key,_value);
}

TransactionRemoveKeyInVarErrorIfNotAlreadyExisting::TransactionRemoveKeyInVarErrorIfNotAlreadyExisting(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME::ByteVec& key):TransactionDictModify(dsct,varName),_key(0)
{
  std::vector<unsigned char> key2;
  FromByteSeqToVB(key,key2);
  _key=PickelizedPyObjServer::GetPyObjFromPickled(key2,_dsct);
}

void TransactionRemoveKeyInVarErrorIfNotAlreadyExisting::perform()
{
  _varc->removeKeyInVarErrorIfNotAlreadyExisting(_key);
}

/*!
 * no implementation it is not a bug !
 */
void TransactionRemoveKeyInVarErrorIfNotAlreadyExisting::notify()
{
}

TransactionRemoveKeyInVarErrorIfNotAlreadyExisting::~TransactionRemoveKeyInVarErrorIfNotAlreadyExisting()
{
  Py_XDECREF(_key);
}

TransactionMorphRdWrIntoRdOnly::TransactionMorphRdWrIntoRdOnly(DataScopeServerTransaction *dsct, const std::string& varName):Transaction(dsct,varName)
{
}

SALOME::PickelizedPyObjRdWrServer_ptr TransactionMorphRdWrIntoRdOnly::getVar()
{
  SALOME::BasicDataServer_var obj(_dsct->retrieveVarInternal(_var_name.c_str()));
  SALOME::PickelizedPyObjRdWrServer_ptr ret(SALOME::PickelizedPyObjRdWrServer::_narrow(obj));
  if(CORBA::is_nil(ret))
    {
      std::ostringstream oss; oss << "TransactionMorphRdWrIntoRdOnly::getVar : var \"" << _var_name << "\" has not expected PickelizedPyObjRdWrServer type !";
      throw Exception(oss.str());
    }
  return ret;
}

void TransactionMorphRdWrIntoRdOnly::prepareRollBackInCaseOfFailure()
{
  BasicDataServer *var(_dsct->retrieveVarInternal2(_var_name));
  if(!var)
    throw Exception("TransactionMorphRdWrIntoRdOnly::prepareRollBackInCaseOfFailure : Returned var is NULL !");
  PickelizedPyObjRdWrServer *varc(dynamic_cast<PickelizedPyObjRdWrServer *>(var));
  if(!varc)
    throw Exception("TransactionMorphRdWrIntoRdOnly::prepareRollBackInCaseOfFailure : Returned var has not expected type !");
}

void TransactionMorphRdWrIntoRdOnly::perform()
{
  _dsct->moveStatusOfVarFromRdWrToRdOnly(_var_name);
}

void TransactionMorphRdWrIntoRdOnly::rollBack()
{
  _dsct->moveStatusOfVarFromRdOnlyToRdWr(_var_name);
}

/*!
 * no implementation it is not a bug !
 */
void TransactionMorphRdWrIntoRdOnly::notify()
{
}

TransactionMultiKeyAddSession::TransactionMultiKeyAddSession(DataScopeServerTransaction *dsct, const std::string& varName):Transaction(dsct,varName)
{
  _dsct->moveStatusOfVarFromRdExtOrRdExtInitToRdExtInit(_var_name);
}

void TransactionMultiKeyAddSession::addKeyValueInVarErrorIfAlreadyExistingNow(const SALOME::ByteVec& key, const SALOME::ByteVec& value)
{
  _dsct->checkVarExistingAndDict(_var_name);
  TransactionAddKeyValueErrorIfAlreadyExisting ret(_dsct,_var_name,key,value);
  {
    ret.perform();
    /*bool mustRollback(true);
    TrustTransaction t;
    t.setTransaction(&ret,&mustRollback);
    t.operate();
    mustRollback=false;//important let this line to notify t that everything was OK*/
  }
  ret.notify();
}

/*!
 * no implementation it is not a bug !
 */
void TransactionMultiKeyAddSession::prepareRollBackInCaseOfFailure()
{
}

void TransactionMultiKeyAddSession::perform()
{
  _dsct->moveStatusOfVarFromRdExtOrRdExtInitToRdExt(_var_name);
}

/*!
 * no implementation it is not a bug !
 */
void TransactionMultiKeyAddSession::rollBack()
{
}

/*!
 * no implementation it is not a bug !
 */
void TransactionMultiKeyAddSession::notify()
{
}

