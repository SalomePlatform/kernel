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
    string  __str__() const; // SWIG : affichage en Python
    string  __repr__() const { return __str__(); }; // SWIG : affichage en Python

  protected:

  private:
    // Convertit une date HH:MM:SS en secondes
    long HMStoLong(const string &);

  };

}

#endif
