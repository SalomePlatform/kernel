//=============================================================================
// File      : Utils_DESTRUCTEUR_GENERIQUE.hxx
// Created   : Mon Nov  5 17:01:47 CET 2001
// Author    : Antoine YESSAYAN, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

# if !defined( __DESTRUCTEUR_GENERIQUE__H__ )
# define __DESTRUCTEUR_GENERIQUE__H__

# include "utilities.h"

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
 * 	class DESTRUCTEUR_SPECIFIQUE_ : public DESTRUCTEUR_GENERIQUE_
 * 
 * <B>Design description</B>
 * 
 * 	A generic destructor supply two functionalities :
 * 	-# a static method to add a destruction (objetct) to be performed DESTRUCTEUR_GENERIQUE_::Ajout(
 * 	DESTRUCTEUR_GENERIQUE_ &objet) ;
 * 	   The Destruction object is stored in a list of pointer to DESTRUCTEUR_GENERIQUE_ objects.
 * 	-# an object method to execute the destruction : operator()().
 */

class DESTRUCTEUR_GENERIQUE_
{
public :
	virtual ~DESTRUCTEUR_GENERIQUE_() {}//!< virtual destructor
	static const int Ajout( DESTRUCTEUR_GENERIQUE_ &objet );//!< adds a destruction object to the list of destructions
	virtual void operator()( void )=0 ;//!< performs the destruction
} ;






/*!\class DESTRUCTEUR_DE_
 *
 * <B>Purpose</B>
 * 
 * The DESTRUCTEUR_DE_ class allows the user to program - at any moment - the destruction of an object
 * at the end of the process.
 *
 * <B>Usage</B>
 * 
 *	In this example the POINT_ ptrPoint will be destroyed at the end of the process (atexit).
 *
 *      POINT_ *ptrPoint = new POINT_ ;<BR>
 * 	DESTRUCTEUR_DE_<POINT_> *ptrDestruction = new DESTRUCTEUR_DE_<POINT_>( *ptrPoint ) ;
 * 
 *	Note that neither ptrPoint, nor ptrDestruction should be destroyed by the user.
 * 
 * <B>Design description</B>
 * 
 * 	The destruction object must be created dynamicaly because it suscribes himself in the list of
 * 	destruction to be performed at the end of the process.
 * 
 */

template <class TYPE> class DESTRUCTEUR_DE_ : public DESTRUCTEUR_GENERIQUE_
{
public :

	inline DESTRUCTEUR_DE_( TYPE &objet ); //! programs the destruction at the end of the process, of the object objet
	virtual void operator()( void ) ; //!< performs the destruction of the object
	virtual ~DESTRUCTEUR_DE_() ;
private :
	const TYPE *_PtrObjet ;
} ;






/*!
This method records in _PtrObjet the address of an object to be destroyed at the end of the process
*/
template <class TYPE> DESTRUCTEUR_DE_<TYPE>::DESTRUCTEUR_DE_( TYPE &objet ): _PtrObjet( &objet )
{
	int k = DESTRUCTEUR_GENERIQUE_::Ajout( *this ) ;	
	ASSERT(k>=0) ;
}

template <class TYPE> DESTRUCTEUR_DE_<TYPE>::~DESTRUCTEUR_DE_()
{
	ASSERT(_PtrObjet==NULL) ;
}

/*!
This method really destroys the object pointed by _PtrObjet. It should be called at the end of the process
(i.e. at exit).
*/
template <class TYPE> void DESTRUCTEUR_DE_<TYPE>::operator()( void )
{
	if ( _PtrObjet )
	{
         	  MESSAGE("deleting _PtrObjet") ;
		delete (TYPE*)_PtrObjet ;
		_PtrObjet = NULL ;
	}
}

# endif		/* # if !defined( __SINGLETON__H__ ) */
