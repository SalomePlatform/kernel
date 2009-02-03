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
 * JobInfo.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Thu Nov  6 10:05:30 2003
 * Projet : Salome 2
 *
 */

#include <iostream>
#include <string>
#include <sstream>
//#include "MEDMEM_STRING.hxx"
#include "Batch_JobInfo.hxx"

using namespace std;

namespace Batch {

  // Destructeur
  JobInfo::~JobInfo()
  {
    // Nothing to do
  }
  

  // Operateur pour l'affichage sur un stream
  ostream & operator <<(ostream & os, const JobInfo & ji)
  {
    return os << ji.__str__();
  }


  // Methodes pour l'interfacage avec Python (SWIG) : affichage en Python
  string JobInfo::__str__() const
  {
    //MEDMEM::STRING sst; 
    ostringstream sst;
    sst << "<JobInfo (" << this << ") :" << endl;
    sst << " ID = " <<_param[ID] << endl;

    sst << "  + Parametre :" << endl;
    Parametre::const_iterator itp;
    for(itp=_param.begin(); itp!=_param.end(); itp++) {
      if ( (*itp).first != ID ) {
	sst << "    * " << (*itp).first << " = " << (*itp).second << endl;
      }
    }

    sst << "  + Environnement :" << endl;
    Environnement::const_iterator ite;
    for(ite=_env.begin(); ite!=_env.end(); ite++) {
      sst << "    * " << (*ite).first << " = " << (*ite).second << endl;
    }

    sst << " >";

    return sst.str();
  }

  // Accesseur
  Parametre JobInfo::getParametre() const
  {
    return _param;
  }

  // Accesseur
  Environnement JobInfo::getEnvironnement() const
  {
    return _env;
  }


}
