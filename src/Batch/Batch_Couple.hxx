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

using namespace std;
#include <string>

namespace Batch {

  class Couple
  {
  public:
    // Constructeur standard
    Couple(const string & local="", const string & remote="") : _local(local), _remote(remote) {}

    // Constructeur par recopie
    Couple(const Couple & C) : _local(C._local), _remote(C._remote) {}

    // Operateur pour l'affichage sur un stream
    friend ostream & operator << (ostream & os, const Couple & cp);

    // Operateur d'affectation
    virtual Couple & operator =(const Couple &);

    // Conversion en chaine
    virtual string str() const;

    // Accesseurs
    virtual string getLocal() const { return _local; }
    virtual string getRemote() const { return _remote; }

  protected:
    string _local;  // chemin d'acces au fichier local
    string _remote; // chemin d'acees au fichier distant

  private:

  };

}

#endif

