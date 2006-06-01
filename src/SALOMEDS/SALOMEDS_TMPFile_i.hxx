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

#ifndef __SALOMEDSIMPL_TMPFILE_I_H__
#define __SALOMEDSIMPL_TMPFILE_I_H__

// IDL headers
#include "SALOMEconfig.h"
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOME_Component)

#include "SALOMEDSImpl_TMPFile.hxx"


//-----------------------------------------------------------------------------
DEFINE_STANDARD_HANDLE( SALOMEDS_TMPFile_i, SALOMEDSImpl_TMPFile );

class SALOMEDS_TMPFile_i : public SALOMEDSImpl_TMPFile
{
public:
  SALOMEDS_TMPFile_i(SALOMEDS::TMPFile* theStream);

  virtual size_t Size();

  virtual TOctet& Get(size_t);

  DEFINE_STANDARD_RTTI( SALOMEDS_TMPFile_i );

protected:
  SALOMEDS::TMPFile_var myStream;
};


//-----------------------------------------------------------------------------
DEFINE_STANDARD_HANDLE( Engines_TMPFile_i, SALOMEDSImpl_TMPFile );

class Engines_TMPFile_i : public SALOMEDSImpl_TMPFile
{
public:
  Engines_TMPFile_i(Engines::TMPFile* theStream);

  virtual size_t Size();

  virtual TOctet& Get(size_t);

  DEFINE_STANDARD_RTTI( Engines_TMPFile_i );

protected:
  Engines::TMPFile_var myStream;
};


#endif
