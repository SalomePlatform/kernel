/*
 * BatchManagerCatalog.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2004
 * Projet : SALOME 2
 *
 */

#ifndef _CATALOG_H_
#define _CATALOG_H_

#include <string>
#include <map>
#include <pthread.h>

namespace Batch {

  class FactBatchManager;
  
  class BatchManagerCatalog
  {
  public:
    // Constructeur
    BatchManagerCatalog();
    // Destructeur
    virtual ~BatchManagerCatalog();

    static Batch::FactBatchManager * getFactBatchManager(const char * type);
    static void addFactBatchManager(const char * type, Batch::FactBatchManager * pFBM);
    virtual Batch::FactBatchManager * operator() (const char * type) const;

    virtual std::map<std::string, FactBatchManager *> * dict() const;
    virtual std::string __repr__() const;

  protected:
    static std::map<std::string, FactBatchManager *> * _p_catalog;
    static pthread_mutex_t _mutex;

  private:

  };

}

#endif
