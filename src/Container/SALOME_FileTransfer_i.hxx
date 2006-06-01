// Copyright (C) 2006  OPEN CASCADE, CEA/DEN, EDF R&D, PRINCIPIA R&D
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
//  File   : SALOME_FileTransfer_i.hxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$


#ifndef _SALOME_FILETRANSFER_I_HXX_
#define _SALOME_FILETRANSFER_I_HXX_

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Component)
#include <map>
#include <cstdio>

class fileTransfer_i:
  public virtual POA_Engines::fileTransfer,
  public virtual PortableServer::RefCountServantBase
{
public:
  fileTransfer_i();
  virtual ~fileTransfer_i();

  CORBA::Long open(const char* fileName);

  void close(CORBA::Long fileId);

  Engines::fileBlock* getBlock(CORBA::Long fileId);

protected:
  int _fileKey;
  std::map<int, FILE*> _fileAccess;
  int _ctr;
};

#endif
