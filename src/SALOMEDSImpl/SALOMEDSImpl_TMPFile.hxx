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

#ifndef __SALOMEDSIMPL_TMPFILE_H__
#define __SALOMEDSIMPL_TMPFILE_H__

//Handle definition
#include <MMgt_TShared.hxx>
#include <Handle_MMgt_TShared.hxx>
#include <Standard_DefineHandle.hxx>


DEFINE_STANDARD_HANDLE( SALOMEDSImpl_TMPFile, MMgt_TShared );


struct SALOMEDSImpl_TMPFile : public MMgt_TShared
{
  typedef unsigned char TOctet;

  Standard_EXPORT virtual size_t Size() = 0;

  Standard_EXPORT virtual TOctet* Data();

  Standard_EXPORT virtual TOctet& Get(size_t) = 0;

  DEFINE_STANDARD_RTTI( SALOMEDSImpl_TMPFile );
};


#endif 
