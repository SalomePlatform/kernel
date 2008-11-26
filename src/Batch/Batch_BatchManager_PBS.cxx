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
 * BatchManager_PBS.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Thu Nov  6 10:17:22 2003
 * Projet : Salome 2
 *
 */

extern "C" {
#include <pbs_error.h>
#include <pbs_ifl.h>
}
#include <iostream>
#include <fstream>
#include <sstream>
//#include "MEDMEM_STRING.hxx"
#include "Batch_BatchManager_PBS.hxx"
using namespace std;

namespace Batch {

  // Recupere le nom du serveur par defaut
//   string BatchManager_PBS::getDefaultServer() {
//     string server_name = "localhost";

//     const char * server_name_path = "@openpbsspooldir@/server_name";
//     ifstream server_name_file(server_name_path);
//     if (server_name_file) {
//       server_name_file >> server_name;
//       server_name_file.close();
//     }

//     return server_name;
//   }

  // Constructeur
//   BatchManager_PBS::BatchManager_PBS() throw(InvalidArgumentException,ConnexionFailureException) : BatchManager(BatchManager_PBS::getDefaultServer())
//   {
//     // On se connecte au serveur PBS
//     _connect = pbs_connect(const_cast< char * >(_hostname.c_str()));
//     if (_connect < 0) { // si erreur
//       char * errmsg = pbs_geterrmsg(_connect);
//       string msg = "PBS Server on host \"";
//       msg += _hostname;
//       msg += "\" : ";
//       msg += errmsg ? errmsg : "Reason unknown";
//       throw ConnexionFailureException(msg.c_str());
//     }
//   }

  // Constructeur
//   BatchManager_PBS::BatchManager_PBS(string host) throw(InvalidArgumentException,ConnexionFailureException) : BatchManager(host)
//   {
//     // On se connecte au serveur PBS
//     _connect = pbs_connect(const_cast< char * >(_hostname.c_str()));
//     if (_connect < 0) { // si erreur
//       char * errmsg = pbs_geterrmsg(_connect);
//       string msg = "PBS Server on host \"";
//       msg += _hostname;
//       msg += "\" : ";
//       msg += errmsg ? errmsg : "Reason unknown";
//       throw ConnexionFailureException(msg.c_str());
//     }
//   }
  BatchManager_PBS::BatchManager_PBS(const FactBatchManager * parent, const char * host) throw(InvalidArgumentException,ConnexionFailureException) : BatchManager(parent, host)
  {
    // On se connecte au serveur PBS
    _connect = pbs_connect(const_cast< char * >(_hostname.c_str()));
    if (_connect < 0) { // si erreur
      char * errmsg = pbs_geterrmsg(_connect);
      string msg = "PBS Server on host \"";
      msg += _hostname;
      msg += "\" : ";
      msg += errmsg ? errmsg : "Reason unknown";
      throw ConnexionFailureException(msg.c_str());
    }
  }

  // Destructeur
  BatchManager_PBS::~BatchManager_PBS()
  {
    // On se deconnecte du serveur PBS
    int rc = pbs_disconnect(_connect);
    if (rc < 0) { // si erreur
      string msg = "PBS Server on host \"";
      msg += _hostname;
      msg += "\" : ";
      msg += pbs_geterrmsg(_connect);
      throw ConnexionFailureException(msg.c_str());
    }
  }

  // Methode pour le controle des jobs : soumet un job au gestionnaire
  const JobId BatchManager_PBS::submitJob(const Job & job)
  {
    Job_PBS jobpbs = job;
    char * ref = pbs_submit(_connect,
			    jobpbs.getAttributesOP(),
			    jobpbs.getScript(),
			    jobpbs.getDestination(),
			    NULL);
    if (!ref) { // si erreur
      char * msg = pbs_geterrmsg(_connect);
      if (!msg) msg = "unknown";
      throw APIInternalFailureException(string("PBS submit error. Reason : ") + msg);
    }

    JobId id(this, string(ref));
    free(ref);
    return id;
  }

  // Methode pour le controle des jobs : retire un job du gestionnaire
  void BatchManager_PBS::deleteJob(const JobId & jobid)
  {
    char * ref = const_cast< char * >(jobid.getReference().c_str());
    int rc = pbs_deljob(_connect, ref, 0);
    if (rc) { // si erreur
      char * msg = pbs_geterrmsg(_connect);
      if (!msg) msg = "unknown";
      throw APIInternalFailureException(string("PBS deljob error. Reason : ") + msg);
    }
  }
   
  // Methode pour le controle des jobs : suspend un job en file d'attente
  void BatchManager_PBS::holdJob(const JobId & jobid)
  {
    char * ref = const_cast< char * >(jobid.getReference().c_str());
    int rc = pbs_holdjob(_connect, ref, USER_HOLD, 0);
    if (rc) { // si erreur
      char * msg = pbs_geterrmsg(_connect);
      if (!msg) msg = "unknown";
      throw APIInternalFailureException(string("PBS holdjob error. Reason : ") + msg);
    }
  }

  // Methode pour le controle des jobs : relache un job suspendu
  void BatchManager_PBS::releaseJob(const JobId & jobid)
  {
    char * ref = const_cast< char * >(jobid.getReference().c_str());
    int rc = pbs_rlsjob(_connect, ref, USER_HOLD, 0);
    if (rc) { // si erreur
      char * msg = pbs_geterrmsg(_connect);
      if (!msg) msg = "unknown";
      throw APIInternalFailureException(string("PBS rlsjob error. Reason : ") + msg);
    }
  }


  // Methode pour le controle des jobs : modifie un job en file d'attente
  void BatchManager_PBS::alterJob(const JobId & jobid, const Parametre & param, const Environnement & env)
  {
    Job job(param, env);
    Job_PBS jobpbs(job);

    char * ref = const_cast< char * >(jobid.getReference().c_str());
    int rc = pbs_alterjob(_connect,
			  ref,
			  jobpbs.getAttributes(),
			  NULL);
    if (rc) { // si erreur
      char * msg = pbs_geterrmsg(_connect);
      if (!msg) msg = "unknown";
      throw APIInternalFailureException(string("PBS alterjob error. Reason : ") + msg);
    }
		
  }

  // Methode pour le controle des jobs : modifie un job en file d'attente
  void BatchManager_PBS::alterJob(const JobId & jobid, const Parametre & param)
  {
    alterJob(jobid, param, Environnement());
  }

  // Methode pour le controle des jobs : modifie un job en file d'attente
  void BatchManager_PBS::alterJob(const JobId & jobid, const Environnement & env)
  {
    alterJob(jobid, Parametre(), env);
  }



  // Methode pour le controle des jobs : renvoie l'etat du job
  JobInfo BatchManager_PBS::queryJob(const JobId & jobid)
  {
    char * id = const_cast< char * >(jobid.getReference().c_str());
    JobInfo_PBS ji = JobInfo_PBS(pbs_statjob(_connect, id, 0, 0), true);
    return ji;
  }



}
