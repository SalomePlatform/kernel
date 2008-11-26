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
 * JobInfo_eSGE.cxx :  emulation of SGE client
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
#include "Batch_JobInfo_eSGE.hxx"

using namespace std;

namespace Batch {



  // Constructeurs
  JobInfo_eSGE::JobInfo_eSGE(int id, string logFile) : JobInfo()
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
    fp.getline(line,80,'\n');
    sline = string(line);

    if( sline.length() > 0 ){
      istringstream iss(sline);
      iss >> status >> status >> status >> status >> status;
    }
    else
      status = "e";

    _param[STATE] = status;

    if( status.find("r") != string::npos)
      _running = true;

  }

  // Teste si un job est present en machine
  bool JobInfo_eSGE::isRunning() const
  {
    return _running;
  }


  // Destructeur
  JobInfo_eSGE::~JobInfo_eSGE()
  {
    // Nothing to do
  }

  // Convertit une date HH:MM:SS en secondes
  long JobInfo_eSGE::HMStoLong(const string & s)
  {
    long hour, min, sec;

    sscanf( s.c_str(), "%ld:%ld:%ld", &hour, &min, &sec);
    return ( ( ( hour * 60L ) + min ) * 60L ) + sec;
  }

  // Methode pour l'interfacage avec Python (SWIG) : affichage en Python
  string JobInfo_eSGE::__str__() const
  {
    ostringstream sst;
    sst << "<JobInfo_eSGE (" << this << ") :" << endl;
    sst << " ID = " <<_param[ID] << endl;
    sst << " STATE = " <<_param[STATE] << endl;

    return sst.str();
  }


}
