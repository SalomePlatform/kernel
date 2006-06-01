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
