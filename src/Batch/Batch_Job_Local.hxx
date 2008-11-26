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
 * Job_Local.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Fri Nov 14 11:00:39 2003
 * Projet : Salome 2
 *
 */

#ifndef _JOB_LOCAL_H_
#define _JOB_LOCAL_H_

#include "Batch_Parametre.hxx"
#include "Batch_Environnement.hxx"
#include "Batch_Job.hxx"

namespace Batch {

  class Job_Local
  {
  public:
    // Constructeur et destructeur
    Job_Local(const Job & job);
    virtual ~Job_Local();

    // Retourne la commande _command
    virtual std::string getCommand(void) const;

    // Retourne l'objet Parametre
    virtual Parametre getParametre() const;

    // Retourne l'objet Environnement
    virtual Environnement getEnvironnement() const;


  protected:
    std::string _command;
    Parametre _param;
    Environnement _env;

  private:
    void addParametre(const Parametre & P);
    void addEnvironnement(const Environnement & E);

  };

}

#endif
