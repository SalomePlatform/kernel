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
 * Job.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#include "Batch_Job.hxx"
#include "Batch_Parametre.hxx"
#include <sstream>
//#include "MEDMEM_STRING.hxx"
using namespace std;

namespace Batch {

  // Constructeur
  Job::Job() : _param(), _env()
  {
    // Nothing to do
  }


  // Constructeur
  Job::Job(Parametre param) : _param(param), _env()
  {
    // Nothing to do
  }


  // Constructeur
  Job::Job(Environnement env) : _param(), _env(env)
  {
    // Nothing to do
  }


  // Constructeur
  Job::Job(Parametre param, Environnement env) : _param(param), _env(env)
  {
    // Nothing to do
  }

  // Operateur pour l'affichage sur un stream
  ostream & operator <<(ostream & os, const Job & job)
  {
    return os << job.__str__();
  }

  // Accesseur
  Batch::Parametre Job::getParametre() const
  {
    return _param;
  }

  // Accesseur
  void Job::setParametre(const Batch::Parametre & param)
  {
    _param = param;
  }

  // Accesseur
  Environnement Job::getEnvironnement() const
  {
    return _env;
  }

  // Accesseur
  void Job::setEnvironnement(const Environnement & env)
  {
    _env = env;
  }


  // Methode pour l'interfacage avec Python (SWIG) : affichage en Python
  string Job::__str__() const {
    //MEDMEM::STRING str;
    ostringstream str;
    str << "<Job (" << this << ") :" << endl;
    str << "  Parametre :" << endl;
    Parametre::const_iterator itp;
    for(itp=_param.begin(); itp!=_param.end(); itp++) {
      str << "   * " << (*itp).first << " : " << (*itp).second << endl;
    }
    str << "  Environnement :" << endl;
    Environnement::const_iterator ite;
    for(ite=_env.begin(); ite!=_env.end(); ite++) {
      str << "   * " << (*ite).first << " : " << (*ite).second << endl;
    }
    str << " >";
    return str.str();
  }


}
