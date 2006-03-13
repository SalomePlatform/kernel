/*
 * FactBatchManager_Local_SH.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2004
 * Projet : SALOME 2
 *
 */

#ifndef _FACTBATCHMANAGER_LOCAL_SH_H_
#define _FACTBATCHMANAGER_LOCAL_SH_H_

using namespace std;
#include <string>
#include <map>
#include "Batch_FactBatchManager.hxx"

namespace Batch {
  
  class BatchManager_Local_SH;

  class FactBatchManager_Local_SH : public FactBatchManager
  {
  public:
    // Constructeur et destructeur
    FactBatchManager_Local_SH();
    virtual ~FactBatchManager_Local_SH();

    virtual BatchManager * operator() (const char * hostname) const;

  protected:

  private:

  };

}

#endif
