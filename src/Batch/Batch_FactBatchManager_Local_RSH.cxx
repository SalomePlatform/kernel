// Copyright (C) 2005  OPEN CASCADE, CEA, EDF R&D, LEG
//           PRINCIPIA R&D, EADS CCR, Lip6, BV, CEDRAT
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
  
  static FactBatchManager_Local_RSH* instanceOfFBM_eLSF()
  {
    static FactBatchManager_Local_RSH* sFBM_Local_RSH = 0;
    if ( !sFBM_Local_RSH )
      sFBM_Local_RSH = new FactBatchManager_Local_RSH();

    return sFBM_Local_RSH;
  }

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
