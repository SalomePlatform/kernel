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
    friend std::ostream & operator <<(std::ostream & os, const JobInfo & ji);

    // Accesseurs
    virtual Parametre getParametre() const;
    virtual Environnement getEnvironnement() const; 

    // Methodes pour l'interfacage avec Python (SWIG)
    // TODO : supprimer ces methodes et transferer leur definitions dans SWIG
    std::string  __str__() const; // SWIG : affichage en Python
    std::string  __repr__() const { return __str__(); }; // SWIG : affichage en Python

  protected:
    Parametre _param; // parametres du job
    Environnement _env; // variables d'environnement du job

  private:

  };

}

#endif
