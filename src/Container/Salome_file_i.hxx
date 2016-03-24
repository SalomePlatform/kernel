// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

//  File   : Salome_file_i.hxx
//  Author : André RIBES, EDF
//  Module : SALOME
//  $Header: 
//
#ifndef _SALOME_FILE_I_HXX_
#define _SALOME_FILE_I_HXX_

#include <SALOMEconfig.h>

#include CORBA_SERVER_HEADER(SALOME_Component)
#include CORBA_SERVER_HEADER(SALOME_Exception)

#include "SALOME_Container.hxx"
#include "SALOME_FileTransfer_i.hxx"

#include <map>
#include <cstdio>
#include <string>

class CONTAINER_EXPORT Salome_file_i:
  public virtual POA_Engines::Salome_file,public virtual fileTransfer_i
{
  public:
    Salome_file_i();
    virtual ~Salome_file_i();

    // Import and export methods
    virtual void load(const char* hdf5_file);
    virtual void save(const char* hdf5_file);
    virtual void save_all(const char* hdf5_file);

    // Adding files
    virtual void setLocalFile(const char* comp_file_name);
    virtual void setDistributedFile(const char* comp_file_name);

    // Configure DistributedFile
    virtual void connect(Engines::Salome_file_ptr source_Salome_file);
    virtual void connectDistributedFile(const char * file_name,
                                        Engines::Salome_file_ptr source_Salome_file);
    virtual void setDistributedSourceFile(const char* file_name,
                                          const char * source_file_name);

    // Recv and check files
    virtual void recvFiles();

    // Removing or deleting files
    virtual void removeFile(const char* file_name);
    virtual void removeFiles();

    // Informations methods:
    virtual void setContainer(Engines::Container_ptr container);
    virtual Engines::files* getFilesInfos();
    virtual Engines::file* getFileInfos(const char* file_name);
    virtual Engines::SfState* getSalome_fileState();

    // ---------------- fileTransfert Methods -----------------------
    virtual CORBA::Long open(const char* file_name);
    virtual void close(CORBA::Long fileId);
    virtual Engines::fileBlock* getBlock(CORBA::Long fileId);

  protected:    
    // ---------------- local C++ methods ---------------------------
    virtual bool checkLocalFile(std::string file_name);
    virtual bool getDistributedFile(std::string file_name);

  protected:

    // Contains a relation between a file ID (int) with
    // a fd descriptor (FILE*) open on the file.
    typedef std::map<int, FILE*> _t_fileAccess;

    // Contains the informations of the files managed by the Salome_file.
    typedef std::map<std::string, Engines::file> _t_fileManaged;

    // Contains the CORBA reference for each distributed file managed.
    typedef std::map<std::string, Engines::Salome_file_var> _t_fileDistributedSource;

    int _fileId;
    long _path_max;
    _t_fileAccess _fileAccess;
    _t_fileManaged _fileManaged;
    _t_fileDistributedSource _fileDistributedSource;
    Engines::SfState _state;
    Engines::Container_ptr _container;
    Engines::Salome_file_var _default_source_Salome_file;
};

#endif
