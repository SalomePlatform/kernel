/*
 * FactBatchManager_Local_SH.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2004
 * Projet : SALOME 2
 *
 */

#include <string>
#include "Batch_BatchManager_Local_SH.hxx"
#include "Batch_FactBatchManager_Local_SH.hxx"
//#include "utilities.h"

namespace Batch {

  static FactBatchManager_Local_SH sFBM_Local_SH;

  // Constructeur
  FactBatchManager_Local_SH::FactBatchManager_Local_SH() : FactBatchManager("SH")
  {
    // Nothing to do
  }

  // Destructeur
  FactBatchManager_Local_SH::~FactBatchManager_Local_SH()
  {
    // Nothing to do
  }

  // Functor
  BatchManager * FactBatchManager_Local_SH::operator() (const char * hostname) const
  {
    // MESSAGE("Building new BatchManager_Local_SH on host '" << hostname << "'");
    return new BatchManager_Local_SH(this, hostname);
  }


}
