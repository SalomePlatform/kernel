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


#include "Batch_Parametre.hxx"
#include "Batch_Environnement.hxx"

namespace Batch {

  class Job
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
