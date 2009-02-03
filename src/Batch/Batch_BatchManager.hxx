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
 * BatchManager.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#ifndef _BATCHMANAGER_H_
#define _BATCHMANAGER_H_

#include "Batch_Defines.hxx"

#include <string>
#include <map>
#include "Batch_Job.hxx"
#include "Batch_JobId.hxx"
#include "Batch_JobInfo.hxx"
#include "Batch_InvalidArgumentException.hxx"

namespace Batch {

  class Job;
  class JobId;
  class JobInfo;
  class FactBatchManager;

  class BATCH_EXPORT BatchManager
  {
  public:
    // Constructeur et destructeur
    //BatchManager(std::string host="localhost") throw(InvalidArgumentException); // connexion a la machine host
    BatchManager(const Batch::FactBatchManager * parent, const char * host="localhost") throw(InvalidArgumentException); // connexion a la machine host
    virtual ~BatchManager();
    virtual std::string __repr__() const;

    // Recupere le l'identifiant d'un job deja soumis au BatchManager
    //virtual const JobId getJobIdByReference(const std::string & ref);
    virtual const Batch::JobId getJobIdByReference(const char * ref);

    // Methodes pour le controle des jobs : virtuelles pures
    virtual const Batch::JobId submitJob(const Batch::Job & job) = 0; // soumet un job au gestionnaire
    virtual void deleteJob(const Batch::JobId & jobid) = 0; // retire un job du gestionnaire
    virtual void holdJob(const Batch::JobId & jobid) = 0; // suspend un job en file d'attente
    virtual void releaseJob(const Batch::JobId & jobid) = 0; // relache un job suspendu
    virtual void alterJob(const Batch::JobId & jobid, const Batch::Parametre & param, const Batch::Environnement & env) = 0; // modifie un job en file d'attente
    virtual void alterJob(const Batch::JobId & jobid, const Batch::Parametre & param) = 0; // modifie un job en file d'attente
    virtual void alterJob(const Batch::JobId & jobid, const Batch::Environnement & env) = 0; // modifie un job en file d'attente
    virtual Batch::JobInfo queryJob(const Batch::JobId & jobid) = 0; // renvoie l'etat du job

  protected:
    std::string _hostname; // serveur ou tourne le BatchManager
    // std::map< const std::string, const Batch::JobId * > jobid_map; // table des jobs deja soumis
    std::map< std::string, const Batch::JobId * > jobid_map; // table des jobs deja soumis
    const Batch::FactBatchManager * _parent;

  private:

  };

}

#endif
