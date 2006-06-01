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
#ifndef _CASCATCH_FAILURE_HeaderFile
#define _CASCATCH_FAILURE_HeaderFile

#include <Standard_Transient.hxx>
#include <Standard_DefineHandle.hxx>
DEFINE_STANDARD_HANDLE( CASCatch_Failure, Standard_Transient )

#include <Standard.hxx>

/*!
 * \class CASCatch_Failure
 * \brief This class presents an exception to be thrown
 *
 */    
class CASCatch_Failure :  public Standard_Transient
{

public:

Standard_EXPORT CASCatch_Failure();
Standard_EXPORT CASCatch_Failure(const Standard_CString aString);
Standard_EXPORT void Reraise(const Standard_CString aMessage) ;
Standard_EXPORT Standard_CString GetError() const;
Standard_EXPORT static  Handle_CASCatch_Failure Caught() ;
Standard_EXPORT static  void Raise(const Standard_CString aMessage = "") ;
Standard_EXPORT virtual  void Throw() const;public:

public:

DEFINE_STANDARD_RTTI( CASCatch_Failure )  

private: 
/*!\var myMessage
 * \brief stores an exception message
 */   
Standard_CString myMessage;

};


#endif
