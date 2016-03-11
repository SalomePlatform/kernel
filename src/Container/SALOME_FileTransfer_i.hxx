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

//  File   : SALOME_FileTransfer_i.hxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$
//
#ifndef _SALOME_FILETRANSFER_I_HXX_
#define _SALOME_FILETRANSFER_I_HXX_

#include "SALOME_Container.hxx"

#include "SALOME_GenericObj_i.hh"

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Component)
#include <map>
#include <cstdio>

class CONTAINER_EXPORT fileTransfer_i:
  public virtual POA_Engines::fileTransfer,
  public virtual PortableServer::ServantBase,
  public virtual SALOME::GenericObj_i
{
public:
  fileTransfer_i();
  virtual ~fileTransfer_i();

  CORBA::Long open(const char* fileName);

  void close(CORBA::Long fileId);

  Engines::fileBlock* getBlock(CORBA::Long fileId);
  CORBA::Long openW(const char* fileName);
  void putBlock(CORBA::Long fileId, const Engines::fileBlock& block);

protected:
  int _fileKey;
  std::map<int, FILE* > _fileAccess;
  int _ctr;
};

#endif
