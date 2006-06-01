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
 * BatchManager_Local.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Thu Nov  6 10:17:22 2003
 * Projet : Salome 2
 *
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctime>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include "Batch_IOMutex.hxx"
#include "Batch_BatchManager_Local.hxx"

namespace Batch {


  // Constructeur
  BatchManager_Local::BatchManager_Local(const FactBatchManager * parent, const char * host) throw(InvalidArgumentException,ConnexionFailureException) : BatchManager(parent, host), _connect(0), _threads_mutex(), _threads(), _thread_id_id_association_mutex(), _thread_id_id_association_cond(), _thread_id_id_association()
  {
    pthread_mutex_init(&_threads_mutex, NULL);
    pthread_mutex_init(&_thread_id_id_association_mutex, NULL);
    pthread_cond_init(&_thread_id_id_association_cond, NULL);
  }

  // Destructeur
  BatchManager_Local::~BatchManager_Local()
  {
    pthread_mutex_destroy(&_threads_mutex);
    pthread_mutex_destroy(&_thread_id_id_association_mutex);
    pthread_cond_destroy(&_thread_id_id_association_cond);
  }

  // Methode pour le controle des jobs : soumet un job au gestionnaire
  const JobId BatchManager_Local::submitJob(const Job & job)
  {
    Job_Local jobLocal = job;

    pthread_t thread_id = submit(jobLocal);

    ostringstream oss;
    oss << getIdByThread_id(thread_id);

    JobId id(this, oss.str());

    return id;
  }

  // Methode pour le controle des jobs : retire un job du gestionnaire
  void BatchManager_Local::deleteJob(const JobId & jobid)
  {
    Id id;

    istringstream iss(jobid.getReference());
    iss >> id;

    // On retrouve le thread_id du thread
    pthread_t thread_id;

    // @@@ --------> SECTION CRITIQUE <-------- @@@
    pthread_mutex_lock(&_threads_mutex);
    if (_threads.find(id) != _threads.end()) 
      thread_id = _threads[id].thread_id;
    pthread_mutex_unlock(&_threads_mutex);
    // @@@ --------> SECTION CRITIQUE <-------- @@@

    cancel(thread_id);
  }
   
  // Methode pour le controle des jobs : suspend un job en file d'attente
  void BatchManager_Local::holdJob(const JobId & jobid)
  {
    Id id;
    istringstream iss(jobid.getReference());
    iss >> id;

    UNDER_LOCK( cout << "BatchManager is sending HOLD command to the thread " << id << endl );

    // On introduit une commande dans la queue du thread
    // @@@ --------> SECTION CRITIQUE <-------- @@@
    pthread_mutex_lock(&_threads_mutex);
    if (_threads.find(id) != _threads.end()) 
      _threads[id].command_queue.push(HOLD);
    pthread_mutex_unlock(&_threads_mutex);
    // @@@ --------> SECTION CRITIQUE <-------- @@@
  }

  // Methode pour le controle des jobs : relache un job suspendu
  void BatchManager_Local::releaseJob(const JobId & jobid)
  {
    Id id;
    istringstream iss(jobid.getReference());
    iss >> id;

    UNDER_LOCK( cout << "BatchManager is sending RELEASE command to the thread " << id << endl );

    // On introduit une commande dans la queue du thread
    // @@@ --------> SECTION CRITIQUE <-------- @@@
    pthread_mutex_lock(&_threads_mutex);
    if (_threads.find(id) != _threads.end()) 
      _threads[id].command_queue.push(RELEASE);
    pthread_mutex_unlock(&_threads_mutex);
     // @@@ --------> SECTION CRITIQUE <-------- @@@
 }


  // Methode pour le controle des jobs : modifie un job en file d'attente
  void BatchManager_Local::alterJob(const JobId & jobid, const Parametre & param, const Environnement & env)
  {
  }

  // Methode pour le controle des jobs : modifie un job en file d'attente
  void BatchManager_Local::alterJob(const JobId & jobid, const Parametre & param)
  {
    alterJob(jobid, param, Environnement());
  }

  // Methode pour le controle des jobs : modifie un job en file d'attente
  void BatchManager_Local::alterJob(const JobId & jobid, const Environnement & env)
  {
    alterJob(jobid, Parametre(), env);
  }



