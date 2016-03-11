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
//  File   : test8.cxx
//  Module : SALOME
//
#include <iostream.h>
#include "HDFOI.hxx"
#include <stdlib.h>


int main()
{
  HDFfile *hdf_file;
  HDFgroup *hdf_group;
  HDFdataset *hdf_dataset;
  hdf_size size[1];
  char message[HDF_NAME_MAX_LEN+1] = "MESSAGE";
  hdf_int32 attribute = 3;

  system("rm file_ascii.hdf");

  try
    {
      // a new HDFfile object
      hdf_file = new HDFfile("file_ascii.hdf");
      MESSAGE( ">> A HDFfile object is created" );
      
      hdf_file->CreateOnDisk();
      MESSAGE( ">> The HDF file is created on Disk " );

      // Inside the HDF file 

      // A new HDF group object
      hdf_group = new HDFgroup("ASCII GROUP",hdf_file);
      MESSAGE( ">> A HDFgroup object 'ASCII' is created" );

      hdf_group->CreateOnDisk();
      MESSAGE( ">> The HDF group is created on Disk " );

      // Transfert d'un fichier ASCII dans un groupe HDF
      HDFConvert::FromAscii("ascii.txt",*hdf_group,"ASCII DATASET");
      MESSAGE( ">> The Ascii file is transfered " );
 
      // The HDF group is built, it can be closed
      hdf_group->CloseOnDisk();
      MESSAGE( ">> The HDF group is closed on Disk " );

      // The HDF file is built, it can be closed
      hdf_file->CloseOnDisk();
      MESSAGE( ">> The HDF file is closed on disk" );

 
      delete hdf_group;
      MESSAGE( ">> The HDF group object is deleted" ); 
      delete hdf_file;
      MESSAGE( ">> The HDF file object is deleted" );
    }
  catch (HDFexception)
    {
      MESSAGE( "!!!! HDFexception" )
    }

  return 0;
}
