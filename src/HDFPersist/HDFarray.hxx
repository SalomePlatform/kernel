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
//  File   : HDFarray.hxx
//  Module : SALOME
//
#ifndef HDFARRAY_HXX
#define HDFARRAY_HXX

#include "HDFobject.hxx"
#include "HDFexport.hxx"

class HDFPERSIST_EXPORT HDFarray : public HDFobject {
protected:
  HDFobject* _father;
  hdf_type   _datatype;
  hdf_size*  _dim;
  int        _ndim;
  
public :
  HDFarray( HDFobject* father, hdf_type dataType, int ndim, hdf_size *dim);
  
  HDFarray( HDFobject* father);
  
  virtual ~HDFarray();

  virtual hdf_type GetDataType();

  virtual hdf_object_type GetObjectType();

  void CreateOnDisk();
  void CloseOnDisk();

  int nDim();
  void GetDim(hdf_size dim[]);
};

#endif /* HDFARRAY_HXX */

