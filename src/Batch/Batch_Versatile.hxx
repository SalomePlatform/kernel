/*
 * Versatile.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#ifndef _VERSATILE_H_
#define _VERSATILE_H_

using namespace std;
#include <iostream>
#include <list>
#include <string>
#include "Batch_GenericType.hxx"
#include "Batch_IntType.hxx"
#include "Batch_BoolType.hxx"
#include "Batch_CharType.hxx"
#include "Batch_LongType.hxx"
#include "Batch_StringType.hxx"
#include "Batch_CoupleType.hxx"
#include "Batch_TypeMismatchException.hxx"
#include "Batch_ListIsFullException.hxx"

namespace Batch {

	// Les types autorises
  // enum DiscriminatorType { UNDEFINED, BOOL, CHAR, INT, LONG, STRING};
  enum DiscriminatorType { UNDEFINED, LONG, STRING, COUPLE };

  typedef struct {
    DiscriminatorType type; // le type de l'element interne
    int maxelem; // le nombre d'elements autorises
  } TypeParam;

  class Versatile : public list< GenericType * >
  {
  public:
		// Constructeur standard et destructeur
    Versatile() : _discriminator(UNDEFINED), _maxsize(1), _name("undefined") {}
    virtual ~Versatile();

		// Constructeur par recopie
    Versatile(const Versatile & V);

		// Constructeur depuis le type de "base"
    Versatile(long   l) : _discriminator(LONG), _maxsize(1), _name("long")   { push_back(new LongType(l)); }
    Versatile(const string & s) : _discriminator(STRING), _maxsize(1), _name("string") { push_back(new StringType(s)); }
    Versatile(const Couple & c) : _discriminator(COUPLE), _maxsize(1), _name("couple") { push_back(new CoupleType(c)); }

		// Operateur d'affectation et de concatenation a partir d'un type de "base"
    Versatile & operator = (const long     l)    throw(TypeMismatchException);
    Versatile & operator = (const string & ch)   throw(TypeMismatchException);
    Versatile & operator +=(const string & ch)   throw(TypeMismatchException,ListIsFullException);
    Versatile & operator , (const string & ch)   throw(TypeMismatchException,ListIsFullException);
    Versatile & operator = (const Couple & cp)   throw(TypeMismatchException);
    Versatile & operator +=(const Couple & cp)   throw(TypeMismatchException,ListIsFullException);
    Versatile & operator , (const Couple & cp)   throw(TypeMismatchException,ListIsFullException);

		// Operateur d'affectation entre objets
    Versatile & operator = (const Versatile & V) throw(TypeMismatchException);

		// Conversion de type vers un type de "base"
    operator long() const throw(TypeMismatchException);
    operator string() const throw(TypeMismatchException);
    operator Couple() const throw(TypeMismatchException);
    string str() const throw(TypeMismatchException);

		// Operateur pour l'affichage sur un stream
    friend ostream & operator << (ostream & os, const Versatile & );

		// Positionnement et recuperation du type de l'element interne
    void setType(DiscriminatorType) throw(TypeMismatchException);
    DiscriminatorType getType() const;

		// Positionnement et recuperation du nombre d'elements internes
    void setMaxSize(int i);
		int getMaxSize() const { return _maxsize; }

		// Positionnement et recuperation du nom de l'objet
    string getName() const;
    void setName(const string & name);

  protected:
		// Efface tous les elements internes de l'objet
    virtual void eraseAll();

    DiscriminatorType _discriminator; // type de l'element interne
    int _maxsize; // nombre max d'elements internes
    string _name; // nom de l'objet (sert pour les exceptions)

  private:

  };

}

#endif