  // Methode pour le controle des jobs : renvoie l'etat du job
  JobInfo BatchManager_Local::queryJob(const JobId & jobid)
  {
    Id id;
    istringstream iss(jobid.getReference());
    iss >> id;

    Parametre param;
    Environnement env;

    //UNDER_LOCK( cout << "JobInfo BatchManager_Local::queryJob(const JobId & jobid) : AVANT section critique" << endl );
    // @@@ --------> SECTION CRITIQUE <-------- @@@
    pthread_mutex_lock(&_threads_mutex);
    param = _threads[id].param;
    env   = _threads[id].env;
    pthread_mutex_unlock(&_threads_mutex);
    // @@@ --------> SECTION CRITIQUE <-------- @@@
    //UNDER_LOCK( cout << "JobInfo BatchManager_Local::queryJob(const JobId & jobid) : APRES section critique" << endl );

    JobInfo_Local ji(param, env);
    return ji;
  }



  // Methode pour le controle des jobs : teste si un job est present en machine
  bool BatchManager_Local::isRunning(const JobId & jobid)
  {
    Id id;
    istringstream iss(jobid.getReference());
    iss >> id;

    Status status;

    //UNDER_LOCK( cout << "JobInfo BatchManager_Local::queryJob(const JobId & jobid) : AVANT section critique" << endl );
    // @@@ --------> SECTION CRITIQUE <-------- @@@
    pthread_mutex_lock(&_threads_mutex);
    status = _threads[id].status;
    pthread_mutex_unlock(&_threads_mutex);
    // @@@ --------> SECTION CRITIQUE <-------- @@@
    //UNDER_LOCK( cout << "JobInfo BatchManager_Local::queryJob(const JobId & jobid) : APRES section critique" << endl );

    return (status == RUNNING);
  }


  // Methode d'execution d'un job
  pthread_t BatchManager_Local::submit(const Job_Local & job)
  {
    // L'id du thread a creer
    pthread_t thread_id = 0;

    // Les attributs du thread a sa creation
    pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);

    ThreadAdapter * p_ta = new ThreadAdapter(*this, job);

    // Creation du thread qui va executer la commande systeme qu'on lui passe
    int rc = pthread_create(&thread_id, 
			    &thread_attr, 
			    &ThreadAdapter::run, 
			    static_cast<void *>(p_ta));
    if (rc) {
    }

    // Liberation des zones memoire maintenant inutiles occupees par les attributs du thread
    pthread_attr_destroy(&thread_attr);

