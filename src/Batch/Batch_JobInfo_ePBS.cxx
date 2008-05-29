// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
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
 * JobInfo_ePBS.cxx :  emulation of PBS client
 *
 * Auteur : Bernard SECHER - CEA DEN
 * Mail   : mailto:bernard.secher@cea.fr
 * Date   : Thu Apr 24 10:17:22 2008
 * Projet : PAL Salome 
 *
 */

#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Batch_Parametre.hxx"
#include "Batch_Environnement.hxx"
#include "Batch_RunTimeException.hxx"
#include "Batch_APIInternalFailureException.hxx"
#include "Batch_JobInfo_ePBS.hxx"

namespace Batch {



  // Constructeurs
  JobInfo_ePBS::JobInfo_ePBS(int id, string logFile) : JobInfo()
  {
    // On remplit les membres _param et _env
    ostringstream oss;
    oss << id;
    _param[ID] = oss.str();

    // read of log file
    char line[128];
    ifstream fp(logFile.c_str(),ios::in);
      
    string status;
    string sline;
    int pos = string::npos;
    while( (pos == string::npos) && fp.getline(line,80,'\n') ){
      sline = string(line);
      pos = sline.find("job_state");
    };
      
    if(pos!=string::npos){
      istringstream iss(sline);
      iss >> status;
      iss >> status;
      iss >> status;
    }
    else
      status = "U";

    _param[STATE] = status;

    if( status.find("R") != string::npos)
      _running = true;

  }

  // Teste si un job est present en machine
  bool JobInfo_ePBS::isRunning() const
  {
    return _running;
  }


  // Destructeur
  JobInfo_ePBS::~JobInfo_ePBS()
  {
    // Nothing to do
  }

  // Convertit une date HH:MM:SS en secondes
  long JobInfo_ePBS::HMStoLong(const string & s)
  {
    long hour, min, sec;

    sscanf( s.c_str(), "%ld:%ld:%ld", &hour, &min, &sec);
    return ( ( ( hour * 60L ) + min ) * 60L ) + sec;
  }

  // Methode pour l'interfacage avec Python (SWIG) : affichage en Python
  string JobInfo_ePBS::__str__() const
  {
    ostringstream sst;
    sst << "<JobInfo_ePBS (" << this << ") :" << endl;
    sst << " ID = " <<_param[ID] << endl;
    sst << " STATE = " <<_param[STATE] << endl;

    return sst.str();
  }


}
