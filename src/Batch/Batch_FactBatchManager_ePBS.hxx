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
 * FactBatchManager_ePBS.hxx : 
 *
 * Auteur : Bernard SECHER : CEA DEN
 * Date   : Avril 2008
 * Projet : PAL Salome
 *
 */

#ifndef _FACTBATCHMANAGER_ePBS_H_
#define _FACTBATCHMANAGER_ePBS_H_

#include "Batch_Defines.hxx"

#include <string>
#include <map>
#include "Batch_BatchManager_eClient.hxx"
#include "Batch_FactBatchManager_eClient.hxx"

namespace Batch {
  
  class BatchManager_ePBS;

  class BATCH_EXPORT FactBatchManager_ePBS : public FactBatchManager_eClient
  {
  public:
    // Constructeur et destructeur
    FactBatchManager_ePBS();
    virtual ~FactBatchManager_ePBS();

    virtual BatchManager * operator() (const char * hostname) const;
    virtual BatchManager_eClient * operator() (const char * hostname, const char * protocol, const char * mpiImpl) const;

  protected:

  private:

  };

}

#endif
