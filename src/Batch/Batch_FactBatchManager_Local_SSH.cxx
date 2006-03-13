/*
 * FactBatchManager_Local_SSH.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2004
 * Projet : SALOME 2
 *
 */

#include <string>
#include "Batch_BatchManager_Local_SSH.hxx"
#include "Batch_FactBatchManager_Local_SSH.hxx"
//#include "utilities.h"

namespace Batch {

  static FactBatchManager_Local_SSH sFBM_Local_SSH;

  // Constructeur
  FactBatchManager_Local_SSH::FactBatchManager_Local_SSH() : FactBatchManager("SSH")
  {
    // Nothing to do
  }

  // Destructeur
  FactBatchManager_Local_SSH::~FactBatchManager_Local_SSH()
  {
    // Nothing to do
  }

  // Functor
  BatchManager * FactBatchManager_Local_SSH::operator() (const char * hostname) const
  {
    // MESSAGE("Building new BatchManager_Local_SSH on host '" << hostname << "'");
    return new BatchManager_Local_SSH(this, hostname);
  }


}
