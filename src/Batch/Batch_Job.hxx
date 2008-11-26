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
 * Job.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#ifndef _JOB_H_
#define _JOB_H_

#include "Batch_Defines.hxx"


#include "Batch_Parametre.hxx"
#include "Batch_Environnement.hxx"

namespace Batch {

  class BATCH_EXPORT Job
  {
  public:
    // Constructeurs et destructeur
    Job();
    Job(Batch::Parametre param);
    Job(Batch::Environnement env);
    Job(Batch::Parametre param, Batch::Environnement env);
    virtual ~Job() {}
    
    // Operateur pour l'affichage sur un stream
    friend std::ostream & operator <<(std::ostream & os, const Job & job);

    // Accesseurs
    Batch::Parametre getParametre() const;
    void setParametre(const Batch::Parametre &);
    
    // Accesseurs
    Batch::Environnement getEnvironnement() const;
    void setEnvironnement(const Batch::Environnement &);
    
    // Methodes pour l'interfacage avec Python (SWIG)
    // TODO : supprimer ces methodes et transferer leur definitions dans SWIG
    std::string  __str__() const; // SWIG : affichage en Python
    std::string  __repr__() const { return __str__(); }; // SWIG : affichage en Python

  protected:
    Batch::Parametre _param; // table des parametres batch du job
    Batch::Environnement _env; // table des variables d'environnement

  private:

  };

}

#endif
