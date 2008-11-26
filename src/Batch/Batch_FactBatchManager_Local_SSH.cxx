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
