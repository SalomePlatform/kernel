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

using namespace std;
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

    static FactBatchManager * getFactBatchManager(const char * type);
    static void addFactBatchManager(const char * type, FactBatchManager * pFBM);
    virtual FactBatchManager * operator() (const char * type) const;

    virtual map<string, FactBatchManager *> * dict() const;
    virtual string __repr__() const;

  protected:
    static map<string, FactBatchManager *> * _p_catalog;
    static pthread_mutex_t _mutex;

  private:

  };

}

#endif
