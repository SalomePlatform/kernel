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
 * BatchManager_eLSF.hxx : emulation of LSF client
 *
 * Auteur : Bernard SECHER - CEA DEN
 * Mail   : mailto:bernard.secher@cea.fr
 * Date   : Thu Apr 24 10:17:22 2008
 * Projet : PAL Salome
 *
 */

#ifndef _BATCHMANAGER_eLSF_H_
#define _BATCHMANAGER_eLSF_H_

#include "Batch_Defines.hxx"

#include "Batch_JobId.hxx"
#include "Batch_JobInfo.hxx"
#include "Batch_JobInfo_eLSF.hxx"
#include "Batch_InvalidArgumentException.hxx"
#include "Batch_ConnexionFailureException.hxx"
#include "Batch_APIInternalFailureException.hxx"
#include "Batch_NotYetImplementedException.hxx"
#include "Batch_BatchManager.hxx"
#include "Batch_BatchManager_eClient.hxx"

namespace Batch {

  class Job;
  class JobId;
  class JobInfo;
  class FactBatchManager;

  class BATCH_EXPORT BatchManager_eLSF : public BatchManager_eClient
  {
  public:
    // Constructeur et destructeur
    BatchManager_eLSF(const FactBatchManager * parent, const char * host="localhost", const char * protocol="ssh", const char * mpiImpl="nompi") throw(InvalidArgumentException,ConnexionFailureException); // connexion a la machine host
    virtual ~BatchManager_eLSF();

    // Recupere le nom du serveur par defaut
    // static string BatchManager_LSF::getDefaultServer();

    // Methodes pour le controle des jobs
    virtual const JobId submitJob(const Job & job); // soumet un job au gestionnaire
    virtual void deleteJob(const JobId & jobid); // retire un job du gestionnaire
    virtual void holdJob(const JobId & jobid); // suspend un job en file d'attente
    virtual void releaseJob(const JobId & jobid); // relache un job suspendu
    virtual void alterJob(const JobId & jobid, const Parametre & param, const Environnement & env); // modifie un job en file d'attente
    virtual void alterJob(const JobId & jobid, const Parametre & param); // modifie un job en file d'attente
    virtual void alterJob(const JobId & jobid, const Environnement & env); // modifie un job en file d'attente
    virtual JobInfo queryJob(const JobId & jobid); // renvoie l'etat du job
    virtual bool isRunning(const JobId & jobid); // teste si un job est present en machine

    virtual void setParametre(const JobId & jobid, const Parametre & param) { return alterJob(jobid, param); } // modifie un job en file d'attente
    virtual void setEnvironnement(const JobId & jobid, const Environnement & env) { return alterJob(jobid, env); } // modifie un job en file d'attente


  protected:
    void buildBatchScript(const Job & job);
    std::string getWallTime(const long edt);

  private:

#ifdef SWIG
  public:
    // Recupere le l'identifiant d'un job deja soumis au BatchManager
    //virtual const JobId getJobIdByReference(const string & ref) { return BatchManager::getJobIdByReference(ref); }
    virtual const JobId getJobIdByReference(const char * ref) { return BatchManager::getJobIdByReference(ref); }
#endif

  };

}

#endif
