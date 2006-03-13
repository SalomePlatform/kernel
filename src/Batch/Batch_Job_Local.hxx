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
