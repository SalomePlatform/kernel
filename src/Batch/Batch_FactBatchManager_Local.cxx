/*
 * FactBatchManager_Local.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2004
 * Projet : SALOME 2
 *
 */

#include <string>
#include "Batch_BatchManager_Local.hxx"
#include "Batch_FactBatchManager_Local.hxx"
//#include "utilities.h"

namespace Batch {

//   static FactBatchManager_Local sFBM_Local;

  // Constructeur
  FactBatchManager_Local::FactBatchManager_Local() : FactBatchManager("Local")
  {
    // Nothing to do
  }

  // Destructeur
  FactBatchManager_Local::~FactBatchManager_Local()
  {
    // Nothing to do
  }

  // Functor
//   BatchManager * FactBatchManager_Local::operator() (const char * hostname) const
//   {
//     // MESSAGE("Building new BatchManager_Local on host '" << hostname << "'");
//     return new BatchManager_Local(this, hostname);
//   }


}
