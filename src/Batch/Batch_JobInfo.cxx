/*
 * JobInfo.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Thu Nov  6 10:05:30 2003
 * Projet : Salome 2
 *
 */

#include <iostream>
#include <string>
#include <sstream>
//#include "MEDMEM_STRING.hxx"
#include "Batch_JobInfo.hxx"

namespace Batch {

  // Destructeur
  JobInfo::~JobInfo()
  {
    // Nothing to do
  }
  

  // Operateur pour l'affichage sur un stream
  ostream & operator <<(ostream & os, const JobInfo & ji)
  {
    return os << ji.__str__();
  }


  // Methodes pour l'interfacage avec Python (SWIG) : affichage en Python
  string JobInfo::__str__() const
  {
    //MEDMEM::STRING sst; 
    ostringstream sst;
    sst << "<JobInfo (" << this << ") :" << endl;
    sst << " ID = " <<_param[ID] << endl;

    sst << "  + Parametre :" << endl;
    Parametre::const_iterator itp;
    for(itp=_param.begin(); itp!=_param.end(); itp++) {
      if ( (*itp).first != ID ) {
	sst << "    * " << (*itp).first << " = " << (*itp).second << endl;
      }
    }

    sst << "  + Environnement :" << endl;
    Environnement::const_iterator ite;
    for(ite=_env.begin(); ite!=_env.end(); ite++) {
      sst << "    * " << (*ite).first << " = " << (*ite).second << endl;
    }

    sst << " >";

    return sst.str();
  }

  // Accesseur
  Parametre JobInfo::getParametre() const
  {
    return _param;
  }

  // Accesseur
  Environnement JobInfo::getEnvironnement() const
  {
    return _env;
  }


}
