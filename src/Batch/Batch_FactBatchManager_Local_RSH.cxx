/*
 * FactBatchManager_Local_RSH.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2004
 * Projet : SALOME 2
 *
 */

#include <string>
#include "Batch_BatchManager_Local_RSH.hxx"
#include "Batch_FactBatchManager_Local_RSH.hxx"
//#include "utilities.h"

namespace Batch {

  static FactBatchManager_Local_RSH sFBM_Local_RSH;

  // Constructeur
  FactBatchManager_Local_RSH::FactBatchManager_Local_RSH() : FactBatchManager("RSH")
  {
    // Nothing to do
  }

  // Destructeur
  FactBatchManager_Local_RSH::~FactBatchManager_Local_RSH()
  {
    // Nothing to do
  }

  // Functor
  BatchManager * FactBatchManager_Local_RSH::operator() (const char * hostname) const
  {
    // MESSAGE("Building new BatchManager_Local_RSH on host '" << hostname << "'");
    return new BatchManager_Local_RSH(this, hostname);
  }


}
