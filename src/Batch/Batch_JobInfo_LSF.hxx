/*
 * JobInfo_LSF.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Fri Nov 21 09:42:05 2003
 * Projet : Salome 2
 *
 */

#ifndef _JOBINFO_LSF_H_
#define _JOBINFO_LSF_H_

extern "C" {

#include <lsf/lsf.h>
#include <lsf/lsbatch.h>
}
#include <string>
#include "Batch_RunTimeException.hxx"
#include "Batch_JobInfo.hxx"

namespace Batch {

  class JobInfo_LSF : public JobInfo
  {
  public:
    // Constructeurs et destructeur
    JobInfo_LSF() : _running(false) {};
    JobInfo_LSF(int id);
    virtual ~JobInfo_LSF();

    // Constructeur par recopie
    JobInfo_LSF(const JobInfo_LSF & jinfo) : JobInfo(jinfo) {};

    // Teste si un job est present en machine
    virtual bool isRunning() const;

    // Methodes pour l'interfacage avec Python (SWIG)
    // TODO : supprimer ces methodes et transferer leur definitions dans SWIG
    string  __str__() const; // SWIG : affichage en Python
    string  __repr__() const { return __str__(); }; // SWIG : affichage en Python

  protected:
    bool _running; // etat du job en machine

  private:
    // Convertit une date HH:MM:SS en secondes
    long HMStoLong(const string &);

  };

}

#endif
