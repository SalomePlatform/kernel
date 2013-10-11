// Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
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

//  SALOME DataContainer : implementation of data container
//  File   : SALOME_DataContainer_i.hxx
//  Author : Mikhail PONIKAROV
//  Module : SALOME
//  $Header$
//
#ifndef _SALOME_DATACONTAINER_I_HXX_
#define _SALOME_DATACONTAINER_I_HXX_

#include "SALOME_Container.hxx"

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Component)

#include <string>

class CONTAINER_EXPORT Engines_DataContainer_i:
  public POA_Engines::DataContainer
{
public:
  Engines_DataContainer_i();
  Engines_DataContainer_i(const char* url,
                          const char* name,
                          const char* identifier,
                          const bool removeAfterGet);

  Engines_DataContainer_i(char* stream,
                          const int   streamSize,
                          const char* name,
                          const char* identifier,
                          const bool removeAfterGet);
  virtual ~Engines_DataContainer_i();

  // --- CORBA methods
  virtual Engines::TMPFile* get();
  virtual char* name();
  virtual char* identifier();
  virtual char* extension();
  virtual void  setExtension(const char* theExt);
            
protected:

  std::string myExt;        ///< extension (type) of the file
  std::string myName;       ///< name of the document corresponding to this data
  std::string myIdentifier; ///< module identifier of the document corresponding to this data
  std::string myURL;        ///< path to the locally located file
  bool        myRemoveAfterGet; ///< if this flag is true, file must be removed after the first "get" method call
  char*       myStream;     ///< if it is not NULL, data must be get from this stream, not from the file
  int         myStreamSize; ///< size (in bytes) if the stream in myStream
};

#endif
