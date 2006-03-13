/*
 * BatchManager_Local.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Thu Nov  6 10:17:22 2003
 * Projet : Salome 2
 *
 */

#ifndef _BATCHMANAGER_LOCAL_H_
#define _BATCHMANAGER_LOCAL_H_


#include <vector>
#include <map>
#include <queue>
#include <deque>
#include <pthread.h>
#include "Batch_Job.hxx"
#include "Batch_JobId.hxx"
#include "Batch_JobInfo.hxx"
#include "Batch_JobInfo_Local.hxx"
#include "Batch_Job_Local.hxx"
#include "Batch_InvalidArgumentException.hxx"
#include "Batch_ConnexionFailureException.hxx"
#include "Batch_APIInternalFailureException.hxx"
#include "Batch_NotYetImplementedException.hxx"
#include "Batch_BatchManager.hxx"

namespace Batch {

  class FactBatchManager;

  class BatchManager_Local : public BatchManager
  {
  private:
    friend class ThreadAdapter;
    class ThreadAdapter{
    public:
      ThreadAdapter(BatchManager_Local & bm, const Job_Local & job);
      static void * run(void * arg);
      BatchManager_Local & getBatchManager() const { return _bm; };

    protected:
      BatchManager_Local & _bm;
      const Job_Local _job;

    private:
      void pere(pid_t child);
      void fils();

    };

    typedef int Id;

    enum Commande {
      NOP = 0,
      HOLD,
      RELEASE,
      TERM,
      KILL,
      ALTER,
    };

    enum Status {
      UNKNOWN = 0,
      RUNNING,
      STOPPED,
      DONE,
      DEAD,
    };

    struct Child {
      pthread_t thread_id;
      queue<Commande, deque<Commande> > command_queue;
      pid_t pid;
      int exit_code;
      Status status;
      Parametre param;
      Environnement env;
    };



  public:
    // Constructeur et destructeur
    BatchManager_Local(const FactBatchManager * parent, const char * host="localhost") throw(InvalidArgumentException,ConnexionFailureException); // connexion a la machine host
    virtual ~BatchManager_Local();

    // Recupere le nom du serveur par defaut
    // static string BatchManager_Local::getDefaultServer();

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
    int _connect; // Local connect id
    pthread_mutex_t _threads_mutex;
    map<Id, Child > _threads;

    // Methode abstraite qui renvoie la commande de copie du fichier source en destination
    virtual string copy_command(const string & host_source, const string & source, const string & host_destination, const string & destination) const = 0;

    // Methode abstraite qui renvoie la commande a executer
    virtual string exec_command(Parametre & param) const = 0;

    // Methode abstraite qui renvoie la commande d'effacement du fichier
    virtual string remove_command(const string & host_destination, const string & destination) const = 0;

  private:
    virtual pthread_t submit(const Job_Local & job);
    virtual void cancel(pthread_t thread_id);
    static  void kill_child_on_exit(void * p_pid);
    static  void delete_on_exit(void * arg);
    Id nextId(); // Retourne un identifiant unique pour un thread (clef de la map)
    Id getIdByThread_id(pthread_t thread_id);
    Id registerThread_id(pthread_t thread_id);
    pthread_mutex_t _thread_id_id_association_mutex;
    pthread_cond_t  _thread_id_id_association_cond;
    map<pthread_t, Id> _thread_id_id_association;

#ifdef SWIG
  public:
    // Recupere le l'identifiant d'un job deja soumis au BatchManager
    //virtual const JobId getJobIdByReference(const string & ref) { return BatchManager::getJobIdByReference(ref); }
    virtual const JobId getJobIdByReference(const char * ref) { return BatchManager::getJobIdByReference(ref); }
#endif

  };

}

#endif
