//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
/*
 * Versatile.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <assert.h>
//#include "MEDMEM_STRING.hxx"
#include "Batch_GenericType.hxx"
#include "Batch_IntType.hxx"
#include "Batch_BoolType.hxx"
#include "Batch_CharType.hxx"
#include "Batch_LongType.hxx"
#include "Batch_StringType.hxx"
#include "Batch_Versatile.hxx"
#include "Batch_TypeMismatchException.hxx"
#include "Batch_ListIsFullException.hxx"
using namespace std;

namespace Batch {

	// Constructeur par recopie
  Versatile::Versatile(const Versatile & V) : _discriminator(V._discriminator), _maxsize(V._maxsize), _name(V._name) // , _str_value(0)
  {
    Versatile::const_iterator it;

		// On prend un a un les elements de l'objet passe en argument qu'on duplique
    for(it=V.begin(); it!=V.end(); it++)
      push_back( (*it)->clone() ); // Attention, la methode clone fait un new implicite
  }

	// Destructeur
  Versatile::~Versatile()
  {
		eraseAll();
  }

	// Operateur d'affectation entre objets
  Versatile & Versatile::operator = (const Versatile & Vrhs) throw(TypeMismatchException)
  {
		// ATTENTION : le forçage de type leve une exception TypeMismatchException entre cas de conflit
    setType(Vrhs._discriminator);
    setMaxSize(Vrhs._maxsize);
    _name = Vrhs._name;

    // On efface les donnees precedentes
    eraseAll();

    // On copie les donnees de Vrhs
    Versatile::const_iterator it;

    for(it=Vrhs.begin(); it!=Vrhs.end(); it++)
      push_back( (*it)->clone() ); // Attention, la methode clone fait un new implicite

    return *this;
  }

	// Operateur d'affectation a partir d'un long
  Versatile & Versatile::operator = (const long   l) throw(TypeMismatchException)
  {
		// ATTENTION : le forçage de type leve une exception TypeMismatchException entre cas de conflit
    setType(LONG);

    // On efface les donnees precedentes
    eraseAll();

		// On ajoute un element interne de type long a l'objet  
    LongType * pL = new LongType(l);
    assert(pL != 0);
    push_back(pL);
    return *this;
  }

	// Operateur d'affectation a partir d'une string
  Versatile & Versatile::operator = (const string & ch) throw(TypeMismatchException)
  {
		// ATTENTION : le forçage de type leve une exception TypeMismatchException entre cas de conflit
    setType(STRING);

    // On efface les donnees precedentes
    eraseAll();
  
		// On ajoute un element interne de type string a l'objet  
    StringType * pS = new StringType(ch);
    assert(pS != 0);
    push_back(pS);

    return *this;
  }

	// Operateur de concatenation a partir d'une string
  Versatile & Versatile::operator +=(const string & ch) throw(TypeMismatchException,ListIsFullException)
  {
		// ATTENTION : le forçage de type leve une exception TypeMismatchException entre cas de conflit
    setType(STRING);

		// Si la taille maximale est atteinte, on leve une exception ListIsFullException
    if (_maxsize == 0) push_back(new StringType(ch));
    else if ((_maxsize > 0) && (size() < _maxsize)) push_back(new StringType(ch));
    else {
      //MEDMEM::STRING msg;
      ostringstream msg;
      msg << "Taille maximum : " << _maxsize;
      throw(ListIsFullException(msg.str()));
    }
    return *this;
  }

	// Operateur de concatenation a partir d'une string
  Versatile & Versatile::operator , (const string & ch) throw(TypeMismatchException,ListIsFullException)
  {
    *this += ch;
    return *this;
  }

	// Operateur d'affectation a partir d'un Couple
  Versatile & Versatile::operator = (const Couple & cp) throw(TypeMismatchException)
  {
 		// ATTENTION : le forçage de type leve une exception TypeMismatchException entre cas de conflit
		setType(COUPLE);

    // On efface les donnees precedentes
    eraseAll();
  
		// On ajoute un element interne de type Couple a l'objet  
    CoupleType * pC = new CoupleType(cp);
    assert(pC != 0);
    push_back(pC);

    return *this;
  }

	// Operateur de concatenation a partir d'un Couple
  Versatile & Versatile::operator +=(const Couple & cp) throw(TypeMismatchException,ListIsFullException)
  {
 		// ATTENTION : le forçage de type leve une exception TypeMismatchException entre cas de conflit
    setType(COUPLE);

		// Si la taille maximale est atteinte, on leve une exception ListIsFullException
    if (_maxsize == 0) push_back(new CoupleType(cp));
    else if ((_maxsize > 0) && (size() < _maxsize)) push_back(new CoupleType(cp));
    else {
      //MEDMEM::STRING msg;
      ostringstream msg;
      msg << "Taille maximum : " << _maxsize;
      throw(ListIsFullException(msg.str()));
    }
    return *this;
  }

	// Operateur de concatenation a partir d'un Couple
  Versatile & Versatile::operator , (const Couple & cp) throw(TypeMismatchException,ListIsFullException)
  {
    *this += cp;
    return *this;
  }

  ostream & operator << (ostream & os, const Versatile & V)
  {
    Versatile::const_iterator it;
    const char * sep = "";

    for(it=V.begin(); it!=V.end(); it++, sep=" ") {
      string s = (*it)->affiche();
      os << sep << s;
    }
    return os;
  }

	// Positionnement du type de l'element interne
  void Versatile::setType(DiscriminatorType t) throw(TypeMismatchException)
  {
		// Si le type est deja defini et ne correspond pas au type en argument
		// une exception TypeMismatchException est levee
    if ( (_discriminator == UNDEFINED) || (_discriminator == t) )
      _discriminator = t;
    else {
      //MEDMEM::STRING sst;
      ostringstream sst;
      sst << "Trying to change type of Versatile object \""
					<< _name << "\"";
      throw(TypeMismatchException(sst.str()));
    }
  }
	
	// Positionnement du nombre d'elements internes
  void Versatile::setMaxSize(int i)
  {
    _maxsize = i;
    if (i <= 0) return;
		// Si la nouvelle taille est inferieure au nombre d'elements deja
		// presents, les elements en surplus sont effaces (troncature)
    if (size() > _maxsize)
      {
				int reste = size() - _maxsize;
				Versatile::iterator it;
				for(it=end(); (it!=begin()) && reste; it--, reste--)
					{
						delete back();
						pop_back();
					}
      }
  }


	// Conversion de type vers un long
  Versatile::operator long() const throw(TypeMismatchException)
  {
		// Si le type ne correspond pas ou si la liste contient plus d'un element,
		// la conversion est impossible et une exception TypeMismatchException 
		// est levee
    if ( (_maxsize != 1) || (_discriminator != LONG) || (size() == 0) ) {
      //MEDMEM::STRING sst;
      ostringstream sst;
      sst << "Cannot cast Versatile object \""
					<< _name << "\" to long";
      throw(TypeMismatchException(sst.str()));
    }
		return *( static_cast<LongType *>(this->front()) );
  }

	// Conversion de type vers un Couple
  Versatile::operator Couple() const throw(TypeMismatchException)
  {
		// Si le type ne correspond pas ou si la liste contient plus d'un element,
		// la conversion est impossible et une exception TypeMismatchException 
		// est levee
    if ( (_maxsize != 1) || (_discriminator != COUPLE) || (size() == 0) ) {
      //MEDMEM::STRING sst;
      ostringstream sst;
      sst << "Cannot cast Versatile object \""
					<< _name << "\" to Couple";
      throw(TypeMismatchException(sst.str()));
    }
		return *( static_cast<CoupleType *>(this->front()) );
  }

	// Conversion de type vers une string
  string Versatile::str() const throw(TypeMismatchException)
  {
		// Si le type ne correspond pas, la conversion est impossible et 
		// une exception TypeMismatchException est levee
    if ( (_discriminator != STRING) || (size() == 0) ) {
      //MEDMEM::STRING sst;
      ostringstream sst;
      sst << "Cannot cast Versatile object \""
					<< _name << "\" to string";
      throw(TypeMismatchException(sst.str()));
    }

		// La chaine renvoyee est la concatenation des chaines internes
    string s;
    Versatile::const_iterator it;
    const char * sep = "";
    for(it=begin(); it!=end(); it++, s+=sep, sep=" ")
      s += *( static_cast<StringType *>(*it));

    return s;
  }

	// Conversion de type vers une string
  Versatile::operator string () const throw(TypeMismatchException)
  {
    return str();
  }

	// Efface tous les elements internes de l'objet
  void Versatile::eraseAll()
  {
    while(!empty()) 
      {
				delete back();
				pop_back();
      }
  }


	// Recuperation du type de l'element interne
  DiscriminatorType Versatile::getType() const
  {
    return _discriminator;
  }

	// Recuperation du nom de l'objet
  string Versatile::getName() const
  {
    return _name;
  }

	// Positionnement du nom de l'objet
  void Versatile::setName(const string & name)
  {
    _name = name;
  }
}
