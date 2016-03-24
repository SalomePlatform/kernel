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

//  SALOME HDFPersist : implementation of HDF persitent ( save/ restore )
//  File   : test5.cxx
//  Module : SALOME
//
#include <iostream.h>
#include "HDFOI.hxx"
#include <stdlib.h>


int main()
{
  HDFfile *hdf_file;
  HDFgroup *hdf_group;

  try
  {
    // A file study.hdf with 2 groups : MESH and GEOM
    hdf_file = new HDFfile("study.hdf");

    hdf_file->CreateOnDisk();

    hdf_group = new HDFgroup("GEOM",hdf_file);

    hdf_group->CreateOnDisk();

    hdf_group->CloseOnDisk();

    delete hdf_group;

    hdf_group = new HDFgroup("MESH",hdf_file); 

    hdf_group->CreateOnDisk();

    hdf_group->CloseOnDisk();

    delete hdf_group;    

    hdf_file->CloseOnDisk();
    
    delete hdf_file;

    // a file mesh.hdf with 2 groups MESH_1 and MESH_2
    hdf_file = new HDFfile("mesh.hdf");

    hdf_file->CreateOnDisk();

    hdf_group = new HDFgroup("MESH_1",hdf_file);

    hdf_group->CreateOnDisk();

    hdf_group->CloseOnDisk();

    delete hdf_group;

    hdf_group = new HDFgroup("MESH_2",hdf_file);

    hdf_group->CreateOnDisk();

    hdf_group->CloseOnDisk();

    delete hdf_group;    

    hdf_file->CloseOnDisk();
    
    delete hdf_file;

    // a file geom.hdf with 2 groups GEOM_1 and GEOM_2
    hdf_file = new HDFfile("geom.hdf");

    hdf_file->CreateOnDisk();

    hdf_group = new HDFgroup("GEOM_1",hdf_file);

    hdf_group->CreateOnDisk();

    hdf_group->CloseOnDisk();

    delete hdf_group;

    hdf_group = new HDFgroup("GEOM_2",hdf_file);

    hdf_group->CreateOnDisk();

    hdf_group->CloseOnDisk();

    delete hdf_group;    

    hdf_file->CloseOnDisk();
    
    delete hdf_file;
  }
  catch (HDFexception)
    {
      MESSAGE( "!!!! HDFexception !!!" )
    }

  return 0;
}
