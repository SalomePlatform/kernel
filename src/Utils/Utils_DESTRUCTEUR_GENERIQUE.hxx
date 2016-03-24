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
//  File   : Utils_DESTRUCTEUR_GENERIQUE.hxx
//  Author : Antoine YESSAYAN, EDF
//  Module : SALOME
//  $Header$
//
# if !defined( __DESTRUCTEUR_GENERIQUE__H__ )
# define __DESTRUCTEUR_GENERIQUE__H__

#include <SALOMEconfig.h>

#include "SALOME_Utils.hxx"

#include <list>
#include <cassert>
#include <omniORB4/CORBA.h>
#include <iostream>
//# include "utilities.h"

/*!\class DESTRUCTEUR_GENERIQUE_
 *
 * <B>Definition</B>
 * 
 * The DESTRUCTEUR_GENERIQUE_ abstract class describes the comportement of any destruction object.
 * Tis type is used to create a list of miscellaneous destruction object.
 *
 * <B>Usage</B>
 * 
 * The only way to use the DESTRUCTEUR_GENERIQUE_ class is inheritance :
 *      class DESTRUCTEUR_SPECIFIQUE_ : public DESTRUCTEUR_GENERIQUE_
 * 
 * <B>Design description</B>
 * 
 *      A generic destructor supply two functionalities :
 *      -# a static method to add a destruction (objetct) to be performed DESTRUCTEUR_GENERIQUE_::Ajout(
 *      DESTRUCTEUR_GENERIQUE_ &objet) ;
 *         The Destruction object is stored in a list of pointer to DESTRUCTEUR_GENERIQUE_ objects.
 *      -# an object method to execute the destruction : operator()().
 */

class UTILS_EXPORT DESTRUCTEUR_GENERIQUE_
{
public :
  static std::list<DESTRUCTEUR_GENERIQUE_*> *Destructeurs;

  virtual ~DESTRUCTEUR_GENERIQUE_() {}//!< virtual destructor
  static const int Ajout( DESTRUCTEUR_GENERIQUE_ &objet );//!< adds a destruction object to the list of destructions
  virtual void operator()( void )=0 ;//!< performs the destruction
};


/*!\class DESTRUCTEUR_DE_
 *
 * <B>Purpose</B>
 * 
 * The DESTRUCTEUR_DE_ class allows the user to program - at any moment - the destruction of an object
 * at the end of the process.
 *
 * <B>Usage</B>
 * 
 *      In this example the POINT_ ptrPoint will be destroyed at the end of the process (atexit).
 *
 *      POINT_ *ptrPoint = new POINT_ ;<BR>
 *      DESTRUCTEUR_DE_<POINT_> *ptrDestruction = new DESTRUCTEUR_DE_<POINT_>( *ptrPoint ) ;
 * 
 *      Note that neither ptrPoint, nor ptrDestruction should be destroyed by the user.
 * 
 * <B>Design description</B>
 * 
 *      The destruction object must be created dynamicaly because it suscribes himself in the list of
 *      destruction to be performed at the end of the process.
 * 
 */
template <class TYPE> class DESTRUCTEUR_DE_ : public DESTRUCTEUR_GENERIQUE_
{
public :
  /* Programs the destruction at the end of the process, of the object objet.
     This method records in _PtrObjet the address of an object to be destroyed 
     at the end of the process
  */
  DESTRUCTEUR_DE_(TYPE &objet):
    _PtrObjet( &objet )
  {
    assert(DESTRUCTEUR_GENERIQUE_::Ajout( *this ) >= 0) ;
  }

  /* Performs the destruction of the object.
     This method really destroys the object pointed by _PtrObjet. 
     It should be called at the end of the process (i.e. at exit).
  */
  virtual void operator()(void){
    typedef PortableServer::ServantBase TServant;
    if(_PtrObjet){
      if(dynamic_cast<TServant*>(_PtrObjet)){
       // std::cerr << "WARNING: automatic destruction for servant is no more used. It's too late in exit. Use explicit call" << std::endl;
  /*
      if(TServant* aServant = dynamic_cast<TServant*>(_PtrObjet)){
        PortableServer::POA_var aPOA = aServant->_default_POA();
        PortableServer::ObjectId_var anObjectId = aPOA->servant_to_id(aServant);
        aPOA->deactivate_object(anObjectId.in());
        aServant->_remove_ref();
  */
      }else{
        //cerr << "DESTRUCTEUR_GENERIQUE_::operator() deleting _PtrObjet" << endl;
        TYPE* aPtr = static_cast<TYPE*>(_PtrObjet);
        delete aPtr;
      }
      _PtrObjet = NULL ;
    }
  } 

  virtual ~DESTRUCTEUR_DE_(){
    assert(!_PtrObjet) ;
  }

private :
  TYPE *_PtrObjet ;
};


# endif         /* # if !defined( __DESTRUCTEUR_GENERIQUE__H__ ) */
