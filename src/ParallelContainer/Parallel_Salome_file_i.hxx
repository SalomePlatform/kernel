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

//  File   : Parallel_Salome_file_i.hxx
//  Author : André RIBES, EDF
//  Module : SALOME
//  $Header: 
//
#ifndef _PARALLEL_SALOME_FILE_I_HXX_
#define _PARALLEL_SALOME_FILE_I_HXX_

#include <SALOMEconfig.h>
#include <SALOME_Container.hxx>
#include <map>
#include <cstdio>

#include "SALOME_PACOExtensionPaCO_Engines_Parallel_Salome_file_server.hxx"
#include "Salome_file_i.hxx"

class CONTAINER_EXPORT Parallel_Salome_file_i:
  public virtual Salome_file_i,
  public virtual Engines::Parallel_Salome_file_serv
{
  public:
    Parallel_Salome_file_i(CORBA::ORB_ptr orb, 
                           const char * ior,
                           int rank);
    virtual ~Parallel_Salome_file_i();

    virtual void setFileNode(const char* file_name, CORBA::Long node);
    virtual CORBA::Long getFileNode(const char* file_name);
    virtual Engines::Container_ptr updateFile(const Engines::file& file);

    // New implementation for these methods
    // For the parallel cases
    virtual void load(const char* hdf5_file);
    virtual void save(const char* hdf5_file);
    virtual void save_all(const char* hdf5_file);
    virtual void connect(Engines::Salome_file_ptr source_Salome_file);
    virtual void connectDistributedFile(const char * file_name,
                                        Engines::Salome_file_ptr source_Salome_file);
    virtual void setDistributedSourceFile(const char* file_name,
                                          const char * source_file_name);
    virtual void recvFiles();
    virtual void recvFiles_node();
    virtual void setContainer(Engines::Container_ptr container);
    //virtual void removeFile(const char* file_name);
    //virtual void removeFiles();
    
    // Local C++ methods
    virtual bool getParallelDistributedFile(std::string file_name);

  private :
    Engines::Parallel_Salome_file_var proxy;
    Engines::PaCO_Parallel_Salome_file * parallel_file;
};

#endif

