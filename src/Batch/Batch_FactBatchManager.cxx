/*
 * FactBatchManager.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2004
 * Projet : SALOME 2
 *
 */

#include <string>
#include <sstream>
#include "Batch_BatchManagerCatalog.hxx"
#include "Batch_FactBatchManager.hxx"
#include "utilities.h"
using namespace std;

namespace Batch {

  // Constructeur
  FactBatchManager::FactBatchManager(const string & _t) : type(_t)
  {
    BatchManagerCatalog::addFactBatchManager(type.c_str(), this);
    ostringstream msg;
    msg << "FactBatchManager of type '" << type << "' inserted into catalog";
    MESSAGE(msg.str().c_str());
  }

  // Destructeur
  FactBatchManager::~FactBatchManager()
  {
    // Nothing to do
  }

  // Accesseur
  string FactBatchManager::getType() const
  {
    return type;
  }

  string FactBatchManager::__repr__() const
  {
    ostringstream oss;
    oss << "<FactBatchManager of type '" << type << "'>";
    return oss.str();
  }

}
