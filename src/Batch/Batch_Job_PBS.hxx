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
