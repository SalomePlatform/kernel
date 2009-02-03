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
 * Job_LSF.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Fri Nov 14 11:00:39 2003
 * Projet : Salome 2
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "Batch_Job_LSF.hxx"

using namespace std;

namespace Batch {


  // Constructeur
  Job_LSF::Job_LSF(const Job & job) : _p_submit(0)
  {
    Parametre P = job.getParametre();
    _p_submit = ParametreToSubmitStruct(P);
  }


  // Destructeur
  Job_LSF::~Job_LSF()
  {
    if (_p_submit) {
      if (_p_submit->jobName)     delete [] _p_submit->jobName;
      if (_p_submit->queue)       delete [] _p_submit->queue;
      if (_p_submit->askedHosts) {
	delete [] *(_p_submit->askedHosts);
	delete [] _p_submit->askedHosts;
      }
      if (_p_submit->resReq)      delete [] _p_submit->resReq;
      if (_p_submit->hostSpec)    delete [] _p_submit->hostSpec;
      if (_p_submit->dependCond)  delete [] _p_submit->dependCond;
      if (_p_submit->timeEvent)   delete [] _p_submit->timeEvent;
      if (_p_submit->inFile)      delete [] _p_submit->inFile;
      if (_p_submit->outFile)     delete [] _p_submit->outFile;
      if (_p_submit->errFile)     delete [] _p_submit->errFile;
      if (_p_submit->command)     delete [] _p_submit->command;
      if (_p_submit->newCommand)  delete [] _p_submit->newCommand;
      if (_p_submit->chkpntDir)   delete [] _p_submit->chkpntDir;
      if (_p_submit->xf)          delete [] _p_submit->xf;
      if (_p_submit->preExecCmd)  delete [] _p_submit->preExecCmd;
      if (_p_submit->mailUser)    delete [] _p_submit->mailUser;
      if (_p_submit->projectName) delete [] _p_submit->projectName;
      if (_p_submit->loginShell)  delete [] _p_submit->loginShell;
      if (_p_submit->exceptList)  delete [] _p_submit->exceptList;
      delete _p_submit;
    }
  }


  // Accesseur
  struct submit * Job_LSF::getSubmitStruct()
  {
    return _p_submit;
  }


  char * Job_LSF::string2char(const string & s)
  {
    char * ch = new char [s.size() + 1];
    memset(ch, 0, s.size() + 1);
    strncat(ch, s.c_str(), s.size());
    return ch;
  }


  struct submit * Job_LSF::ParametreToSubmitStruct(const Parametre & P)
  {
    if (! _p_submit) _p_submit = new struct submit;

    memset( (void *) _p_submit, 0, sizeof(struct submit));

    struct submit & sub = * _p_submit;
    sub.options  = 0;
    sub.options2 = 0;

    sub.beginTime = 0; // job can run as soon as possible (default)
    sub.termTime  = 0; // job can run as long as it wishes (default)

    sub.numProcessors    = 1; // job can run on one single processor (default)
    sub.maxNumProcessors = 1; // job can run on one single processor (default)

    for(int i = 0; i< LSF_RLIM_NLIMITS; i++) sub.rLimits[i] = DEFAULT_RLIMIT;

    typedef std::vector< struct xFile > XFTAB;
    XFTAB xf_tab;

