/*
 * FactBatchManager_Local.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2004
 * Projet : SALOME 2
 *
 */

#ifndef _FACTBATCHMANAGER_LOCAL_H_
#define _FACTBATCHMANAGER_LOCAL_H_

using namespace std;
#include <string>
#include <map>
#include "Batch_FactBatchManager.hxx"

namespace Batch {
  
  class BatchManager_Local;

  class FactBatchManager_Local : public FactBatchManager
  {
  public:
    // Constructeur et destructeur
    FactBatchManager_Local();
    virtual ~FactBatchManager_Local();

    virtual BatchManager * operator() (const char * hostname) const = 0;

  protected:

  private:

  };

}

#endif
