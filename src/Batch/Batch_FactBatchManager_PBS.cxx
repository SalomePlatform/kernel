/*
 * FactBatchManager_PBS.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2004
 * Projet : SALOME 2
 *
 */

#include <string>
#include "Batch_BatchManager_PBS.hxx"
#include "Batch_FactBatchManager_PBS.hxx"
#include "utilities.h"
using namespace std;

namespace Batch {

  static FactBatchManager_PBS sFBM_PBS;

  // Constructeur
  FactBatchManager_PBS::FactBatchManager_PBS() : FactBatchManager("PBS")
  {
    // Nothing to do
  }

  // Destructeur
  FactBatchManager_PBS::~FactBatchManager_PBS()
  {
    // Nothing to do
  }

  // Functor
  BatchManager_PBS * FactBatchManager_PBS::operator() (const char * hostname) const
  {
    MESSAGE("Building new BatchManager_PBS on host '" << hostname << "'");
    return new BatchManager_PBS(this, hostname);
  }


}
