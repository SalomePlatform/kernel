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
 * BatchManager_Local_RSH.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Thu Nov  6 10:17:22 2003
 * Projet : Salome 2
 *
 */

#ifndef _BATCHMANAGER_LOCAL_RSH_H_
#define _BATCHMANAGER_LOCAL_RSH_H_

#include "Batch_Defines.hxx"

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

  class BATCH_EXPORT BatchManager_Local_RSH : public BatchManager_Local
  {
  public:
    // Constructeur et destructeur
    BatchManager_Local_RSH(const FactBatchManager * parent, const char * host="localhost") throw(InvalidArgumentException,ConnexionFailureException); // connexion a la machine host
    virtual ~BatchManager_Local_RSH();

  protected:
    // Methode abstraite qui renvoie la commande de copie du fichier source en destination
    virtual std::string copy_command( const std::string & host_source,
				      const std::string & source,
				      const std::string & host_destination,
				      const std::string & destination) const;

    // Methode abstraite qui renvoie la commande a executer
    virtual std::string exec_command(Parametre & param) const;

    // Methode qui renvoie la commande d'effacement du fichier
    virtual std::string remove_command( const std::string & host_destination,
					const std::string & destination) const;

  };

}

#endif
