/*
 * FactBatchManager_Local_RSH.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2004
 * Projet : SALOME 2
 *
 */

#ifndef _FACTBATCHMANAGER_LOCAL_RSH_H_
#define _FACTBATCHMANAGER_LOCAL_RSH_H_

using namespace std;
#include <string>
#include <map>
#include "Batch_FactBatchManager.hxx"

namespace Batch {
  
  class BatchManager_Local_RSH;

  class FactBatchManager_Local_RSH : public FactBatchManager
  {
  public:
    // Constructeur et destructeur
    FactBatchManager_Local_RSH();
    virtual ~FactBatchManager_Local_RSH();

    virtual BatchManager * operator() (const char * hostname) const;

  protected:

  private:

  };

}

#endif
