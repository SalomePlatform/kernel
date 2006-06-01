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
 * BatchManagerCatalog.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2004
 * Projet : SALOME 2
 *
 */

#ifndef _CATALOG_H_
#define _CATALOG_H_

#include <string>
#include <map>
#include <pthread.h>

namespace Batch {

  class FactBatchManager;
  
  class BatchManagerCatalog
  {
  public:
    // Constructeur
    BatchManagerCatalog();
    // Destructeur
    virtual ~BatchManagerCatalog();

    static Batch::FactBatchManager * getFactBatchManager(const char * type);
    static void addFactBatchManager(const char * type, Batch::FactBatchManager * pFBM);
    virtual Batch::FactBatchManager * operator() (const char * type) const;

    virtual std::map<std::string, FactBatchManager *> * dict() const;
    virtual std::string __repr__() const;

  protected:
    static std::map<std::string, FactBatchManager *> * _p_catalog;
    static pthread_mutex_t _mutex;

  private:

  };

}

#endif
