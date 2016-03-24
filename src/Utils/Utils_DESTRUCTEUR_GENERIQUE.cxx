// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

//  SALOME Utils : general SALOME's definitions and tools
//  File   : Utils_DESTRUCTEUR_GENERIQUE.cxx
//  Author : Antoine YESSAYAN, EDF
//  Module : SALOME
//  $Header$
//
# include <iostream>
# include <list>
extern "C"
{
# include <stdlib.h>
}

# include "Utils_DESTRUCTEUR_GENERIQUE.hxx"
//# include "utilities.h"
# include "LocalTraceBufferPool.hxx"
void Nettoyage();

#ifdef _DEBUG_
// static int MYDEBUG = 0;
#else
// static int MYDEBUG = 0;
#endif

std::list<DESTRUCTEUR_GENERIQUE_*> *DESTRUCTEUR_GENERIQUE_::Destructeurs=0 ;

/*! \class ATEXIT_
 *
 * Mecanisme pour faire executer une seule fois DESTRUCTEUR_GENERIQUE_::Nettoyage
 * a la fin du traitement : creation d'un singleton statique de l'objet
 * tres specialise ATEXIT_.
 *
 * La creation d'un objet de type ATEXIT_ entraine l'inscription de la fonction
 * Nettoyage() par atexit(). Il suffit donc de creer un singleton statique du type ATEXIT_
 * pour effectuer cet enregistrement une seule fois independament de l'utilisateur.
 */

//CCRT
static bool ATEXIT_Done = false ;
//CCRT

class ATEXIT_
{
public :
        /*!
         * Allocation dynamique de Destructeurs, une liste chainee de DESTRUCTEUR_GENERIQUE_* et enregistrement
         * de la fonction Nettoyage() par atexit().
         *
         * La liste chainee Destructeurs est detruite dans la fonction Nettoyage.
         */
        //CCRT  ATEXIT_( void )
        ATEXIT_( bool Make_ATEXIT )
        {
          //CCRT
          if ( Make_ATEXIT && !ATEXIT_Done ) {
            //CCRT
                assert (DESTRUCTEUR_GENERIQUE_::Destructeurs==0);
                //cerr << "ATEXIT_::ATEXIT_ Construction ATEXIT" << endl;// message necessaire pour utiliser logger dans Nettoyage (cf.BUG KERNEL4561)
                DESTRUCTEUR_GENERIQUE_::Destructeurs = 
                      new std::list<DESTRUCTEUR_GENERIQUE_*> ; // Destructeur alloue dynamiquement (cf. ci-dessous) ,
                                                                   // il est utilise puis detruit par la fonction Nettoyage
                //To be sure the trace singleton will be the last one to be destroyed initialize it here before calling atexit
                LocalTraceBufferPool::instance();
#ifndef _DEBUG_
                atexit( Nettoyage );                      // execute Nettoyage lors de exit, aprs la destruction des donnees statiques !
#else
                int cr = atexit( Nettoyage );                      // execute Nettoyage lors de exit, aprs la destruction des donnees statiques !
                assert(cr==0) ;
#endif
                ATEXIT_Done = true ;
          }
        }

        ~ATEXIT_( )
        {
          //cerr << "ATEXIT_::~ATEXIT_ Destruction ATEXIT" << endl;
        }
};




static ATEXIT_ nettoyage = ATEXIT_( false );    /* singleton statique */


/*!
 * traitement effectue :
 * -# execution de tous les objets de type DESTRUCTEUR_DE_ stockes dans la liste Destructeurs (ce qui detruit les
 *    singletons correspondant) ;
 * -# puis destruction de tous les objets de type DESTRUCTEUR_DE_ stockes dans la liste Destructeurs;
 * -# destruction de la liste Destructeurs.
 */

void Nettoyage( void )
{
  //cerr << "Nettoyage()" << endl;
  //if(MYDEBUG) BEGIN_OF("Nettoyage( void )") ;
        assert(DESTRUCTEUR_GENERIQUE_::Destructeurs) ;
        //if(MYDEBUG) SCRUTE( DESTRUCTEUR_GENERIQUE_::Destructeurs->size() ) ;
        if( DESTRUCTEUR_GENERIQUE_::Destructeurs->size() )
        {
                std::list<DESTRUCTEUR_GENERIQUE_*>::iterator it = DESTRUCTEUR_GENERIQUE_::Destructeurs->end() ;

                do
                {
                  //if(MYDEBUG) MESSAGE( "DESTRUCTION d'un SINGLETON");
                        it-- ;
                        DESTRUCTEUR_GENERIQUE_* ptr = *it ;
                        //DESTRUCTEUR_GENERIQUE_::Destructeurs->remove( *it ) ;
                        (*ptr)() ;
                        delete ptr ;
                }while( it!=  DESTRUCTEUR_GENERIQUE_::Destructeurs->begin() ) ;

                DESTRUCTEUR_GENERIQUE_::Destructeurs->clear() ;
                //if(MYDEBUG) SCRUTE( DESTRUCTEUR_GENERIQUE_::Destructeurs->size() ) ;
                assert( DESTRUCTEUR_GENERIQUE_::Destructeurs->size()==0 ) ;
                assert( DESTRUCTEUR_GENERIQUE_::Destructeurs->empty() ) ;
        }

        delete DESTRUCTEUR_GENERIQUE_::Destructeurs;
        DESTRUCTEUR_GENERIQUE_::Destructeurs=0;
        //if(MYDEBUG) END_OF("Nettoyage( void )") ;
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
        assert(Destructeurs) ;
        Destructeurs->push_back( &objet ) ;
        return Destructeurs->size() ;
}
