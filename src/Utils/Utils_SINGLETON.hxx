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
//  File   : Utils_SINGLETON.hxx
//  Author : Antoine YESSAYAN, EDF
//  Module : SALOME
//  $Header$
//
# if !defined( __SINGLETON__H__ )
# define __SINGLETON__H__

#include "SALOME_Utils.hxx"

# include "Utils_DESTRUCTEUR_GENERIQUE.hxx"
# include <list>

/*!\class SINGLETON_
 *
 * <B>Definition</B>
 * 
 * A singleton is a data which is created and deleted only once in the application.
 * The C++ compiler allow the user to create static data before the first executable statement.
 * They are deleted after the last statement.statement.
 *
 * The SINGLETON_ template class deals with dynamic singleton. It is useful for functor objects.
 * For example, an object which, when created, connects the application to a system and
 * disconnects the application at deletion.
 *
 *
 * <B>Usage</B>
 * 
 * To create a single instance a POINT_ object :
 * 
 * # include "Utils_SINGLETON.hxx"
 *      ...
 *      ptrPoint = SINGLETON_<POINT_>::Instance() ;
 * 
 * 
 * <B>Design description</B>
 *
 *      -# the user creates an object of class TYPE By using a class method : SINGLETON_<TYPE>::Instance() which
 *         returns a pointer to the single object ;
 *      -# this class method uses the default constructor to create an object ;
 *      -# at the same time, this class method reate a destructor object which is added to the generic list
 *         of destructors objects to be executed at the end of the application (atexit) ;
 *      -# at the end of the application process all the deletions are performed by the Nettoyage() C function
 *         which execute the destructions objects then deletes the destructions objects themselves ;
 *      -# the Nettoyage() C function is recorded using atexit() C function through the creation of a static
 *         single object ATEXIT_().
 */


template <class TYPE> class SINGLETON_
{

public :

        static TYPE *Instance( void );          //!< Singleton dynamic creation using the default builder
        static bool IsAlreadyExisting( void );  //!< returns True if the singleton is already existing
        static int Destruction( void );         //!< destroys the Singleton before the end of the application process

private :

        TYPE _Instance ;
        static SINGLETON_ *PtrSingleton ;

        SINGLETON_( void );
        ~SINGLETON_();

} ;     /* class SINGLETON_<TYPE> */




template <class TYPE> SINGLETON_<TYPE> *SINGLETON_<TYPE>::PtrSingleton=NULL ;



/*!
 * The class method Instance :
 *  -# creates an object of class TYPE ;
 *  -# creates a destruction object DESTRUCTEUR_DE_<TYPE> which is appended to the list of destruction objects to be
 *     executed ;
 *  -# returns a pointer to the created object.
 *
 *  Note that the two created objects are deleted at the end of the process in the function Nettoyage().
 */
template <class TYPE> TYPE *SINGLETON_<TYPE>::Instance( void )
{
        if ( ! PtrSingleton )
        {
                //MESSAGE("SINGLETON_<TYPE>::Instance( void )") ;
                PtrSingleton = new SINGLETON_<TYPE> ;
                new DESTRUCTEUR_DE_<TYPE>( PtrSingleton->_Instance ) ;
        }
        return &PtrSingleton->_Instance ;
}


template <class TYPE> bool SINGLETON_<TYPE>::IsAlreadyExisting( void )
{
        return PtrSingleton ? true : false ;
}




template <class TYPE> SINGLETON_<TYPE>::SINGLETON_( void )
{
        //MESSAGE("CREATION d'un SINGLETON_") ;
}




/*!
        The method SINGLETON_<TYPE>::Destruction can be called by the user. If it is not
        the function nettoyage() calls it atexit.

        N.B. : the singleton objects are destroyed in the reverse order of there creation.
*/
template <class TYPE> int SINGLETON_<TYPE>::Destruction( void )
{
        int k = - 1 ;
        //BEGIN_OF("SINGLETON_<TYPE>::Destruction( void )") ;
        if ( PtrSingleton )
        {
          //MESSAGE("Destruction du SINGLETON_") ;


                std::list<DESTRUCTEUR_GENERIQUE_ *>::iterator k ;
                for( k=DESTRUCTEUR_GENERIQUE_::Destructeurs->begin() ; k!=DESTRUCTEUR_GENERIQUE_::Destructeurs->end();k++)
                {
                        if ( *k == PtrSingleton->_Instance )
                        {
                                DESTRUCTEUR_GENERIQUE_::Destructeurs->erase( k ) ;
                                break ;
                        }
                }
                delete PtrSingleton ;
                PtrSingleton = NULL ;
        }
        //END_OF("SINGLETON_<TYPE>::Destruction( void )") ;
        return k ;
}


template <class TYPE> SINGLETON_<TYPE>::~SINGLETON_()
{
  //MESSAGE("passage dans SINGLETON_<TYPE>::~SINGLETON_( void )") ;
}

# endif         /* # if !defined( __SINGLETON__H__ ) */
