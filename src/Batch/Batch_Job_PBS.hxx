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
 * Job_PBS.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Fri Nov 14 11:00:39 2003
 * Projet : Salome 2
 *
 */

#ifndef _JOB_PBS_H_
#define _JOB_PBS_H_

extern "C" {

#include <pbs_error.h>
#include <pbs_ifl.h>
}
#include "Batch_Job.hxx"

namespace Batch {

  class Job_PBS
  {
  public:
    // Constructeur et destructeur
    Job_PBS(const Job & job);
    virtual ~Job_PBS();

    // Accesseurs
    struct attropl * getAttributesOP();
    struct attrl   * getAttributes();
    char *           getScript();
    char *           getDestination();

  protected:
    struct attropl * _p_attropl; // liste chainee d'attributs + operateur
    struct attrl   * _p_attrl; // liste chainee d'attributs
    char * _script; // chemin d'acces au script du job
    char * _destination; // queue dans laquelle le job est soumis

  private:
    // Ajoute un element (name,resource,value) a la liste chainee d'attributs + operateur
    void setResourceAttributeOP(struct attropl ** attr_list, const char * attr_name, const char * attr_resource, const char * attr_value);

    // Recherche un element (name,resource,value) dans la liste chainee d'attributs + operateur
    struct attropl * findResourceAttributeOP(struct attropl * attr_list, const char * attr_name, const char * attr_resource);

    // Recherche le dernier element de la liste chainee d'attributs + operateur
    struct attropl * lastAttributeOP(struct attropl * attr_list);

    // Convertit un objet Parametre en liste chainee d'attributs + operateur
    struct attropl * ParametreToAttributeOPList(struct attropl ** _p_attr_list, Parametre & param);

    // Convertit un objet Environnement en liste chainee d'attributs + operateur
    struct attropl * EnvironnementToAttributeOPList(struct attropl ** _p_attr_list, Environnement & env);

    // Ajoute les variables d'environnement presentes dans tout job PBS
    void addPBSEnvironnement(Environnement & E);


    // Ajoute un element (name,resource,value) a la liste chainee d'attributs
    void setResourceAttribute(struct attrl ** attr_list, const char * attr_name, const char * attr_resource, const char * attr_value);

    // Recherche un element (name,resource,value) dans la liste chainee d'attributs
    struct attrl   * findResourceAttribute(struct attrl * attr_list, const char * attr_name, const char * attr_resource);

    // Recherche le dernier element de la liste chainee d'attributs
    struct attrl   * lastAttribute(struct attrl * attr_list);

  };

}

#endif
