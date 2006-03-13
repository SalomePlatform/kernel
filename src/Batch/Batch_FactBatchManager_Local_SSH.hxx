/*
 * FactBatchManager_Local_SSH.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2004
 * Projet : SALOME 2
 *
 */

#ifndef _FACTBATCHMANAGER_LOCAL_SSH_H_
#define _FACTBATCHMANAGER_LOCAL_SSH_H_

using namespace std;
#include <string>
#include <map>
#include "Batch_FactBatchManager.hxx"

namespace Batch {
  
  class BatchManager_Local_SSH;

  class FactBatchManager_Local_SSH : public FactBatchManager
  {
  public:
    // Constructeur et destructeur
    FactBatchManager_Local_SSH();
    virtual ~FactBatchManager_Local_SSH();

    virtual BatchManager * operator() (const char * hostname) const;

  protected:

  private:

  };

}

#endif
