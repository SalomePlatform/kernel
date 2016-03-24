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
//  File   : HDFdataset.hxx
//  Module : SALOME
//
#ifndef HDFDATASET_HXX
#define HDFDATASET_HXX

#include "HDFtypes.h"
#include "HDFinternalObject.hxx"
#include "HDFcontainerObject.hxx"
#include "HDFexport.hxx"

class HDFPERSIST_EXPORT HDFdataset : public HDFinternalObject
{
private :
  HDFcontainerObject *_father;
  hdf_idt _fid;
  hdf_type _type;
  hdf_size *_dim;
  hdf_byte_order _byte_order;
  hdf_size _size;
  int _ndim;
  char* _attribute;
  hdf_idt _arrayId;

public:
  HDFdataset(const char *name, HDFcontainerObject *father,hdf_type type, 
             hdf_size dim[],int dimsize, hdf_byte_order order = H5T_ORDER_NONE);

  HDFdataset(const char *name,HDFcontainerObject *father);
  virtual ~HDFdataset();

  void CreateOnDisk();
  void OpenOnDisk();
  void CloseOnDisk();

  void WriteOnDisk(void *values);
  void ReadFromDisk(void *values);

  HDFcontainerObject *GetFather();
  hdf_type GetType();
  int nDim();
  void GetDim(hdf_size dim[]);
  hdf_size GetSize();
  hdf_object_type GetObjectType();
  hdf_byte_order GetOrder();

  void SetArrayId(hdf_idt arrayId);

  int nAttributes();
  char* GetAttributeName(unsigned idx);
};

#endif