    return thread_id;
  }


  // Methode de destruction d'un job
  void BatchManager_Local::cancel(pthread_t thread_id)
  {
    pthread_cancel(thread_id);
  }


  // Fabrique un identifiant unique pour les threads puisque le thread_id n'est pas unique 
  // au cours du temps (il peut etre reutilise lorsqu'un thread se termine)
  // ATTENTION : cette methode est uniquement protegee par la section critique de l'association
  // Thread_id / Id (_thread_id_id_association_mutex)
  BatchManager_Local::Id BatchManager_Local::nextId() 
  {
    static Id id = 0;
    Id nextId = id++;
    //UNDER_LOCK( cout << "BatchManager_Local::Id BatchManager_Local::nextId() : Id = " << nextId << endl );
    return nextId;
  }


  // Retourne l'Id enregistre dans l'association Thread_id / Id et le detruit immediatement
  BatchManager_Local::Id BatchManager_Local::getIdByThread_id(pthread_t thread_id)
  {
    Id id = -1;

    // @@@ --------> SECTION CRITIQUE <-------- @@@
    pthread_mutex_lock(&_thread_id_id_association_mutex);
    while (_thread_id_id_association.find(thread_id) == _thread_id_id_association.end()) 
      pthread_cond_wait(&_thread_id_id_association_cond, &_thread_id_id_association_mutex);

    id = _thread_id_id_association[thread_id];
    _thread_id_id_association.erase(thread_id);
    
    pthread_mutex_unlock(&_thread_id_id_association_mutex);
    // @@@ --------> SECTION CRITIQUE <-------- @@@

    //UNDER_LOCK( cout << "BatchManager_Local::Id BatchManager_Local::getIdByThread_id(pthread_t thread_id) : Id = " << id << " - thread_id = " << thread_id << endl );
    return id;
  }


  // Associe un Thread_id a un Id nouvellement cree
  BatchManager_Local::Id BatchManager_Local::registerThread_id(pthread_t thread_id) 
  {
    Id id = -1;

    // @@@ --------> SECTION CRITIQUE <-------- @@@
    pthread_mutex_lock(&_thread_id_id_association_mutex);
    if (_thread_id_id_association.find(thread_id) == _thread_id_id_association.end()) {
      id = _thread_id_id_association[thread_id] = nextId();
      pthread_cond_signal(&_thread_id_id_association_cond);

    } else {
      UNDER_LOCK( cerr << "ERROR : Pthread Inconstency. Two threads own the same thread_id." << endl );
    }
    pthread_mutex_unlock(&_thread_id_id_association_mutex);
    // @@@ --------> SECTION CRITIQUE <-------- @@@

    //UNDER_LOCK( cout << "BatchManager_Local::Id BatchManager_Local::registerThread_id(pthread_t thread_id) : Id = " << id << " - thread_id = " << thread_id << endl );
    return id;
  }


  // Constructeur de la classe ThreadAdapter
  BatchManager_Local::ThreadAdapter::ThreadAdapter(BatchManager_Local & bm, const Job_Local & job) :
    _bm(bm), _job(job)
  {
    // Nothing to do
  }



  // Methode d'execution du thread
  void * BatchManager_Local::ThreadAdapter::run(void * arg)
  {
    // On bloque tous les signaux pour ce thread
    sigset_t setmask;
    sigfillset(&setmask);
    pthread_sigmask(SIG_BLOCK, &setmask, NULL);

    
    // On autorise la terminaison differee du thread
    // (ces valeurs sont les valeurs par defaut mais on les force par precaution)
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,  NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    // On enregistre la fonction de suppression du fils en cas d'arret du thread
    // Cette fontion sera automatiquement appelee lorsqu'une demande d'annulation
    // sera prise en compte par pthread_testcancel()
    pid_t child;
    pthread_cleanup_push(BatchManager_Local::kill_child_on_exit, static_cast<void *> (&child));
    pthread_cleanup_push(BatchManager_Local::delete_on_exit, arg);

    ThreadAdapter * p_ta = static_cast<ThreadAdapter *>(arg);




    // Le code retour cumule (ORed) de tous les appels
    // Nul en cas de reussite de l'ensemble des operations
    int rc = 0;

    // Cette table contient la liste des fichiers a detruire a la fin du processus
    std::vector<string> files_to_delete;



    // On copie les fichiers d'entree pour le fils
    const Parametre param   = p_ta->_job.getParametre();
    Parametre::const_iterator it;

    // On initialise la variable workdir a la valeur du Current Working Directory
    char * cwd = new char [PATH_MAX];
    getcwd(cwd, PATH_MAX);
    string workdir = cwd;
    delete [] cwd;

    if ( (it = param.find(WORKDIR)) != param.end() ) {
      workdir = static_cast<string>( (*it).second );
    }

    string executionhost = string(param[EXECUTIONHOST]);

    if ( (it = param.find(INFILE)) != param.end() ) {
      Versatile V = (*it).second;
      Versatile::iterator Vit;

      for(Vit=V.begin(); Vit!=V.end(); Vit++) {
	CoupleType cpt  = *static_cast< CoupleType * >(*Vit);
	Couple cp       = cpt;
	string local    = cp.getLocal();
	string remote   = cp.getRemote();

	string copy_cmd = p_ta->getBatchManager().copy_command("", local, executionhost, workdir + "/" + remote);
	UNDER_LOCK( cout << "Copying : " << copy_cmd << endl );

	if (system(copy_cmd.c_str()) ) {
	  // Echec de la copie
	  rc |= 1;
	} else {
	  // On enregistre le fichier comme etant a detruire
	  files_to_delete.push_back(workdir + "/" + remote);
	}

      }
    }





    // On forke/exec un nouveau process pour pouvoir controler le fils
    // (plus finement qu'avec un appel system)
    // int rc = system(commande.c_str());
    child = fork();
    if (child < 0) { // erreur
      UNDER_LOCK( cerr << "Fork impossible (rc=" << child << ")" << endl );

    } else if (child > 0) { // pere
      p_ta->pere(child);

    } else { // fils
      p_ta->fils();
    }




    // On copie les fichiers de sortie du fils
    if ( (it = param.find(OUTFILE)) != param.end() ) {
      Versatile V = (*it).second;
      Versatile::iterator Vit;

      for(Vit=V.begin(); Vit!=V.end(); Vit++) {
	CoupleType cpt  = *static_cast< CoupleType * >(*Vit);
	Couple cp       = cpt;
	string local    = cp.getLocal();
	string remote   = cp.getRemote();

	string copy_cmd = p_ta->getBatchManager().copy_command(executionhost, workdir + "/" + remote, "", local);
	UNDER_LOCK( cout << "Copying : " << copy_cmd << endl );

	if (system(copy_cmd.c_str()) ) {
	  // Echec de la copie
	  rc |= 1;
	} else {
	  // On enregistre le fichier comme etant a detruire
	  files_to_delete.push_back(workdir + "/" + remote);
	}

      }
    }




    // On efface les fichiers d'entree et de sortie du fils si les copies precedentes ont reussi
    // ou si la creation du fils n'a pu avoir lieu
    if ( (rc == 0) || (child < 0) ) {
      std::vector<string>::const_iterator it;
      for(it=files_to_delete.begin(); it!=files_to_delete.end(); it++) {
	string remove_cmd = p_ta->getBatchManager().remove_command(executionhost, *it);
	UNDER_LOCK( cout << "Removing : " << remove_cmd << endl );
	system(remove_cmd.c_str());
      }
    }



    // On retire la fonction de nettoyage de la memoire
    pthread_cleanup_pop(0);

    // On retire la fonction de suppression du fils
    pthread_cleanup_pop(0);



    // On invoque la fonction de nettoyage de la memoire
    delete_on_exit(arg);

    UNDER_LOCK( cout << "Father is leaving" << endl );
    pthread_exit(NULL);

    return NULL;
  }




  void BatchManager_Local::ThreadAdapter::pere(pid_t child)
  {
    time_t child_starttime = time(NULL);

    // On enregistre le fils dans la table des threads
    pthread_t thread_id = pthread_self();
    Id id = _bm.registerThread_id(thread_id);

    Parametre param   = _job.getParametre();
    Environnement env = _job.getEnvironnement();

    ostringstream thread_id_sst;
    thread_id_sst << id;
    param[ID]         = thread_id_sst.str();
    param[STATE]      = "Running";
    param[PID]        = child;

    // @@@ --------> SECTION CRITIQUE <-------- @@@
    pthread_mutex_lock(&_bm._threads_mutex);
    _bm._threads[id].thread_id = thread_id;
    _bm._threads[id].pid       = child;
    _bm._threads[id].status    = RUNNING;
    _bm._threads[id].param     = param;
    _bm._threads[id].env       = env;
    _bm._threads[id].command_queue.push(NOP);
    pthread_mutex_unlock(&_bm._threads_mutex);
    // @@@ --------> SECTION CRITIQUE <-------- @@@





    // on boucle en attendant que le fils ait termine
    while (1) {
      int child_rc = 0;
      pid_t child_wait_rc = waitpid(child, &child_rc, WNOHANG /* | WUNTRACED */);
      if (child_wait_rc > 0) {
	if (WIFSTOPPED(child_rc)) {
	  // NOTA : pour rentrer dans cette section, il faut que le flag WUNTRACED 
	  // soit positionne dans l'appel a waitpid ci-dessus. Ce flag est couramment 
	  // desactive car s'il est possible de detecter l'arret d'un process, il est 
	  // plus difficile de detecter sa reprise.

	  // Le fils est simplement stoppe
	  // @@@ --------> SECTION CRITIQUE <-------- @@@
	  pthread_mutex_lock(&_bm._threads_mutex);
	  _bm._threads[id].status       = STOPPED;
	  _bm._threads[id].param[STATE] = "Stopped";
	  pthread_mutex_unlock(&_bm._threads_mutex);
	  // @@@ --------> SECTION CRITIQUE <-------- @@@
	  UNDER_LOCK( cout << "Father sees his child is STOPPED : " << child_wait_rc << endl );

	} 
	else {
	  // Le fils est termine, on sort de la boucle et du if englobant
	  // @@@ --------> SECTION CRITIQUE <-------- @@@
	  pthread_mutex_lock(&_bm._threads_mutex);
	  _bm._threads[id].status       = DONE;
	  _bm._threads[id].param[STATE] = "Done";
	  pthread_mutex_unlock(&_bm._threads_mutex);
	  // @@@ --------> SECTION CRITIQUE <-------- @@@
	  UNDER_LOCK( cout << "Father sees his child is DONE : " << child_wait_rc << " (child_rc=" << (WIFEXITED(child_rc) ? WEXITSTATUS(child_rc) : -1) << ")" << endl );
	  break;
	}
      }
      else if (child_wait_rc == -1) {
	// Le fils a disparu ...
	// @@@ --------> SECTION CRITIQUE <-------- @@@
	pthread_mutex_lock(&_bm._threads_mutex);
	_bm._threads[id].status       = DEAD;
	_bm._threads[id].param[STATE] = "Dead";
	pthread_mutex_unlock(&_bm._threads_mutex);
	// @@@ --------> SECTION CRITIQUE <-------- @@@
	UNDER_LOCK( cout << "Father sees his child is DEAD : " << child_wait_rc << " (Reason : " << strerror(errno) << ")" << endl );
	break;
      }



      // On teste si le thread doit etre detruit
      pthread_testcancel();



      // On regarde si le fils n'a pas depasse son temps (wallclock time)
      time_t child_currenttime = time(NULL);
      time_t child_elapsedtime = child_currenttime - child_starttime;
      if (param.find(MAXWALLTIME) != param.end()) {
	int maxwalltime = param[MAXWALLTIME];
	// 	  cout << "child_starttime          = " << child_starttime        << endl
	// 	       << "child_currenttime        = " << child_currenttime      << endl
	// 	       << "child_elapsedtime        = " << child_elapsedtime      << endl
	// 	       << "maxwalltime              = " << maxwalltime            << endl
	// 	       << "int(maxwalltime * 1.1)   = " << int(maxwalltime * 1.1) << endl;
	if (child_elapsedtime > int(maxwalltime * 1.1) ) { // On se donne 10% de marge avant le KILL
	  UNDER_LOCK( cout << "Father is sending KILL command to the thread " << id << endl );
	  // On introduit une commande dans la queue du thread
	  // @@@ --------> SECTION CRITIQUE <-------- @@@
	  pthread_mutex_lock(&_bm._threads_mutex);
	  if (_bm._threads.find(id) != _bm._threads.end()) 
	    _bm._threads[id].command_queue.push(KILL);
	  pthread_mutex_unlock(&_bm._threads_mutex);
	  // @@@ --------> SECTION CRITIQUE <-------- @@@


	} else if (child_elapsedtime > maxwalltime ) {
	  UNDER_LOCK( cout << "Father is sending TERM command to the thread " << id << endl );
	  // On introduit une commande dans la queue du thread
	  // @@@ --------> SECTION CRITIQUE <-------- @@@
	  pthread_mutex_lock(&_bm._threads_mutex);
	  if (_bm._threads.find(id) != _bm._threads.end()) 
	    _bm._threads[id].command_queue.push(TERM);
	  pthread_mutex_unlock(&_bm._threads_mutex);
	  // @@@ --------> SECTION CRITIQUE <-------- @@@
	}
      }



      // On regarde s'il y a quelque chose a faire dans la queue de commande
      // @@@ --------> SECTION CRITIQUE <-------- @@@
      pthread_mutex_lock(&_bm._threads_mutex);
      if (_bm._threads.find(id) != _bm._threads.end()) {
	while (_bm._threads[id].command_queue.size() > 0) {
	  Commande cmd = _bm._threads[id].command_queue.front();
	  _bm._threads[id].command_queue.pop();

	  switch (cmd) {
	  case NOP:
	    UNDER_LOCK( cout << "Father does nothing to his child" << endl );
	    break;

	  case HOLD:
	    UNDER_LOCK( cout << "Father is sending SIGSTOP signal to his child" << endl );
	    kill(child, SIGSTOP);
	    break;

	  case RELEASE:
	    UNDER_LOCK( cout << "Father is sending SIGCONT signal to his child" << endl );
	    kill(child, SIGCONT);
	    break;

	  case TERM:
	    UNDER_LOCK( cout << "Father is sending SIGTERM signal to his child" << endl );
	    kill(child, SIGTERM);
	    break;

	  case KILL:
	    UNDER_LOCK( cout << "Father is sending SIGKILL signal to his child" << endl );
	    kill(child, SIGKILL);
	    break;

	  case ALTER:
	    break;

	  default:
	    break;
	  }
	}
	  
      }
      pthread_mutex_unlock(&_bm._threads_mutex);
      // @@@ --------> SECTION CRITIQUE <-------- @@@

      // On fait une petite pause pour ne pas surcharger inutilement le processeur
      sleep(1);
	
    }


  }




  void BatchManager_Local::ThreadAdapter::fils()
  {
    Parametre param = _job.getParametre();
    Parametre::iterator it;

    try {

    // On se place dans le repertoire de travail
    if ( (it = param.find(WORKDIR)) != param.end() ) {
      string workdir = static_cast<string>( (*it).second );
      chdir(workdir.c_str());
    }




    // EXECUTABLE is MANDATORY, if missing, we exit with failure notification
    char * execpath = NULL;
    if (param.find(EXECUTABLE) != param.end()) {
      string executable = _bm.exec_command(param);
      execpath          = new char [executable.size() + 1];
      strncpy(execpath, executable.c_str(), executable.size() + 1);
    } else exit(1); 

    string debug_command = execpath;

    string name = (param.find(NAME) != param.end()) ? param[NAME] : param[EXECUTABLE];

    char **  argv = NULL;
    if (param.find(ARGUMENTS) != param.end()) {
      Versatile V = param[ARGUMENTS];

      argv = new char * [V.size() + 2]; // 1 pour name et 1 pour le NULL terminal

      argv[0] = new char [name.size() + 1];
      strncpy(argv[0], name.c_str(), name.size() + 1);

      debug_command  += string(" # ") + argv[0];

      int i = 1;
      for(Versatile::const_iterator it=V.begin(); it!=V.end(); it++, i++) {
	StringType argt = * static_cast<StringType *>(*it);
	string     arg  = argt;
	argv[i]         = new char [arg.size() + 1];
	strncpy(argv[i], arg.c_str(), arg.size() + 1);
	debug_command  += string(" # ") + argv[i];
      }

      // assert (i == V.size() + 1)
      argv[i] = NULL;
    }


    UNDER_LOCK( cout << "*** debug_command = " << debug_command << endl );



    Environnement env = _job.getEnvironnement();


    char ** envp = NULL;
    if(env.size() > 0) {
      envp = new char * [env.size() + 1]; // 1 pour le NULL terminal
      int i = 0;
      for(Environnement::const_iterator it=env.begin(); it!=env.end(); it++, i++) {
	const string  & key   = (*it).first;
	const string  & value = (*it).second;
	ostringstream oss;
	oss << key << "=" << value;
	envp[i]         = new char [oss.str().size() + 1];
	strncpy(envp[i], oss.str().c_str(), oss.str().size() + 1);
      }

      // assert (i == env.size())
      envp[i] = NULL;
    }




    // On positionne les limites systeme imposees au fils
    if (param.find(MAXCPUTIME) != param.end()) {
      int maxcputime = param[MAXCPUTIME];
      struct rlimit limit;
      limit.rlim_cur = maxcputime;
      limit.rlim_max = int(maxcputime * 1.1);
      setrlimit(RLIMIT_CPU, &limit);
    }

    if (param.find(MAXDISKSIZE) != param.end()) {
      int maxdisksize = param[MAXDISKSIZE];
      struct rlimit limit;
      limit.rlim_cur = maxdisksize * 1024;
      limit.rlim_max = int(maxdisksize * 1.1) * 1024;
      setrlimit(RLIMIT_FSIZE, &limit);
    }

    if (param.find(MAXRAMSIZE) != param.end()) {
      int maxramsize = param[MAXRAMSIZE];
      struct rlimit limit;
      limit.rlim_cur = maxramsize * 1024;
      limit.rlim_max = int(maxramsize * 1.1) * 1024;
      setrlimit(RLIMIT_AS, &limit);
    }



    // On cree une session pour le fils de facon a ce qu'il ne soit pas
    // detruit lorsque le shell se termine (le shell ouvre une session et
    // tue tous les process appartenant a la session en quittant)
    setsid();


    // On ferme les descripteurs de fichiers standards
    //close(STDIN_FILENO);
    //close(STDOUT_FILENO);
    //close(STDERR_FILENO);


    // On execute la commande du fils
    execve(execpath, argv, envp);

    // No need to deallocate since nothing happens after a successful exec

    // Normalement on ne devrait jamais arriver ici    
    ofstream file_err("error.log");
    UNDER_LOCK( file_err << "Echec de l'appel a execve" << endl );
    
    } catch (GenericException & e) {
      
      std::cerr << "Caught exception : " << e.type << " : " << e.message << std::endl;
    }

    exit(99);

  }




  void BatchManager_Local::kill_child_on_exit(void * p_pid)
  {
    pid_t child = * static_cast<pid_t *>(p_pid);

    // On tue le fils
    kill(child, SIGTERM);

    // Nota : on pourrait aussi faire a la suite un kill(child, SIGKILL)
    // mais cette option n'est pas implementee pour le moment, car il est 
    // preferable de laisser le process fils se terminer normalement et seul.

  }

  void BatchManager_Local::delete_on_exit(void * arg)
  {
    ThreadAdapter * p_ta = static_cast<ThreadAdapter *>(arg);
    delete p_ta;
  }

}