    string st_second;
    for(Parametre::const_iterator it = P.begin(); it != P.end(); it++) {
      if ( (*it).first == ACCOUNT ) {
	sub.options |= SUB_PROJECT_NAME;
	st_second = (*it).second.str();
	sub.projectName = string2char(st_second);

      } else if ( (*it).first == CHECKPOINT ) {
	if (static_cast< long >((*it).second))
	  sub.options |= SUB_CHKPNT_PERIOD;
	else
	  sub.options &= ~ SUB_CHKPNT_PERIOD;

      } else if ( (*it).first == CKPTINTERVAL ) {
	sub.chkpntPeriod = static_cast< long >((*it).second);

      } else if ( (*it).first == EXECUTABLE ) {
	st_second = (*it).second.str();
	sub.command = string2char(st_second);

      } else if ( (*it).first == EXECUTIONHOST ) {
	sub.options |= SUB_HOST;
	if (! sub.numAskedHosts) {
	  sub.numAskedHosts = 1;
	  sub.askedHosts = new char* [1];
	}
	st_second = (*it).second.str();
	sub.askedHosts[0] = string2char(st_second);

      } else if ( (*it).first == HOLD ) {
	if (static_cast< long >((*it).second))
	  sub.options2 |= SUB2_HOLD;
	else
	  sub.options2 &= ~ SUB2_HOLD;

      } else if ( (*it).first == INFILE ) {
	Versatile V = (*it).second;
	Versatile::iterator Vit;

	for(Vit=V.begin(); Vit!=V.end(); Vit++) {
	  CoupleType cpt  = *static_cast< CoupleType * >(*Vit);
	  Couple cp       = cpt;
	  string local    = cp.getLocal();
	  string remote   = cp.getRemote();
					
	  // ATTENTION : les notions de fichier "local" ou "remote" sont inverses de celle de PBS qui a un point de vue serveur et non pas utilisateur
	  if (remote == "stdin"){
	    sub.options |= SUB_IN_FILE;
	    sub.inFile = string2char(local);

	  } else {
	    struct xFile xf;
	    strncpy(xf.subFn,  local.c_str(),  MAXFILENAMELEN - 1); xf.subFn[MAXFILENAMELEN - 1]  = 0;
	    strncpy(xf.execFn, remote.c_str(), MAXFILENAMELEN - 1); xf.execFn[MAXFILENAMELEN - 1] = 0;
	    xf.options = XF_OP_SUB2EXEC;
	    xf_tab.push_back(xf);
	  }
	}

      } else if ( (*it).first == MAIL ) {
	sub.options |= SUB_MAIL_USER;
	st_second = (*it).second.str();
	sub.mailUser = string2char(st_second);

      } else if ( (*it).first == MAXCPUTIME ) {
	sub.rLimits[LSF_RLIMIT_CPU] = static_cast< long >((*it).second);

      } else if ( (*it).first == MAXDISKSIZE ) {
	sub.rLimits[LSF_RLIMIT_FSIZE] = static_cast< long >((*it).second);

      } else if ( (*it).first == MAXRAMSIZE ) {
	sub.rLimits[LSF_RLIMIT_SWAP] = static_cast< long >((*it).second);

      } else if ( (*it).first == MAXWALLTIME ) {
	sub.rLimits[LSF_RLIMIT_RUN] = static_cast< long >((*it).second);

      } else if ( (*it).first == NAME ) {
	sub.options |= SUB_JOB_NAME;
	st_second = (*it).second.str();
	sub.jobName = string2char(st_second);

      } else if ( (*it).first == NBPROC ) {
	sub.numProcessors    = static_cast< long >((*it).second);
	sub.maxNumProcessors = static_cast< long >((*it).second);

      } else if ( (*it).first == OUTFILE ) {
	Versatile V = (*it).second;
	Versatile::iterator Vit;

	for(Vit=V.begin(); Vit!=V.end(); Vit++) {
	  CoupleType cpt  = *static_cast< CoupleType * >(*Vit);
	  Couple cp       = cpt;
	  string local    = cp.getLocal();
	  string remote   = cp.getRemote();
					
	  // ATTENTION : les notions de fichier "local" ou "remote" sont inverses de celle de PBS qui a un point de vue serveur et non pas utilisateur
	  if (remote == "stdout"){
	    sub.options |= SUB_OUT_FILE;
	    sub.outFile = string2char(local);

	  } else if (remote == "stderr"){
	    sub.options |= SUB_ERR_FILE;
	    sub.errFile = string2char(local);

	  } else {
	    struct xFile xf;
	    strncpy(xf.subFn,  local.c_str(),  MAXFILENAMELEN - 1); xf.subFn[MAXFILENAMELEN - 1]  = 0;
	    strncpy(xf.execFn, remote.c_str(), MAXFILENAMELEN - 1); xf.execFn[MAXFILENAMELEN - 1] = 0;
	    xf.options = XF_OP_EXEC2SUB;
	    xf_tab.push_back(xf);
	  }
	}


      } else if ( (*it).first == QUEUE ) {
	sub.options |= SUB_QUEUE;
	st_second = (*it).second.str();
	sub.queue = string2char(st_second);

      } else if ( (*it).first == STARTTIME ) {
	sub.beginTime = static_cast< long >((*it).second);

      } else if ( (*it).first == TMPDIR ) {
	// TODO

      } else if ( (*it).first == USER ) {
	// TODO

      }
    }


    // Transfert de fichiers en entree et sortie
    sub.options |= SUB_OTHER_FILES;
    sub.nxf = xf_tab.size();
    sub.xf = new struct xFile [sub.nxf];
    int ixf = 0;
    for(XFTAB::const_iterator it_xf=xf_tab.begin(); it_xf != xf_tab.end(); it_xf++, ixf++)
      sub.xf[ixf] = *it_xf; // *it_xf == xf_tab[ixf]
	

    return _p_submit;
  }

}
