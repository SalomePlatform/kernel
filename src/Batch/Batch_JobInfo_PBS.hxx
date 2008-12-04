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
 * JobInfo_PBS.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Fri Nov 21 09:42:05 2003
 * Projet : Salome 2
 *
 */

#ifndef _JOBINFO_PBS_H_
#define _JOBINFO_PBS_H_

extern "C" {

#include <pbs_error.h>
#include <pbs_ifl.h>
}
#include <string>
#include "Batch_RunTimeException.hxx"
#include "Batch_JobInfo.hxx"

namespace Batch {

  class JobInfo_PBS : public JobInfo
  {
  public:
    // Constructeurs et destructeur
    JobInfo_PBS() : JobInfo() {};
    JobInfo_PBS(struct batch_status * stat_list, bool tobedeleted = false);
    virtual ~JobInfo_PBS();

    // Constructeur par recopie
    JobInfo_PBS(const JobInfo_PBS & jinfo) : JobInfo(jinfo) {};

    // Methodes pour l'interfacage avec Python (SWIG)
    // TODO : supprimer ces methodes et transferer leur definitions dans SWIG
    std::string  __str__() const; // SWIG : affichage en Python
    std::string  __repr__() const { return __str__(); }; // SWIG : affichage en Python

  protected:

  private:
    // Convertit une date HH:MM:SS en secondes
    long HMStoLong(const std::string &);

  };

}

#endif
