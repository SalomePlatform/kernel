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
 * JobInfo_LSF.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Fri Nov 21 09:42:06 2003
 * Projet : Salome 2
 *
 */

#include <cstdio>
#include <sstream>
#include <string>
#include "Batch_Parametre.hxx"
#include "Batch_Environnement.hxx"
#include "Batch_RunTimeException.hxx"
#include "Batch_APIInternalFailureException.hxx"
#include "Batch_JobInfo_LSF.hxx"

using namespace std;

namespace Batch {



  // Constructeurs
  JobInfo_LSF::JobInfo_LSF(int id) : JobInfo()
  {
    struct loadIndexLog * p_ld        = new struct loadIndexLog;
    struct jobInfoHead  * p_jInfoHead = lsb_openjobinfo_a(id, NULL, NULL, NULL, NULL, ALL_JOB);

    int more = p_jInfoHead->numJobs;
    if (more != 1) {
      char * msg = lsb_sysmsg();
      if (!msg) msg = "unknown";
      throw APIInternalFailureException(string("LSF lsb_openjobinfo error. Reason : ") + msg);     
    }

    // on remplit une structure contenant <more> elements
    struct jobInfoEnt & jobInfo = * lsb_readjobinfo(&more);


    // On remplit les membres _param et _env
    _param[ACCOUNT]          = jobInfo.submit.projectName;
    _param[CHECKPOINT]       = jobInfo.submit.chkpntPeriod != 0;
    _param[CKPTINTERVAL]     = jobInfo.submit.chkpntPeriod;
    _param[CREATIONTIME]     = jobInfo.submitTime;
    // _param[EGROUP]           = jobInfo.;
    _param[ELIGIBLETIME]     = jobInfo.reserveTime;
    _param[ENDTIME]          = jobInfo.endTime;
    _param[EUSER]            = jobInfo.execUsername;
    _param[EXECUTABLE]       = jobInfo.submit.command;
    _param[EXITCODE]         = jobInfo.exitStatus;
    _param[HOLD]             = jobInfo.status & (JOB_STAT_PSUSP | JOB_STAT_SSUSP | JOB_STAT_USUSP);
    _param[MAIL]             = jobInfo.submit.mailUser;
    _param[MAXCPUTIME]       = jobInfo.submit.rLimits[LSF_RLIMIT_CPU];
    _param[MAXDISKSIZE]      = jobInfo.submit.rLimits[LSF_RLIMIT_FSIZE];
    _param[MAXRAMSIZE]       = jobInfo.submit.rLimits[LSF_RLIMIT_SWAP];
    _param[MAXWALLTIME]      = jobInfo.submit.rLimits[LSF_RLIMIT_RUN];
    _param[MODIFICATIONTIME] = jobInfo.lastEvent;
    _param[NAME]             = jobInfo.jName;
    _param[NBPROC]           = jobInfo.submit.numProcessors;
    _param[PID]              = jobInfo.jobPid;
    _param[QUEUE]            = jobInfo.submit.queue;
    _param[QUEUEDTIME]       = jobInfo.submitTime;
    // _param[SERVER]           = jobInfo.;
    _param[STARTTIME]        = jobInfo.startTime;
    _param[TEXT]             = jobInfo.numReasons ? lsb_pendreason(jobInfo.numReasons,
								   jobInfo.reasonTb, 
								   p_jInfoHead,
								   p_ld,0) : "";
    // _param[TMPDIR]           = jobInfo.;
    _param[USEDCPUTIME]      = static_cast<long>(jobInfo.cpuTime);
    // _param[USEDDISKSIZE]     = jobInfo.;
    _param[USEDRAMSIZE]      = jobInfo.runRusage.mem;
    _param[USEDWALLTIME]     = jobInfo.duration * 60L;
    _param[USER]             = jobInfo.user;


    ostringstream oss;
    int jobid = jobInfo.jobId;
    oss << jobid;
    _param[ID] = oss.str();


    string hosts, sep;
    for(int i=0; i < jobInfo.numExHosts; i++, sep="+") {
      hosts += jobInfo.exHosts[i];
      hosts += sep;
    }
    _param[EXECUTIONHOST]    = hosts;

    ostringstream status;

    if (IS_PEND(jobInfo.status))
      status << " Job is pending;";
    if (IS_START(jobInfo.status))
      status << " Job is started;";
    if (IS_FINISH(jobInfo.status))
      status << " Job is finished;";
    if (IS_SUSP(jobInfo.status))
      status << " Job is suspended;";
    if (IS_POST_DONE(jobInfo.status))
      status << " Job is post-done;";
    if (IS_POST_ERR(jobInfo.status))
      status << " Job is post-error;";

    _param[STATE] = status.str();
    _running = IS_FINISH(jobInfo.status) ? false : true;


    if (strlen(jobInfo.submit.inFile))
      _param[INFILE]  += Couple(jobInfo.submit.inFile, "stdin");
    if (strlen(jobInfo.submit.outFile))
      _param[OUTFILE]  += Couple(jobInfo.submit.outFile, "stdout");
    if (strlen(jobInfo.submit.errFile))
      _param[OUTFILE]  += Couple(jobInfo.submit.errFile, "stderr");

    for(int i=0; i < jobInfo.submit.nxf; i++) {
      switch (jobInfo.submit.xf[i].options) {
      case XF_OP_SUB2EXEC:
	_param[INFILE]  += Couple(jobInfo.submit.xf[i].subFn, jobInfo.submit.xf[i].execFn);
	break;

      case XF_OP_EXEC2SUB:
	_param[OUTFILE] += Couple(jobInfo.submit.xf[i].subFn, jobInfo.submit.xf[i].execFn);
	break;

      default:
	break;
      }
    }


    lsb_closejobinfo();
    delete p_ld;
  }



    // Teste si un job est present en machine
  bool JobInfo_LSF::isRunning() const
  {
    return _running;
  }


  // Destructeur
  JobInfo_LSF::~JobInfo_LSF()
  {
    // Nothing to do
  }


  
  // Convertit une date HH:MM:SS en secondes
  long JobInfo_LSF::HMStoLong(const string & s)
  {
    long hour, min, sec;

    sscanf( s.c_str(), "%ld:%ld:%ld", &hour, &min, &sec);
    return ( ( ( hour * 60L ) + min ) * 60L ) + sec;
  }

  // Methode pour l'interfacage avec Python (SWIG) : affichage en Python
  string JobInfo_LSF::__str__() const
  {
    ostringstream sst;
    sst << "<JobInfo_LSF (" << this << ") :" << endl;
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
