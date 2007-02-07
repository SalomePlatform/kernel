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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : SalomeString.hxx
//  Module : SALOME

# ifndef SALOME_STRING
# define SALOME_STRING

#include "SALOME_Utils.hxx"

# include <string>
# include <sstream>

class UTILS_EXPORT SALOME_STRING : public string
{
private :
	ostringstream _s ;
public :
	operator const char*() const
	{
		return _s.str().c_str() ;
	}
	SALOME_STRING() : _s()
	{
	}
	template <class T> SALOME_STRING( const T &valeur ) : _s()
	{
		_s << valeur ;
	}
	template <class T> SALOME_STRING &operator<<( const T &valeur )
	{
		_s << valeur ;
		return *this ;
	}
} ;

// Exemple d'utilisation avec les exceptions SALOME
//      SALOME_STRING msgErr;
//      msgErr << "ESSAI::ESSAI() : This Object cannot be instanciated by now ! Try "<< 11 << "times just to see what happens ";
//      throw SALOME_EXCEPTION (LOCALIZED(msgErr)) ;

# endif
