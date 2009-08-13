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
 * BatchManager_Local_SH.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Thu Nov  6 10:17:22 2003
 * Projet : Salome 2
 *
 */

#ifdef HAVE_SALOME_CONFIG
#  include <SALOMEconfig.h>
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <sys/types.h>
#ifndef WIN32
#include <sys/wait.h>
#include <unistd.h>
#endif
#include <ctime>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include "Batch_IOMutex.hxx"
#include "Batch_BatchManager_Local_SH.hxx"

#ifndef RM
#error "RM undefined. You must set RM to a valid path to a rm-like command."
#endif

#ifndef CP
#error "CP undefined. You must set CP to a valid path to a cp-like command."
#endif

#ifndef SH
#error "SH undefined. You must set SH to a valid path to a sh-like command."
#endif

using namespace std;

namespace Batch {


  // Constructeur
  BatchManager_Local_SH::BatchManager_Local_SH(const FactBatchManager * parent, const char * host) throw(InvalidArgumentException,ConnexionFailureException) : BatchManager_Local(parent, host)
  {
  }

  // Destructeur
  BatchManager_Local_SH::~BatchManager_Local_SH()
  {
  }


  // Methode qui renvoie la commande de copie du fichier source en destination
  string BatchManager_Local_SH::copy_command(const string & host_source, const string & source, const string & host_destination, const string & destination) const
  {
    ostringstream copy_cmd;
    copy_cmd << CP << " " << source << " " << destination;
    return copy_cmd.str();
  }
  
  // Methode qui renvoie la commande a executer
  string BatchManager_Local_SH::exec_command(Parametre & param) const
  {
    ostringstream exec_sub_cmd;
    exec_sub_cmd << param[EXECUTABLE];

    if (param.find(ARGUMENTS) != param.end()) {
      Versatile V = param[ARGUMENTS];
      for(Versatile::const_iterator it=V.begin(); it!=V.end(); it++) {
	StringType argt = * static_cast<StringType *>(*it);
	string     arg  = argt;
	exec_sub_cmd << " " << arg;
      }
    }

    param[ARGUMENTS]  = "-c";
    param[ARGUMENTS] += exec_sub_cmd.str();

    return SH;
  }

  // Methode qui renvoie la commande d'effacement du fichier
  string BatchManager_Local_SH::remove_command(const string & host_destination, const string & destination) const
  {
    ostringstream remove_cmd;
    remove_cmd << RM << " " << destination;
    return remove_cmd.str();
  }
  
}
