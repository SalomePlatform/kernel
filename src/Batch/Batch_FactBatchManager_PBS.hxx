/*
 * FactBatchManager_PBS.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2004
 * Projet : SALOME 2
 *
 */

#ifndef _FACTBATCHMANAGER_PBS_H_
#define _FACTBATCHMANAGER_PBS_H_

#include "Batch_FactBatchManager.hxx"

namespace Batch {
  
  class BatchManager_PBS;

  class FactBatchManager_PBS : public FactBatchManager
  {
  public:
    // Constructeur et destructeur
    FactBatchManager_PBS();
    virtual ~FactBatchManager_PBS();

    virtual BatchManager_PBS * operator() (const char * hostname) const;

  protected:

  private:

  };

}

#endif
