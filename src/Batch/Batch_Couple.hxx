/*
 * Couple.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Tue Dec  9 15:00:35 2003
 * Projet : Salome 2
 *
 */

#ifndef _COUPLE_H_
#define _COUPLE_H_

#include <string>

namespace Batch {

  class Couple
  {
  public:
    // Constructeur standard
    Couple(const std::string & local="", const std::string & remote="") : _local(local), _remote(remote) {}

    // Constructeur par recopie
    Couple(const Couple & C) : _local(C._local), _remote(C._remote) {}

    // Operateur pour l'affichage sur un stream
    friend std::ostream & operator << (std::ostream & os, const Couple & cp);

    // Operateur d'affectation
    virtual Couple & operator =(const Couple &);

    // Conversion en chaine
    virtual std::string str() const;

    // Accesseurs
    virtual std::string getLocal() const { return _local; }
    virtual std::string getRemote() const { return _remote; }

  protected:
    std::string _local;  // chemin d'acces au fichier local
    std::string _remote; // chemin d'acees au fichier distant

  private:

  };

}

#endif

