//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
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
#include <iostream>
#include "Batch_BatchManagerCatalog.hxx"
#include "Batch_FactBatchManager.hxx"
using namespace std;

namespace Batch {

  // Constructeur
  FactBatchManager::FactBatchManager(const string & _t) : type(_t)
  {
    BatchManagerCatalog::addFactBatchManager(type.c_str(), this);
    ostringstream msg;
    msg << "FactBatchManager of type '" << type << "' inserted into catalog";
    cerr << msg.str().c_str() << endl;
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
