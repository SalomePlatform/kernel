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
    Job(Parametre param);
    Job(Environnement env);
    Job(Parametre param, Environnement env);
    virtual ~Job() {}
    
    // Operateur pour l'affichage sur un stream
    friend std::ostream & operator <<(std::ostream & os, const Job & job);

    // Accesseurs
    Parametre getParametre() const;
    void setParametre(const Parametre &);
    
    // Accesseurs
    Environnement getEnvironnement() const;
    void setEnvironnement(const Environnement &);
    
    // Methodes pour l'interfacage avec Python (SWIG)
    // TODO : supprimer ces methodes et transferer leur definitions dans SWIG
    std::string  __str__() const; // SWIG : affichage en Python
    std::string  __repr__() const { return __str__(); }; // SWIG : affichage en Python

  protected:
    Parametre _param; // table des parametres batch du job
    Environnement _env; // table des variables d'environnement

  private:

  };

}

#endif
