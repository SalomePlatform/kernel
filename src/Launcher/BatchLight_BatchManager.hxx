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
 * BatchManager.hxx : 
 *
 * Auteur : Bernard SECHER - CEA/DEN
 * Date   : Juillet 2007
 * Projet : SALOME
 *
 */

#ifndef _BL_BATCHMANAGER_H_
#define _BL_BATCHMANAGER_H_

#include <vector>
#include <map>
#include <string>
#include "Utils_SALOME_Exception.hxx"
#include <SALOMEconfig.h>
#include <stdlib.h>
#include CORBA_CLIENT_HEADER(SALOME_ContainerManager)
#include "MpiImpl.hxx"

namespace BatchLight {

  class Job;

  struct batchParams{
    std::string hostname; // serveur ou tourne le BatchManager
    std::string protocol; // protocole d'acces au serveur: ssh ou rsh
    std::string username; // username d'acces au serveur
    std::string applipath; // path of apllication directory on server
    std::vector<std::string> modulesList; // list of Salome modules installed on server
    unsigned int nbnodes; // number of nodes on cluster
    unsigned int nbprocpernode; // number of processors on each node
    std::string mpiImpl; // mpi implementation
  };

  class BatchManager
  {
  public:
    // Constructeur et destructeur
    BatchManager(const batchParams& p) throw(SALOME_Exception); // connexion a la machine host
    virtual ~BatchManager();

    // Methodes pour le controle des jobs : virtuelles pures
    const int submitJob(BatchLight::Job* job); // soumet un job au gestionnaire
    virtual void deleteJob(const int & jobid) = 0; // retire un job du gestionnaire
    virtual std::string queryJob(const int & jobid) = 0; // renvoie l'etat du job
    void importOutputFiles( const char *directory, const CORBA::Long jobId ) throw(SALOME_Exception);

  protected:
    batchParams _params;
    MpiImpl *_mpiImpl;
    std::map <int,const BatchLight::Job *> _jobmap;

    virtual int submit(BatchLight::Job* job) throw(SALOME_Exception) = 0;
    void setDirForTmpFiles(BatchLight::Job* job);
    void exportInputFiles(BatchLight::Job* job) throw(SALOME_Exception);
    virtual void buildSalomeCouplingScript(BatchLight::Job* job) throw(SALOME_Exception) = 0;
    virtual void buildSalomeBatchScript(BatchLight::Job* job) throw(SALOME_Exception) = 0;

    std::string BuildTemporaryFileName() const;
    void RmTmpFile(std::string & TemporaryFileName);
    MpiImpl *FactoryMpiImpl(std::string mpiImpl) throw(SALOME_Exception);
  private:

  };

}

#endif
