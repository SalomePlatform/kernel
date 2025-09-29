// Copyright (C) 2007-2025  CEA, EDF, OPEN CASCADE
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
#include "SALOMESDS_Auto.hxx"

#include <string>
#include <vector>

namespace SALOMESDS
{
  class PickelizedPyObjServer;
  class PickelizedPyObjRdWrServer;
  class PickelizedPyObjRdExtServer;

  class SALOMESDS_EXPORT Transaction : public virtual POA_SALOME_CMOD::Transaction, public POAHolder
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
    static void FromByteSeqToVB(const SALOME_CMOD::ByteVec& bsToBeConv, std::vector<unsigned char>& ret);
    static void FromVBToByteSeq(const std::vector<unsigned char>& bsToBeConv, SALOME_CMOD::ByteVec& ret);
  protected:
    DataScopeServerTransaction *_dsct;
    std::string _var_name;
  };

  class TransactionVarCreate : public Transaction
  {
  public:
    TransactionVarCreate(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME_CMOD::ByteVec& constValue);
    void prepareRollBackInCaseOfFailure() override;
    void rollBack() override;
    void notify() override;
  protected:
    std::vector<unsigned char> _data;
  };

  class TransactionRdOnlyVarCreate : public TransactionVarCreate
  {
  public:
    TransactionRdOnlyVarCreate(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME_CMOD::ByteVec& constValue):TransactionVarCreate(dsct,varName,constValue) { }
    void perform();
  };

  class TransactionRdExtVarCreate : public TransactionVarCreate
  {
  public:
    TransactionRdExtVarCreate(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME_CMOD::ByteVec& constValue):TransactionVarCreate(dsct,varName,constValue) { }
    void perform() override;
  };

  class TransactionRdExtVarFreeStyleCreate : public TransactionRdExtVarCreate
  {
  public:
    TransactionRdExtVarFreeStyleCreate(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME_CMOD::ByteVec& constValue, const char *compareFuncContent);
    void prepareRollBackInCaseOfFailure() override;
    void rollBack() override;
    void perform() override;
  protected:
    bool _null_rollback = false;
    std::string _cmp_func_content;
    SALOME::AutoPyRef _cmp_func;
  };

  class TransactionRdExtInitVarCreate : public TransactionVarCreate
  {
  public:
    TransactionRdExtInitVarCreate(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME_CMOD::ByteVec& constValue):TransactionVarCreate(dsct,varName,constValue) { }
    void perform() override;
  };

  class TransactionRdWrVarCreate : public TransactionVarCreate
  {
  public:
    TransactionRdWrVarCreate(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME_CMOD::ByteVec& constValue):TransactionVarCreate(dsct,varName,constValue) { }
    void perform() override;
  };

  class TransactionKillVar : public Transaction
  {
  public:
    TransactionKillVar(DataScopeServerTransaction *dsct, const std::string& varName);
    void prepareRollBackInCaseOfFailure() override;
    void perform() override;
    void rollBack() override;
    void notify() override;
  };

  class PickelizedPyObjServer;

  class TransactionDictModify : public Transaction
  {
  public:
    TransactionDictModify(DataScopeServerTransaction *dsct, const std::string& varName);
    PickelizedPyObjServer *checkVarExistingAndDict() { return _dsct->checkVarExistingAndDict(_var_name); }
    void prepareRollBackInCaseOfFailure() override;
    void rollBack() override;
  protected:
    std::string _zeDataBefore;
    PickelizedPyObjServer *_varc;
  };

  class TransactionAddKeyValue : public TransactionDictModify
  {
  public:
    TransactionAddKeyValue(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME_CMOD::ByteVec& key, const SALOME_CMOD::ByteVec& value);
    void prepareRollBackInCaseOfFailure() override;
    void notify() override;
    ~TransactionAddKeyValue();
  protected:
    PyObject *_key;
    PyObject *_value;
  };

  class TransactionAddKeyValueHard : public TransactionAddKeyValue
  {
  public:
    TransactionAddKeyValueHard(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME_CMOD::ByteVec& key, const SALOME_CMOD::ByteVec& value);
    void perform() override;
  };

  class TransactionAddKeyValueErrorIfAlreadyExisting : public TransactionAddKeyValue
  {
  public:
    TransactionAddKeyValueErrorIfAlreadyExisting(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME_CMOD::ByteVec& key, const SALOME_CMOD::ByteVec& value);
    void perform() override;
  };

  class TransactionRemoveKeyInVarErrorIfNotAlreadyExisting : public TransactionDictModify
  {
  public:
    TransactionRemoveKeyInVarErrorIfNotAlreadyExisting(DataScopeServerTransaction *dsct, const std::string& varName, const SALOME_CMOD::ByteVec& key);
    void perform() override;
    void notify() override;
    ~TransactionRemoveKeyInVarErrorIfNotAlreadyExisting();
  private:
    PyObject *_key;
  };

  class TransactionMorphRdWrIntoRdOnly : public Transaction, public virtual POA_SALOME_CMOD::TransactionRdWrAccess
  {
  public:
    TransactionMorphRdWrIntoRdOnly(DataScopeServerTransaction *dsct, const std::string& varName);
  public:
    SALOME_CMOD::PickelizedPyObjRdWrServer_ptr getVar();
  public:
    void prepareRollBackInCaseOfFailure();
    void perform() override;
    void rollBack() override;
    void notify() override;
  };

  /*!
   * This transaction switch from RdExt to RdExtInit in constructor and when perform called RdExtInit to RdExt.
   */
  class TransactionMultiKeyAddSession : public Transaction, public virtual POA_SALOME_CMOD::TransactionMultiKeyAddSession
  {
  public:
    TransactionMultiKeyAddSession(DataScopeServerTransaction *dsct, const std::string& varName);
  public://remotely callable
    void addKeyValueInVarErrorIfAlreadyExistingNow(const SALOME_CMOD::ByteVec& key, const SALOME_CMOD::ByteVec& value);
  public:
    void prepareRollBackInCaseOfFailure() override;
    void perform() override;
    void rollBack() override;
    void notify() override;
  };
}

#endif
