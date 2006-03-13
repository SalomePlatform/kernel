/*
 * JobInfo_Local.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Fri Nov 21 09:42:06 2003
 * Projet : Salome 2
 *
 */

#include <cstdio>
#include "Batch_Parametre.hxx"
#include "Batch_Environnement.hxx"
#include "Batch_JobInfo_Local.hxx"

namespace Batch {

  // Constructeurs
  JobInfo_Local::JobInfo_Local(const Parametre & param, const Environnement & env) :
    JobInfo(param, env)
  {
    // Nothing to do
  }


  // Destructeur
  JobInfo_Local::~JobInfo_Local()
  {
    // Nothing to do
  }
  

}
