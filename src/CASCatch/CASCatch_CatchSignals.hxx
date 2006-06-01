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

#ifndef _CASCatch_CatchSignals_HeaderFile
#define _CASCatch_CatchSignals_HeaderFile

#include <Standard.hxx>

/*!
 * \class CASCatch_CatchSignals
 * \brief This class controls an exception handling
 *
 */ 
class CASCatch_CatchSignals  {

public:

 // Methods PUBLIC
 // 
Standard_EXPORT CASCatch_CatchSignals();
Standard_EXPORT void Destroy() ;
~CASCatch_CatchSignals() { Destroy(); }
Standard_EXPORT void Activate() ;
Standard_EXPORT void Deactivate() ;

private:

/*!\var mySigStates[7], private
 * \brief stores signals' handler functions
 */ 
Standard_Address mySigStates[7];

/*!\var myFloatOpWord
 * \brief stores a float operation word, private
 */ 
Standard_Integer myFloatOpWord;

/*!\var myIsActivated
 * \brief stores a flag whether a catcher is activated, private]
 */
Standard_Boolean myIsActivated;

};


#endif
