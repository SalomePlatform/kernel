/*
 * Job_Local.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Fri Nov 14 11:00:39 2003
 * Projet : Salome 2
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Batch_Job_Local.hxx"

namespace Batch {

  // Constructeur
  Job_Local::Job_Local(const Job & job) : 
    _command(), _param(job.getParametre()), _env(job.getEnvironnement())
  {
    // On positionne le nom du EXECUTIONHOST a "localhost" s'il n'est pas precise
    if ( _param.find(EXECUTIONHOST) == _param.end() ) {
      _param[EXECUTIONHOST] = "localhost";
    }

    // On convertit les objets Parametre et Environnement en liste chainee d'attributs + operateur
    addEnvironnement( _env   );
    addParametre    ( _param );

  }


  // Destructeur
  Job_Local::~Job_Local()
  {
  }


  void Job_Local::addParametre(const Parametre & P)
  {
    // En dernier, on ajoute le chemin complet de la commande
    _command += P[EXECUTABLE].str();
  }


  void Job_Local::addEnvironnement(const Environnement & E)
  {
    for(Environnement::const_iterator it=E.begin(); it != E.end(); it++) {
      string variable = (*it).first;
      string value    = (*it).second;

      // On remplace toutes les occurences de single-quote par backslash-single-quote
      for(int pos=0; pos < value.size(); pos++) {
	pos = value.find("'", pos);
	if ( (pos < 0) || (pos > value.size()) ) break;
	value.replace(pos, 1, "\'");
      }
      _command += variable + "='" + value + "' ";
    }
  }

  string Job_Local::getCommand(void) const {
    return _command;
  }


  // Retourne l'objet Parametre
  Parametre Job_Local::getParametre() const
  {
    return _param;
  }

  // Retourne l'objet Environnement
  Environnement Job_Local::getEnvironnement() const
  {
    return _env;
  }


}
