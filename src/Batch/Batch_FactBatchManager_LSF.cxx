/*
 * FactBatchManager_LSF.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2004
 * Projet : SALOME 2
 *
 */

#include <string>
#include "Batch_BatchManager_LSF.hxx"
#include "Batch_FactBatchManager_LSF.hxx"
//#include "utilities.h"

namespace Batch {

  static FactBatchManager_LSF sFBM_LSF;

  // Constructeur
  FactBatchManager_LSF::FactBatchManager_LSF() : FactBatchManager("LSF")
  {
    // Nothing to do
  }

  // Destructeur
  FactBatchManager_LSF::~FactBatchManager_LSF()
  {
    // Nothing to do
  }

  // Functor
  BatchManager * FactBatchManager_LSF::operator() (const char * hostname) const
  {
    // MESSAGE("Building new BatchManager_LSF on host '" << hostname << "'");
    return new BatchManager_LSF(this, hostname);
  }


}
