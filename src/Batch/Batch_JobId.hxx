/*
 * JobId.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#ifndef _JOBID_H_
#define _JOBID_H_


#include "Batch_JobInfo.hxx"
#include "Batch_BatchManager.hxx"

namespace Batch {

  class BatchManager;

  class JobId
  {
    friend class BatchManager;

  public:
    // Constructeur standard et destructeur
    JobId();
    virtual ~JobId();

    // Constructeur avec le pointeur sur le BatchManager associe et avec une reference
    JobId(Batch::BatchManager *, std::string ref);

    // Operateur d'affectation entre objets
    virtual JobId & operator =(const Batch::JobId &);

    // Constructeur par recopie
    JobId(const Batch::JobId &);

    // Accesseur pour la reference interne
    virtual std::string getReference() const;

    // Methodes pour le controle du job
    virtual void deleteJob() const; // retire un job du gestionnaire
    virtual void holdJob() const; // suspend un job en file d'attente
    virtual void releaseJob() const; // relache un job suspendu
    virtual void alterJob(const Batch::Parametre & param, const Batch::Environnement & env) const; // modifie un job en file d'attente
    virtual void alterJob(const Batch::Parametre & param) const; // modifie un job en file d'attente
    virtual void alterJob(const Batch::Environnement & env) const; // modifie un job en file d'attente
    virtual void setParametre(const Batch::Parametre & param) { return alterJob(param); } // modifie un job en file d'attente
    virtual void setEnvironnement(const Batch::Environnement & env) { return alterJob(env); } // modifie un job en file d'attente
    virtual Batch::JobInfo queryJob() const; // renvoie l'etat du job

    // Methodes pour l'interfacage avec Python (SWIG)
    // TODO : supprimer ces methodes et transferer leur definitions dans SWIG
    std::string  __str__() const; // SWIG : affichage en Python
    std::string  __repr__() const { return __str__(); }; // SWIG : affichage en Python

  protected:
    Batch::BatchManager * _p_batchmanager; // pointeur sur le BatchManager qui controle le job
    std::string _reference; // reference du job au sein du BatchManager

  private:

  };

}

#endif
