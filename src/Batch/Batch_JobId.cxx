/*
 * JobId.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#include "Batch_JobId.hxx"
#include "Batch_BatchManager.hxx"
#include <sstream>
//#include "MEDMEM_STRING.hxx"

namespace Batch {

  // Constructeur standard
  JobId::JobId() : _p_batchmanager(), _reference("undefined")
  {
    // Nothing to do
  }

  // Constructeur avec le pointeur sur le BatchManager associe et avec une reference
  JobId::JobId(BatchManager * _p_bm, string ref) : _p_batchmanager(_p_bm), _reference(ref)
  {
    // Nothing to do
  }

  // Destructeur
  JobId::~JobId()
  {
    // Nothing to do
  }

  // Operateur d'affectation entre objets
  JobId & JobId::operator =(const JobId & jobid)
  {
    _p_batchmanager = jobid._p_batchmanager;
    _reference      = jobid._reference;

    return *this;
  }

  // Constructeur par recopie
  JobId::JobId(const JobId & jobid) : _p_batchmanager(jobid._p_batchmanager), _reference(jobid._reference)
  {
    // Nothing to do
  }

  // Accesseur pour la reference interne
  string JobId::getReference() const
  {
    return _reference;
  }

  // Methode pour le controle du job : retire le job du gestionnaire
  void JobId::deleteJob() const
  {
    assert(_p_batchmanager != 0);
    _p_batchmanager->deleteJob(*this);
  }
   
  // Methode pour le controle du job : suspend le job en file d'attente
  void JobId::holdJob() const
  {
    assert(_p_batchmanager != 0);
    _p_batchmanager->holdJob(*this);
  }

  // Methode pour le controle du job : relache le job suspendu
  void JobId::releaseJob() const
  {
    assert(_p_batchmanager != 0);
    _p_batchmanager->releaseJob(*this);
  }

  // Methode pour le controle du job : modifie le job en file d'attente
  void JobId::alterJob(const Parametre & param, const Environnement & env) const
  {
    assert(_p_batchmanager != 0);
    _p_batchmanager->alterJob(*this, param, env);
  }

  // Methode pour le controle du job : modifie le job en file d'attente
  void JobId::alterJob(const Parametre & param) const
  {
    assert(_p_batchmanager != 0);
    _p_batchmanager->alterJob(*this, param);
  }

  // Methode pour le controle du job : modifie le job en file d'attente
  void JobId::alterJob(const Environnement & env) const
  {
    assert(_p_batchmanager != 0);
    _p_batchmanager->alterJob(*this, env);
  }

  // Methode pour le controle du job : renvoie l'etat du job
  JobInfo JobId::queryJob() const
  {
    assert(_p_batchmanager != 0);
    return _p_batchmanager->queryJob(*this);
  }


  // Methode pour l'interfacage avec Python (SWIG) : affichage en Python
  string JobId::__str__() const {
    //MEDMEM::STRING str;
    ostringstream str;
    str << "<JobId (" << this << ") : referenced '" << _reference << "'>";
    return str.str();
  }

}
