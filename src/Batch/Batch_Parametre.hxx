/*
 * Parametre.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#ifndef _PARAMETRE_H_
#define _PARAMETRE_H_

using namespace std;
#include <map>
#include <string>
#include "Batch_InvalidKeyException.hxx"
#include "Batch_Versatile.hxx"


// Ces macros permettent de simplifier l'ajout de nouvelles
// clefs dans la map Parametre
// TODO : remplacer ce mecanisme statique par la lecture
// TODO : d'une descrption dans un fichier exterieur (genre XML)

#define def_extern_MapKey(mk) extern const string & mk;
#define def_static_MapKey(mk) const string Batch::Parametre::mk(#mk); \
                              const string & mk = Batch::Parametre::mk;

namespace Batch {

  class Parametre : public map< string, Versatile >
  {
  public:
		// Constructeur standard
    Parametre();

		// Constructeur par recopie
    Parametre::Parametre(const Parametre & PM);

		// Operateur de recherche dans la map
    Versatile & operator [] (const string &);
    const Versatile & operator [] (const string &) const;

		// Operateur d'affectation
    Parametre & operator =(const Parametre & PM);

		// Declarations statique des clefs de la map
		// TODO : supprimer les declarations statiques des clefs de la map
    static const string ACCOUNT;
    static const string CHECKPOINT;
    static const string CKPTINTERVAL;
    static const string CREATIONTIME;
    static const string EGROUP;
    static const string ELIGIBLETIME;
    static const string EUSER;
    static const string EXECUTABLE;
    static const string EXECUTIONHOST;
    static const string HOLD;
    static const string ID;
    static const string INFILE;
    static const string MAIL;
    static const string MAXCPUTIME;
    static const string MAXDISKSIZE;
    static const string MAXRAMSIZE;
    static const string MAXWALLTIME;
    static const string MODIFICATIONTIME;
    static const string NAME;
    static const string OUTFILE;
    static const string PID;
    static const string QUEUE;
    static const string QUEUEDTIME;
    static const string SERVER;
    static const string STARTDATE;
    static const string STATE;
    static const string TEXT;
    static const string TMPDIR;
    static const string USEDCPUTIME;
    static const string USEDDISKSIZE;
    static const string USEDRAMSIZE;
    static const string USEDWALLTIME;
    static const string USER;

  protected:
    map< string, TypeParam > TypeMap; // map interne servant a controler le type de la valeur associee a chaque clef

  private:

  };

}

def_extern_MapKey(ACCOUNT);
def_extern_MapKey(CHECKPOINT);
def_extern_MapKey(CKPTINTERVAL);
def_extern_MapKey(CREATIONTIME);
def_extern_MapKey(EGROUP);
def_extern_MapKey(ELIGIBLETIME);
def_extern_MapKey(EUSER);
def_extern_MapKey(EXECUTABLE);
def_extern_MapKey(EXECUTIONHOST);
def_extern_MapKey(HOLD);
def_extern_MapKey(ID);
def_extern_MapKey(INFILE);
def_extern_MapKey(MAIL);
def_extern_MapKey(MAXCPUTIME);
def_extern_MapKey(MAXDISKSIZE);
def_extern_MapKey(MAXRAMSIZE);
def_extern_MapKey(MAXWALLTIME);
def_extern_MapKey(MODIFICATIONTIME);
def_extern_MapKey(NAME);
def_extern_MapKey(OUTFILE);
def_extern_MapKey(PID);
def_extern_MapKey(QUEUE);
def_extern_MapKey(QUEUEDTIME);
def_extern_MapKey(SERVER);
def_extern_MapKey(STARTDATE);
def_extern_MapKey(STATE);
def_extern_MapKey(TEXT);
def_extern_MapKey(TMPDIR);
def_extern_MapKey(USEDCPUTIME);
def_extern_MapKey(USEDDISKSIZE);
def_extern_MapKey(USEDRAMSIZE);
def_extern_MapKey(USEDWALLTIME);
def_extern_MapKey(USER);

#endif
