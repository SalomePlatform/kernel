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
 * BatchManager_eLSF.hxx : emulation of client
 *
 * Auteur : Bernard SECHER - CEA DEN
 * Mail   : mailto:bernard.secher@cea.fr
 * Date   : Thu Apr 24 10:17:22 2008
 * Projet : PAL Salome
 *
 */

#ifndef _BATCHMANAGER_eClient_H_
#define _BATCHMANAGER_eClient_H_

#include "Batch_Defines.hxx"

#include "MpiImpl.hxx"
#include "Batch_BatchManager.hxx"

#include <string>

namespace Batch {

  class Job;

  class BATCH_EXPORT EmulationException
  {
  public:
    const std::string msg;

    EmulationException(const std::string m) : msg(m) {}
  };

  class BATCH_EXPORT BatchManager_eClient : public BatchManager
  {
  public:
    // Constructeur et destructeur
    BatchManager_eClient(const Batch::FactBatchManager * parent, const char* host="localhost", const char* protocol="ssh", const char* mpiImpl="mpich1");
    virtual ~BatchManager_eClient();
    void importOutputFiles( const Job & job, const std::string directory ) throw(EmulationException);

  protected:
    std::string _protocol; // protocol to access _hostname
    std::string _username; // username to access _hostname
    MpiImpl *_mpiImpl; // Mpi implementation to launch executable in batch script

    std::string createAndOpenTemporaryFile(std::ofstream & outputStream) const;
    MpiImpl* FactoryMpiImpl(std::string mpiImpl) throw(EmulationException);
    void exportInputFiles(const Job & job) throw(EmulationException);

  private:

  };

}

#endif
