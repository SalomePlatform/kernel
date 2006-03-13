/*
 * JobInfo_Local.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Fri Nov 21 09:42:05 2003
 * Projet : Salome 2
 *
 */

#ifndef _JOBINFO_LOCAL_H_
#define _JOBINFO_LOCAL_H_

#include <string>
#include "Batch_JobInfo.hxx"

namespace Batch {

  class JobInfo_Local : public JobInfo
  {
  public:
    // Constructeurs et destructeur
    JobInfo_Local() {};
    JobInfo_Local(const Parametre & param, const Environnement & env);
    virtual ~JobInfo_Local();

    // Constructeur par recopie
    JobInfo_Local(const JobInfo_Local & jinfo) : JobInfo(jinfo) {};


  protected:

  private:

  };

}

#endif
