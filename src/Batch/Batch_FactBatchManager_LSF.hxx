/*
 * FactBatchManager_LSF.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2004
 * Projet : SALOME 2
 *
 */

#ifndef _FACTBATCHMANAGER_LSF_H_
#define _FACTBATCHMANAGER_LSF_H_

using namespace std;
#include <string>
#include <map>
#include "Batch_FactBatchManager.hxx"

namespace Batch {
  
  class BatchManager_LSF;

  class FactBatchManager_LSF : public FactBatchManager
  {
  public:
    // Constructeur et destructeur
    FactBatchManager_LSF();
    virtual ~FactBatchManager_LSF();

    virtual BatchManager * operator() (const char * hostname) const;

  protected:

  private:

  };

}

#endif
