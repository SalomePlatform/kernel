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

#ifndef __SALOMESDS_TRANSACTION_HXX__
#define __SALOMESDS_TRANSACTION_HXX__

#include "SALOMEconfig.h"
#include CORBA_SERVER_HEADER(SALOME_SDS)

#include "SALOMESDS_Defines.hxx"
#include "SALOMESDS_Exception.hxx"
#include "SALOMESDS_DataServerManager.hxx"

#include <string>
#include <vector>

namespace SALOMESDS
{
  class PickelizedPyObjServer;
  class PickelizedPyObjRdWrServer;
  class PickelizedPyObjRdExtServer;

  class SALOMESDS_EXPORT Transaction : public virtual POA_SALOME::Transaction, public POAHolder
  {
  public:
    Transaction(DataScopeServerTransaction *dsct, const std::string& varName):_dsct(dsct),_var_name(varName) { if(!_dsct) throw Exception("Transaction constructor error !"); }
    std::string getVarName() const { return _var_name; }
    void checkVarExisting() { _dsct->checkExistingVar(_var_name); }
    void checkNotAlreadyExisting() { _dsct->checkNotAlreadyExistingVar(_var_name); }
    PortableServer::POA_var getPOA() const { return _dsct->getPOA(); }
    virtual void prepareRollBackInCaseOfFailure() = 0;
    virtual void perform() = 0;
    virtual void rollBack() = 0;
    virtual void notify() = 0;
    virtual ~Transaction();
  public:
    static void FromByteSeqToVB(const SALOME::ByteVec& bsToBeConv, std::vector<unsigned char>& ret);
    static void FromVBToByteSeq(const std::vector<unsigned char>& bsToBeConv, SALOME::ByteVec& ret);
  protected:
    DataScopeServerTransaction *_dsct;
    std::string _var_name;
  };

  class TransactionVarCreate : public Transaction
  {
  public:
    TransactionVarCreate(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME::ByteVec& constValue);
    void prepareRollBackInCaseOfFailure();
    void rollBack();
    void notify();
  protected:
    std::vector<unsigned char> _data;
  };

  class TransactionRdOnlyVarCreate : public TransactionVarCreate
  {
  public:
    TransactionRdOnlyVarCreate(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME::ByteVec& constValue):TransactionVarCreate(dsct,varName,constValue) { }
    void perform();
  };

  class TransactionRdExtVarCreate : public TransactionVarCreate
  {
  public:
    TransactionRdExtVarCreate(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME::ByteVec& constValue):TransactionVarCreate(dsct,varName,constValue) { }
    void perform();
  };

  class TransactionRdExtInitVarCreate : public TransactionVarCreate
  {
  public:
    TransactionRdExtInitVarCreate(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME::ByteVec& constValue):TransactionVarCreate(dsct,varName,constValue) { }
    void perform();
  };

  class TransactionRdWrVarCreate : public TransactionVarCreate
  {
  public:
    TransactionRdWrVarCreate(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME::ByteVec& constValue):TransactionVarCreate(dsct,varName,constValue) { }
    void perform();
  };

  class TransactionKillVar : public Transaction
  {
  public:
    TransactionKillVar(DataScopeServerTransaction *dsct, const std::string& varName);
    void prepareRollBackInCaseOfFailure();
    void perform();
    void rollBack();
    void notify();
  };

  class PickelizedPyObjServer;

  class TransactionDictModify : public Transaction
  {
  public:
    TransactionDictModify(DataScopeServerTransaction *dsct, const std::string& varName);
    PickelizedPyObjServer *checkVarExistingAndDict() { return _dsct->checkVarExistingAndDict(_var_name); }
    void prepareRollBackInCaseOfFailure();
    void rollBack();
  protected:
    std::string _zeDataBefore;
    PickelizedPyObjServer *_varc;
  };

  class TransactionAddKeyValue : public TransactionDictModify
  {
  public:
    TransactionAddKeyValue(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME::ByteVec& key, const SALOME::ByteVec& value);
    void prepareRollBackInCaseOfFailure();
    void notify();
    ~TransactionAddKeyValue();
  protected:
    PyObject *_key;
    PyObject *_value;
  };

  class TransactionAddKeyValueHard : public TransactionAddKeyValue
  {
  public:
    TransactionAddKeyValueHard(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME::ByteVec& key, const SALOME::ByteVec& value);
    void perform();
  };

  class TransactionAddKeyValueErrorIfAlreadyExisting : public TransactionAddKeyValue
  {
  public:
    TransactionAddKeyValueErrorIfAlreadyExisting(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME::ByteVec& key, const SALOME::ByteVec& value);
    void perform();
  };

  class TransactionRemoveKeyInVarErrorIfNotAlreadyExisting : public TransactionDictModify
  {
  public:
    TransactionRemoveKeyInVarErrorIfNotAlreadyExisting(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME::ByteVec& key);
    void perform();
    void notify();
    ~TransactionRemoveKeyInVarErrorIfNotAlreadyExisting();
  private:
    PyObject *_key;
  };

  class TransactionMorphRdWrIntoRdOnly : public Transaction, public virtual POA_SALOME::TransactionRdWrAccess
  {
  public:
    TransactionMorphRdWrIntoRdOnly(DataScopeServerTransaction *dsct, const std::string& varName);
  public:
    SALOME::PickelizedPyObjRdWrServer_ptr getVar();
  public:
    void prepareRollBackInCaseOfFailure();
    void perform();
    void rollBack();
    void notify();
  };

  /*!
   * This transaction switch from RdExt to RdExtInit in constructor and when perform called RdExtInit to RdExt.
   */
  class TransactionMultiKeyAddSession : public Transaction, public virtual POA_SALOME::TransactionMultiKeyAddSession
  {
  public:
    TransactionMultiKeyAddSession(DataScopeServerTransaction *dsct, const std::string& varName);
  public://remotely callable
    void addKeyValueInVarErrorIfAlreadyExistingNow(const SALOME::ByteVec& key, const SALOME::ByteVec& value);
  public:
    void prepareRollBackInCaseOfFailure();
    void perform();
    void rollBack();
    void notify();
  };
}

#endif
