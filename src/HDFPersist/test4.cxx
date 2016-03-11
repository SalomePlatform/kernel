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
//  File   : test4.cxx
//  Module : SALOME
//
#include <iostream.h>
#include "HDFOI.hxx"
#include <stdlib.h>


int main()
{
  HDFfile *hdf_file;
  HDFgroup *hdf_group;
  HDFdataset *hdf_dataset1, *hdf_dataset2;
  int n,i;
  char name[HDF_NAME_MAX_LEN+1];
  hdf_object_type type;
  hdf_type data_type;
  int ndim, size;
  hdf_size *dim;
  hdf_float64 *val1;
  hdf_int32 *val2;

  try  
    {
      // a new HDFfile object
      hdf_file = new HDFfile("file_test3.hdf");
      MESSAGE( ">> A HDFfile object is created" );
      
      hdf_file->OpenOnDisk(HDF_RDONLY);
      MESSAGE(">> The HDF file is opened on Disk with HDF_RDONLY access mode") 
      // It is possible to read the name of all (objects) sons in the file
      n = hdf_file->nInternalObjects(); 
      for (i=0;i<n;i++) 
        {
          hdf_file->InternalObjectIndentify(i,name);
          MESSAGE( "--> First Level Internal Object Name : " << name );
        }   

      // Or it is possible to ask if an object is in the file
      if (hdf_file->ExistInternalObject("MESH"))
          MESSAGE( "--> The object naming 'MESH' is in the file " );

      // it is possible to determine the type of an object
      type = hdf_file->InternalObjectType("MESH");
      switch (type)
        {
        case HDF_GROUP :
          MESSAGE( "--> Its type is HDF_GROUP" );
          break;
          
        case HDF_DATASET :
          MESSAGE( "--> Its type is HDF_DATASET" );
          break;
          
        default :
          MESSAGE( "--> !!!! PANIC !!!" );
        }

      hdf_group = new HDFgroup(name,hdf_file); 
      MESSAGE( ">> A new HDF group object " << name << " is created in memory")

      hdf_group->OpenOnDisk();
      MESSAGE( ">> The group " << name << " is opened on disk" );
 
      // object inside the group ?
      n = hdf_group->nInternalObjects();  
      MESSAGE( ">> There are " << n << " objects in this group " );
      for (i=0;i<n;i++)     
        {
          hdf_group->InternalObjectIndentify(i,name);
          type = hdf_group->InternalObjectType(name);
          MESSAGE( "--> First Level Internal Object Name : " << name );
          switch (type)
            {
            case HDF_GROUP :
              MESSAGE( "--> Its type is HDF_GROUP" );
              break;
              
            case HDF_DATASET :
              MESSAGE( "--> Its type is HDF_DATASET" );
              break;
              
            default :
              MESSAGE( "--> !!!! PANIC !!!" );
            }
        } 

      // The first dataset object 'COORDINATES'
      hdf_dataset1 = new HDFdataset("COORDINATES",hdf_group);
      MESSAGE( ">> The dataset object 'COORDINATES' is created in memory " );

      hdf_dataset1->OpenOnDisk();
      MESSAGE( ">> The dataset object 'COORDINATES' is opened on disk " );

      data_type = hdf_dataset1->GetType();
      switch(data_type)
        {
        case HDF_INT32 :
          MESSAGE( "--> Type of data : HDF_INT32 " );
          break; 
        case HDF_INT64 :
          MESSAGE( "--> Type of data : HDF_INT64 " );
          break; 
        case HDF_FLOAT64 :
          MESSAGE( "--> Type of data : HDF_FLOAT64 " );
          break;
        default :
          MESSAGE( "--> Type of data : HDF_STRING " );
        }
 
      ndim = hdf_dataset1->nDim();
      MESSAGE( "--> Number of dimensions : " << ndim );

      dim = new hdf_size[ndim];
      hdf_dataset1->GetDim(dim);
      for (i=0;i<ndim;i++)
        MESSAGE( "--> Dimension " << i+1 << " of size : " << dim[i] );
      delete dim;

      size = hdf_dataset1->GetSize();
      val1 = new hdf_float64[size];
      hdf_dataset1->ReadFromDisk(val1);
      MESSAGE( "--> The values are : ");
      for (i=0;i<size;i++)
        MESSAGE( " " << val1[i]);
      MESSAGE();
      delete val1;

      hdf_dataset1->CloseOnDisk();
      MESSAGE( ">> The dataset object 'COORDINATES' is closed on disk " );

      // The second dataset 'UNIT'
      hdf_dataset2 = new HDFdataset("UNIT",hdf_group);
      MESSAGE( ">> The dataset object 'UNIT' is created in memory " );

      hdf_dataset2->OpenOnDisk();
      MESSAGE( ">> The dataset object 'UNIT' is opened on disk " );

      data_type = hdf_dataset2->GetType();
      switch(data_type)
        {
        case HDF_INT32 :
          MESSAGE( "--> Type of data : HDF_INT32 " );
          break;
        case HDF_INT64 :
          MESSAGE( "--> Type of data : HDF_INT64 " );
          break;
        case HDF_FLOAT64 :
          MESSAGE( "--> Type of data : HDF_FLOAT64 " );
          break;
        default :
          MESSAGE( "--> Type of data : HDF_STRING " );
        }

      ndim = hdf_dataset2->nDim();
      MESSAGE( "--> Number of dimensions : " << ndim );

      dim = new hdf_size[ndim];
      hdf_dataset2->GetDim(dim);
      for (i=0;i<ndim;i++)
        MESSAGE( "--> Dimension " << i+1 << " of size : " << dim[i] );
      delete dim;

      size = hdf_dataset2->GetSize();
      val2 = new hdf_int32[size];
      hdf_dataset2->ReadFromDisk(val2);
      MESSAGE( "--> The values are : ");
      for (i=0;i<size;i++)
        MESSAGE( " " << val2[i]);
      MESSAGE();
      delete val2;

      hdf_dataset2->CloseOnDisk();
      MESSAGE( ">> The dataset object 'UNIT' is closed on disk " );
      
      hdf_group->CloseOnDisk();
      MESSAGE( ">> The group is closed on file" );
      
      hdf_file->CloseOnDisk();
      MESSAGE(">> The HDF file is closed on Disk" )      
      
      // Memory clean
      delete hdf_dataset1;
      delete hdf_dataset2;
      delete hdf_group;
      delete hdf_file;
      MESSAGE( ">> MEMORY CLEAN : all HDF objects have been deleted" );
    }
  catch (HDFexception)
    {
      MESSAGE( "!!! HDFexception !!! " )
    }  
  
  return 0;
}
