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
 * JobInfo_PBS.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Fri Nov 21 09:42:06 2003
 * Projet : Salome 2
 *
 */

#include <cstdio>
#include <sstream>
//#include "MEDMEM_STRING.hxx"
#include "Batch_Parametre.hxx"
#include "Batch_Environnement.hxx"
#include "Batch_RunTimeException.hxx"
#include "Batch_JobInfo_PBS.hxx"
using namespace std;

namespace Batch {

  // Constructeurs
  JobInfo_PBS::JobInfo_PBS(struct batch_status * list, bool tobedeleted) : JobInfo()
  {
    // On ne considere que le premier element de la liste
    // Si tout est OK, la liste ne devrait contenir qu'un element
    // Sinon on leve une exception.
    struct batch_status * p_job = list;
    int i;
    for(i=0; p_job; p_job = p_job->next) i++;
    if (i == 0) throw RunTimeException("Liste vide (le job est absent de la file)");
    if (i > 1) {
      //MEDMEM::STRING sst;
      ostringstream sst;
      sst << "JobInfo_PBS::JobInfo_PBS(struct batch_status * list, bool tobedeleted) : la liste contient "
	  << i << " elements" << " (1 seul requis)" << endl;
      throw RunTimeException(sst.str());
    }
    p_job = list;

    // On remplit les membres _param et _env

    if (p_job->name && strlen(p_job->name)) _param[ID]   = p_job->name;
    if (p_job->text && strlen(p_job->text)) _param[TEXT] = p_job->text;

    for(struct attrl * p_attr = p_job->attribs; p_attr; p_attr = p_attr->next) {

      string name, res, value;
      if (p_attr->name && strlen(p_attr->name)) name = p_attr->name;
      if (p_attr->resource && strlen(p_attr->resource)) res = p_attr->resource;
      if (p_attr->value && strlen(p_attr->value)) value = p_attr->value;

      if (name == ATTR_N) {
	_param[NAME] = value;
      
      } else if (name == ATTR_owner) {
	_param[USER] = value;

      } else if (name == ATTR_state) {
	_param[STATE] = value;

      } else if (name == ATTR_queue) {
	_param[QUEUE] = value;

      } else if (name == ATTR_A) {
	_param[ACCOUNT] = value;

      } else if (name == ATTR_M) {
	_param[MAIL] = value;

      } else if (name == ATTR_c) {
	if (!strcmp(value.c_str(), CHECKPOINT_UNSPECIFIED)) _param[CHECKPOINT] = 1L;
	else _param[CHECKPOINT] = 0L;

      } else if (name == ATTR_h) {
	if (!strcmp(value.c_str(), NO_HOLD)) _param[HOLD] = 0L;
	else _param[HOLD] = 1L;

      } else if (name == ATTR_ctime) {
	_param[CREATIONTIME] = atol(value.c_str());

      } else if (name == ATTR_etime) {
	_param[ELIGIBLETIME] = atol(value.c_str());

      } else if (name == ATTR_mtime) {
	_param[MODIFICATIONTIME] = atol(value.c_str());

      } else if (name == ATTR_qtime) {
	_param[QUEUEDTIME] = atol(value.c_str());

      } else if (name == ATTR_exechost) {
	_param[EXECUTIONHOST] = value;

      } else if (name == ATTR_session) {
	_param[PID] = atol(value.c_str());

      } else if (name == ATTR_euser) {
	_param[EUSER] = value;

      } else if (name == ATTR_egroup) {
	_param[EGROUP] = value;

      } else if (name == ATTR_l) {
	if (res == "cput") {
	  _param[MAXCPUTIME] = HMStoLong(value);

	} else if (res == "walltime") {
	  _param[MAXWALLTIME] = HMStoLong(value);

	}

      } else if (name == ATTR_used) {
	if (res == "cput") {
	  _param[USEDCPUTIME] = HMStoLong(value);

	} else if (res == "walltime") {
	  _param[USEDWALLTIME] = HMStoLong(value);

	}

      } else if (name == ATTR_v) {
	int deb = 0;
	int pos = 0;
	bool ok = true;

	while (ok) {
	  pos = value.find(",", deb);
	  string sub = value.substr(deb, pos-deb);
	  deb = pos + 1;
	  if (pos < 0) ok = false;

	  int eq = sub.find("=");
	  _env[sub.substr(0, eq)] = sub.substr(eq+1);
	}

      }
    }


    if (tobedeleted) pbs_statfree(list);
  }

  // Destructeur
  JobInfo_PBS::~JobInfo_PBS()
  {
    // Nothing to do
  }
  
  // Convertit une date HH:MM:SS en secondes
  long JobInfo_PBS::HMStoLong(const string & s)
  {
    long hour, min, sec;

    sscanf( s.c_str(), "%ld:%ld:%ld", &hour, &min, &sec);
    return ( ( ( hour * 60L ) + min ) * 60L ) + sec;
  }

  // Methode pour l'interfacage avec Python (SWIG) : affichage en Python
  string JobInfo_PBS::__str__() const
  {
    //MEDMEM::STRING sst; 
    ostringstream sst;
    sst << "<JobInfo_PBS (" << this << ") :" << endl;
    sst << " ID = " <<_param[ID] << endl;

    sst << "  + Parametre :" << endl;
    Parametre::const_iterator itp;
    for(itp=_param.begin(); itp!=_param.end(); itp++) {
      if ( (*itp).first != ID ) {
	sst << "    * " << (*itp).first << " = " << (*itp).second << endl;
      }
    }
    return sst.str();
  }


}
