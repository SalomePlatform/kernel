// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
/*
 * BatchManagerCatalog.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2004
 * Projet : SALOME 2
 *
 */

#include <string>
#include <sstream>
#include <map>
#include "Batch_BatchManagerCatalog.hxx"
#include "Batch_FactBatchManager.hxx"
using namespace std;

namespace Batch {

  pthread_mutex_t BatchManagerCatalog::_mutex = PTHREAD_MUTEX_INITIALIZER;
  std::map<string, FactBatchManager *> * BatchManagerCatalog::_p_catalog = 0;

  // Constructeur
  BatchManagerCatalog::BatchManagerCatalog()
  {
    // Nothing to do
  }

  // Destructeur
  BatchManagerCatalog::~BatchManagerCatalog()
  {
    // Nothing to do
  }

  // Functor
  FactBatchManager * BatchManagerCatalog::getFactBatchManager(const char * type)
  {
    return (* BatchManagerCatalog::_p_catalog)[type];
  }

  void BatchManagerCatalog::addFactBatchManager(const char * type, FactBatchManager * pFBM)
  {
    if (pFBM) { // *** section critique ***
      pthread_mutex_lock(&_mutex);

      if (! BatchManagerCatalog::_p_catalog) BatchManagerCatalog::_p_catalog = new std::map<string, FactBatchManager *>;
      (*BatchManagerCatalog::_p_catalog)[type] = pFBM;

      pthread_mutex_unlock(&_mutex);
    }
  }

  FactBatchManager * BatchManagerCatalog::operator() (const char * type) const
  {
    return BatchManagerCatalog::getFactBatchManager(type);
  }

  std::map<string, FactBatchManager *> * BatchManagerCatalog::dict() const
  {
    return _p_catalog;
  }

  string BatchManagerCatalog::__repr__() const
  {
    ostringstream oss;
    oss << "<BatchManagerCatalog contains {";
    string sep;
    for(std::map<string, FactBatchManager *>::const_iterator it = (*_p_catalog).begin(); it != (*_p_catalog).end(); it++, sep=", ") {
      oss << sep << "'" << (*it).first << "' : '" << (*it).second->__repr__() << "'";
    }
    oss << "}>";
    return oss.str();
  }

}
