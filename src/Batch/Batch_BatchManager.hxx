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

using namespace std;
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

  class BatchManager
  {
  public:
    // Constructeur et destructeur
    //BatchManager(string host="localhost") throw(InvalidArgumentException); // connexion a la machine host
    BatchManager(const FactBatchManager * parent, const char * host="localhost") throw(InvalidArgumentException); // connexion a la machine host
    virtual ~BatchManager();
    virtual string __repr__() const;

    // Recupere le l'identifiant d'un job deja soumis au BatchManager
    //virtual const JobId getJobIdByReference(const string & ref);
    virtual const JobId getJobIdByReference(const char * ref);

    // Methodes pour le controle des jobs : virtuelles pures
    virtual const JobId submitJob(const Job & job) = 0; // soumet un job au gestionnaire
    virtual void deleteJob(const JobId & jobid) = 0; // retire un job du gestionnaire
    virtual void holdJob(const JobId & jobid) = 0; // suspend un job en file d'attente
    virtual void releaseJob(const JobId & jobid) = 0; // relache un job suspendu
    virtual void alterJob(const JobId & jobid, const Parametre & param, const Environnement & env) = 0; // modifie un job en file d'attente
    virtual void alterJob(const JobId & jobid, const Parametre & param) = 0; // modifie un job en file d'attente
    virtual void alterJob(const JobId & jobid, const Environnement & env) = 0; // modifie un job en file d'attente
    virtual JobInfo queryJob(const JobId & jobid) = 0; // renvoie l'etat du job

  protected:
    string _hostname; // serveur ou tourne le BatchManager
    map< const string, const JobId * > jobid_map; // table des jobs deja soumis
    const FactBatchManager * _parent;

  private:

  };

}

#endif
