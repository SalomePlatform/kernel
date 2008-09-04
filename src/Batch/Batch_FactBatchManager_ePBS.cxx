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
 * FactBatchManager_ePBS.cxx : 
 *
 * Auteur : Bernard SECHER - CEA DEN
 * Date   : Avril 2008
 * Projet : PAL Salome
 *
 */

#include <string>
#include "Batch_BatchManager_ePBS.hxx"
#include "Batch_FactBatchManager_ePBS.hxx"
//#include "utilities.h"

namespace Batch {

  static FactBatchManager_ePBS* instanceOfFBM_eLSF()
  {
    static FactBatchManager_ePBS* sFBM_ePBS = 0;
    if ( !sFBM_ePBS )
      sFBM_ePBS = new FactBatchManager_ePBS();

    return sFBM_ePBS;
  }

  // Constructeur
  FactBatchManager_ePBS::FactBatchManager_ePBS() : FactBatchManager_eClient("ePBS")
  {
    // Nothing to do
  }

  // Destructeur
  FactBatchManager_ePBS::~FactBatchManager_ePBS()
  {
    // Nothing to do
  }

  // Functor
  BatchManager * FactBatchManager_ePBS::operator() (const char * hostname) const
  {
    // MESSAGE("Building new BatchManager_PBS on host '" << hostname << "'");
    return new BatchManager_ePBS(this, hostname);
  }

  BatchManager_eClient * FactBatchManager_ePBS::operator() (const char * hostname, const char * protocol, const char * mpiImpl) const
  {
    // MESSAGE("Building new BatchManager_PBS on host '" << hostname << "'");
    return new BatchManager_ePBS(this, hostname, protocol, mpiImpl);
  }


}
