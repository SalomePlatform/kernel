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
 * Job_LSF.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Fri Nov 14 11:00:39 2003
 * Projet : Salome 2
 *
 */

#ifndef _JOB_LSF_H_
#define _JOB_LSF_H_

extern "C" {

#include <lsf/lsf.h>
#include <lsf/lsbatch.h>
}
#include <string>
#include "Batch_Job.hxx"

namespace Batch {

  class Job_LSF
  {
  public:
    // Constructeur et destructeur
    Job_LSF(const Job & job);
    virtual ~Job_LSF();

    // Accesseurs
    struct submit * getSubmitStruct();

  protected:
    struct submit * _p_submit; // structure pour soumettre les jobs

  private:
    struct submit * ParametreToSubmitStruct(const Parametre & P);
    char * string2char(const std::string &);
  };

}

#endif
