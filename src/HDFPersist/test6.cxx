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
//  File   : test6.cxx
//  Module : SALOME
//
#include <iostream.h>
#include "HDFOI.hxx"
#include <stdlib.h>

int main()
{
  HDFfile *study;
  HDFgroup *geom, *mesh;
  HDFgroup *geom1, *geom2, *mesh1, *mesh2;
  int n,m,i;
  char name[HDF_NAME_MAX_LEN+1];

  try
    {
      // the groups 'GEOM' and 'MESH' become mounted points for
      // the files 'geom.hdf' and 'mesh.hdf' in the 'study.hdf' file

      study = new HDFfile("study.hdf");
      MESSAGE( ">> The HDFfile object for 'study.hdf' is created" );

      study->OpenOnDisk(HDF_RDONLY);
      MESSAGE( "--> The file is opened" );

      geom = new HDFgroup("GEOM",study);
      MESSAGE( ">> The HDFgroup object for 'GEOM' is created" );

      geom->FileMount("geom.hdf",HDF_RDONLY);
      MESSAGE( ">> 'GEOM' becomes a mounted point for the file 'geom.hdf'" );

      geom->OpenOnDisk();
      MESSAGE( "--> The group is opened" );

      m = geom->nInternalObjects();
      MESSAGE( "--> There are " << m << " objects in the group" );
      
      for (i=0;i<m;i++) 
        {
          geom->InternalObjectIndentify(i,name);
          MESSAGE( "--> First Level Internal Object Name : " << name );
        }        

      geom1 = new HDFgroup("GEOM_1",geom);
      MESSAGE( ">> The HDF group object is created for 'GEOM_1' with 'GEOM' as father" );
      MESSAGE( "--> This group is in fact in the file 'geom.hdf'");

      geom1->OpenOnDisk();
      MESSAGE( "-> The group 'GEOM_1' is opened" );

      geom1->CloseOnDisk();
      MESSAGE( "-> The group 'GEOM_1' is closed" );

      geom2 = new HDFgroup("GEOM_2",geom);
      MESSAGE( ">> The HDF group object is created for 'GEOM_2' with 'GEOM' as father" );
      MESSAGE( "--> This group is in fact in the file 'geom.hdf'");      

      geom2->OpenOnDisk();
      MESSAGE( "-> The group 'GEOM_2' is opened" );

      geom2->CloseOnDisk();
      MESSAGE( "-> The group 'GEOM_2' is closed" );

      geom->CloseOnDisk();
      MESSAGE( "-> The group 'GEOM' is closed" );

      geom->FileUnMount();
      MESSAGE( ">> The file 'geom.hdf' is unmounted from 'GEOM' " ); 

      mesh = new HDFgroup("MESH",study);

      mesh->FileMount("mesh.hdf",HDF_RDONLY);

      mesh->OpenOnDisk();

      mesh1 = new HDFgroup("MESH_1",mesh);

      mesh1->OpenOnDisk();

      mesh1->CloseOnDisk();

      mesh2 = new HDFgroup("MESH_2",mesh);

      mesh2->OpenOnDisk();

      mesh2->CloseOnDisk();

      mesh->CloseOnDisk();

      mesh->FileUnMount();

      MESSAGE( ">> The same operations have been done for the file 'mesh.hdf'" )
      MESSAGE( "   and the groups MESH_1 and MESH_2" )
      MESSAGE( "--> This file has been mounted on the group 'MESH' of the file 'study.hdf' ")
              
      study->CloseOnDisk();
      MESSAGE( ">> The study.hdf file is closed" );

      MESSAGE( ">> Memory clean" );
      delete mesh1;
      delete mesh2;
      delete mesh;
      delete geom1;
      delete geom2;
      delete geom;
      delete study;
    }
  catch(HDFexception)
    {
      MESSAGE( "!!! HDFexception !!! ")
    }
  return 0;
}
