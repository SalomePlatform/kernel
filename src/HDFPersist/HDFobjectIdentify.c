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

/*----------------------------------------------------------------------------
SALOME HDFPersist : implementation of HDF persitent ( save/ restore )
  File   : HDFobjectIdentify.c
  Module : SALOME
----------------------------------------------------------------------------*/

#include "hdfi.h"
#include <string.h>

/*
 * - Name: HDFobjectIdentify
 * - Description : find the name of the HDF object of rank "i" in the HDF
 *     group "path"
 * - Parameters :
 *     - fid     (IN)     : l'ID du fichier o
 *     - path  (IN)       : group access path
 *     - i (IN)           : access rank
 *     - name (OUT)       : expected name
 * - Result : 
 *     - if sucess : 0
 *     - if failure : -1
 */ 

hdf_err RankInfo(hdf_idt id, const char *name, void *data);

hdf_err HDFobjectIdentify(hdf_idt fid,const char *path,int i,char *name)
{
  int idx;

  if ((idx = H5Giterate(fid,path,&i,RankInfo,
                        name)) < 0)
    return -1;

  return 0;
}


hdf_err RankInfo(hdf_idt id, const char *name, void *data)
{
  if ( data )
    strcpy( (char*)data, name );
  else 
    return -1;

  return 1;
}
