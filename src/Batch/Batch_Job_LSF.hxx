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
    char * string2char(const string &);
  };

}

#endif
