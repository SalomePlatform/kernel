// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
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
#ifndef  DF_DEF_HXX
#define  DF_DEF_HXX

#ifdef WIN32
# ifdef DF_EXPORTS
#  define DF_EXPORT __declspec( dllexport )
# else
#  define DF_EXPORT __declspec( dllimport )
# endif
#else
# define DF_EXPORT
#endif

#include <iostream.h> 
#include <stdio.h>
#include <boost/shared_ptr.hpp> 

template<class T> class df_shared_ptr: public boost::shared_ptr<T>
{
public:
  df_shared_ptr() {}
  
  template<class Y>
    explicit df_shared_ptr(Y * p)
      {
	boost::shared_ptr<T>::reset(p);
      }
  
  template<class Y>
    df_shared_ptr(df_shared_ptr<Y> const & r):
      boost::shared_ptr<T>(r,boost::detail::dynamic_cast_tag())
	{}
  
  template<class Y>
    df_shared_ptr & operator=(df_shared_ptr<Y> const & r)
      {
	df_shared_ptr<T>(r).swap(*this);
	return *this;
      }
  
  template<class Y> df_shared_ptr& operator()(Y * p) // Y must be complete
    {
      if(T* pt = dynamic_cast<T*>(p))
	boost::shared_ptr<T>::reset(pt);
      else
	boost::throw_exception(std::bad_cast());
      return *this;
    }
    
};

# if defined(WNT) && !defined(HAVE_NO_DLL)

#  ifndef Standard_EXPORT
#   define Standard_EXPORT __declspec( dllexport )
// For global variables :
#   define Standard_EXPORTEXTERN __declspec( dllexport ) extern
#   define Standard_EXPORTEXTERNC extern "C" __declspec( dllexport )
#  endif  /* Standard_EXPORT */

#  ifndef Standard_IMPORT
#   define Standard_IMPORT __declspec( dllimport ) extern
#   define Standard_IMPORTC extern "C" __declspec( dllimport )
#  endif  /* Standard_IMPORT */

# else  /* WNT */

#  ifndef Standard_EXPORT
#   define Standard_EXPORT
// For global variables :
#   define Standard_EXPORTEXTERN extern
#   define Standard_EXPORTEXTERNC extern "C"
#  endif  /* Standard_EXPORT */

#  ifndef Standard_IMPORT
#   define Standard_IMPORT extern
#   define Standard_IMPORTC extern "C"
#  endif  /* Standard_IMPORT */

# endif  /* WNT */

# ifndef __Standard_API
//#  ifdef WNT
#   if !defined(WNT) 
#    define __Standard_API Standard_EXPORT
#    define __Standard_APIEXTERN Standard_EXPORTEXTERN
#   else
#    define __Standard_API Standard_IMPORT
#    define __Standard_APIEXTERN Standard_IMPORT
#   endif  // __Standard_DLL
//#  else
//#   define __Standard_API
//#  endif  // WNT
# endif  // __Standard_API

#include <iostream>
class Standard_EXPORT DFexception
{
public :
  DFexception(const char *message) {
    std::cerr << message << std::endl;
  }
}; 


#endif
