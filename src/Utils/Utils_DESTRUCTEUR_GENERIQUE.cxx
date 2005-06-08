//  SALOME Utils : general SALOME's definitions and tools
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : Utils_DESTRUCTEUR_GENERIQUE.cxx
//  Author : Antoine YESSAYAN, EDF
//  Module : SALOME
//  $Header$


# include <iostream>
# include <list>
extern "C"
{
# include <stdlib.h>
}

# include "Utils_DESTRUCTEUR_GENERIQUE.hxx"
# include "utilities.h"
using namespace std;

void Nettoyage();

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif

std::list<DESTRUCTEUR_GENERIQUE_*> *DESTRUCTEUR_GENERIQUE_::Destructeurs=0 ;

/*! \class ATEXIT_
 *
 * Mécanisme pour faire exécuter une seule fois DESTRUCTEUR_GENERIQUE_::Nettoyage
 * à la fin du traitement : creation d'un singleton statique de l'objet
 * tres specialise ATEXIT_.
 *
 * La création d'un objet de type ATEXIT_ entraîne l'inscription de la fonction
 * Nettoyage() par atexit(). Il suffit donc de créer un singleton statique du type ATEXIT_
 * pour effectuer cet enregistrement une seule fois indépendament de l'utilisateur.
 */

//CCRT
static bool ATEXIT_Done = false ;
//CCRT

class ATEXIT_
{
public :
	/*!
	 * Allocation dynamique de Destructeurs, une liste chaînée de DESTRUCTEUR_GENERIQUE_* et enregistrement
	 * de la fonction Nettoyage() par atexit().
	 *
	 * La liste chaînée Destructeurs est détruite dans la fonction Nettoyage.
	 */
        //CCRT	ATEXIT_( void )
        ATEXIT_( bool Make_ATEXIT )
	{
	  //CCRT
	  if ( Make_ATEXIT && !ATEXIT_Done ) {
	    //CCRT
		ASSERT (DESTRUCTEUR_GENERIQUE_::Destructeurs==0);
		if(MYDEBUG) MESSAGE("Construction ATEXIT"); // message necessaire pour utiliser logger dans Nettoyage (cf.BUG KERNEL4561)
		DESTRUCTEUR_GENERIQUE_::Destructeurs = 
                      new std::list<DESTRUCTEUR_GENERIQUE_*> ; // Destructeurs alloué dynamiquement (cf. ci-dessous) ,
								   // il est utilisé puis détruit par la fonction Nettoyage
		int cr = atexit( Nettoyage );                      // exécute Nettoyage lors de exit, après la destruction des données statiques !
		ASSERT(cr==0) ;
		ATEXIT_Done = true ;
	  }
	}

	~ATEXIT_( )
	{
		if(MYDEBUG) MESSAGE("Destruction ATEXIT") ;
	}
};




static ATEXIT_ nettoyage = ATEXIT_( false );	/* singleton statique */


/*!
 * traitement effectué :
 * -# exécution de tous les objets de type DESTRUCTEUR_DE_ stockés dans la liste Destructeurs (ce qui détruit les
 *    singletons correspondant) ;
 * -# puis destruction de tous les objets de type DESTRUCTEUR_DE_ stockés dans la liste Destructeurs;
 * -# destruction de la liste Destructeurs.
 */

void Nettoyage( void )
{
	if(MYDEBUG) BEGIN_OF("Nettoyage( void )") ;
	ASSERT(DESTRUCTEUR_GENERIQUE_::Destructeurs) ;
	if(MYDEBUG) SCRUTE( DESTRUCTEUR_GENERIQUE_::Destructeurs->size() ) ;
	if( DESTRUCTEUR_GENERIQUE_::Destructeurs->size() )
	{
		std::list<DESTRUCTEUR_GENERIQUE_*>::iterator it = DESTRUCTEUR_GENERIQUE_::Destructeurs->end() ;

		do
		{
			if(MYDEBUG) MESSAGE( "DESTRUCTION d'un SINGLETON");
			it-- ;
			DESTRUCTEUR_GENERIQUE_* ptr = *it ;
			//DESTRUCTEUR_GENERIQUE_::Destructeurs->remove( *it ) ;
			(*ptr)() ;
			delete ptr ;
		}while( it!=  DESTRUCTEUR_GENERIQUE_::Destructeurs->begin() ) ;

		DESTRUCTEUR_GENERIQUE_::Destructeurs->clear() ;
		if(MYDEBUG) SCRUTE( DESTRUCTEUR_GENERIQUE_::Destructeurs->size() ) ;
		ASSERT( DESTRUCTEUR_GENERIQUE_::Destructeurs->size()==0 ) ;
		ASSERT( DESTRUCTEUR_GENERIQUE_::Destructeurs->empty() ) ;
	}

	delete DESTRUCTEUR_GENERIQUE_::Destructeurs;
	DESTRUCTEUR_GENERIQUE_::Destructeurs=0;
	if(MYDEBUG) END_OF("Nettoyage( void )") ;
	return ;
}


/*!
 * Adds a destruction object to the list of actions to be performed at the end
 * of the process
 */
const int DESTRUCTEUR_GENERIQUE_::Ajout( DESTRUCTEUR_GENERIQUE_ &objet )
{
	// N.B. : l'ordre de creation des SINGLETON etant important
	//        on n'utilise pas deux fois la meme position pour
	//        les stocker dans la pile des objets.

        //CCRT
        if ( !ATEXIT_Done ) {
          nettoyage = ATEXIT_( true ) ;
	}
	//CCRT
	ASSERT(Destructeurs) ;
	Destructeurs->push_back( &objet ) ;
	return Destructeurs->size() ;
}
