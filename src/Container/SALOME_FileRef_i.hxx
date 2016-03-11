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

//  File   : SALOME_FileRef_i.hxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$
//
#ifndef _SALOME_FILEREF_I_HXX_
#define _SALOME_FILEREF_I_HXX_

#include "SALOME_Container.hxx"

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Component)

#include <string>
#include <map>

class CONTAINER_EXPORT fileRef_i:
  public virtual POA_Engines::fileRef,
  public virtual PortableServer::ServantBase
{
public:
  fileRef_i();
  fileRef_i(Engines::Container_ptr container,
            const char* origFileName);
  virtual ~fileRef_i();

  char* origFileName();

  char* refMachine();

  Engines::Container_ptr getContainer();

  CORBA::Boolean addRef(const char* machine,
                        const char* fileName);

  char* getRef(const char* machine);

protected:
  Engines::Container_var _container;
  std::string _origFileName;
  std::string _machine;
  std::map<std::string, std::string> _copies;
};

#endif
