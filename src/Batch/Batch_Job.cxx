/*
 * Job.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#include "Batch_Job.hxx"
#include "Batch_Parametre.hxx"
#include <sstream>
//#include "MEDMEM_STRING.hxx"
using namespace std;

namespace Batch {

  // Constructeur
  Job::Job() : _param(), _env()
  {
    // Nothing to do
  }


  // Constructeur
  Job::Job(Parametre param) : _param(param), _env()
  {
    // Nothing to do
  }


  // Constructeur
  Job::Job(Environnement env) : _param(), _env(env)
  {
    // Nothing to do
  }


  // Constructeur
  Job::Job(Parametre param, Environnement env) : _param(param), _env(env)
  {
    // Nothing to do
  }

  // Operateur pour l'affichage sur un stream
  ostream & operator <<(ostream & os, const Job & job)
  {
    return os << job.__str__();
  }

  // Accesseur
  Parametre Job::getParametre() const
  {
    return _param;
  }

  // Accesseur
  void Job::setParametre(const Parametre & param)
  {
    _param = param;
  }

  // Accesseur
  Environnement Job::getEnvironnement() const
  {
    return _env;
  }

  // Accesseur
  void Job::setEnvironnement(const Environnement & env)
  {
    _env = env;
  }


  // Methode pour l'interfacage avec Python (SWIG) : affichage en Python
  string Job::__str__() const {
    //MEDMEM::STRING str;
    ostringstream str;
    str << "<Job (" << this << ") :" << endl;
    str << "  Parametre :" << endl;
    Parametre::const_iterator itp;
    for(itp=_param.begin(); itp!=_param.end(); itp++) {
      str << "   * " << (*itp).first << " : " << (*itp).second << endl;
    }
    str << "  Environnement :" << endl;
    Environnement::const_iterator ite;
    for(ite=_env.begin(); ite!=_env.end(); ite++) {
      str << "   * " << (*ite).first << " : " << (*ite).second << endl;
    }
    str << " >";
    return str.str();
  }


}
