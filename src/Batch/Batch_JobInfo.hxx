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
// See http://www.salome-platform.org/
//
/*
 * JobInfo.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Thu Nov  6 10:05:30 2003
 * Projet : Salome 2
 *
 */

#ifndef _JOBINFO_H_
#define _JOBINFO_H_

using namespace std;
#include <iostream>
#include <string>
#include "Batch_Parametre.hxx"
#include "Batch_Environnement.hxx"

namespace Batch {

  class JobInfo
  {
  public:
    // Constructeur standard et destructeur
    JobInfo() : _param(), _env() {};
    virtual ~JobInfo();

    // Constructeur par recopie
    JobInfo(const JobInfo & jinfo) : _param(jinfo._param), _env(jinfo._env) {};

    // Operateur pour l'affichage sur un stream
    friend ostream & operator <<(ostream & os, const JobInfo & ji);

    // Accesseurs
    // _CS_gbo Ajout explicite du namespace pour les besoins de swig (mauvaise gestion
    // des namespace par swig.
    virtual Batch::Parametre getParametre() const;
    virtual Batch::Environnement getEnvironnement() const; 

    // Methodes pour l'interfacage avec Python (SWIG)
    // TODO : supprimer ces methodes et transferer leur definitions dans SWIG
    string  __str__() const; // SWIG : affichage en Python
    string  __repr__() const { return __str__(); }; // SWIG : affichage en Python

  protected:
    Parametre _param; // parametres du job
    Environnement _env; // variables d'environnement du job
    JobInfo(const Parametre & param, const Environnement & env) : _param(param), _env(env) {};

  private:

  };

}

#endif
