/*
 * BatchManager_Local_SH.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Thu Nov  6 10:17:22 2003
 * Projet : Salome 2
 *
 */

#ifndef _BATCHMANAGER_LOCAL_SH_H_
#define _BATCHMANAGER_LOCAL_SH_H_


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
#include "Batch_BatchManager_Local.hxx"

namespace Batch {

  class FactBatchManager;

  class BatchManager_Local_SH : public BatchManager_Local
  {
  public:
    // Constructeur et destructeur
    BatchManager_Local_SH(const FactBatchManager * parent, const char * host="localhost") throw(InvalidArgumentException,ConnexionFailureException); // connexion a la machine host
    virtual ~BatchManager_Local_SH();

  protected:
    // Methode qui renvoie la commande de copie du fichier source en destination
    virtual string copy_command(const string & host_source, const string & source, const string & host_destination, const string & destination) const;

    // Methode qui renvoie la commande a executer
    virtual string exec_command(Parametre & param) const;

    // Methode qui renvoie la commande d'effacement du fichier
    virtual string remove_command(const string & host_destination, const string & destination) const;

  };

}

#endif
