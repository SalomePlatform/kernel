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

//  File   : SALOME_Basics.hxx
//  Author : Roman NIKOLAEV
//  Module : SALOME
//  $Header$
//
#ifndef _SALOME_KERNEL_HELPERS_HXX_
#define _SALOME_KERNEL_HELPERS_HXX_


#ifdef WIN32
# if defined KERNELHELPERS_EXPORTS || defined SalomeKernelHelpers_EXPORTS
#  define KERNELHELPERS_EXPORT __declspec( dllexport )
# else
#  define KERNELHELPERS_EXPORT __declspec( dllimport )
# endif
#else
# define KERNELHELPERS_EXPORT
#endif

#endif //_SALOME_KERNEL_HELPERS_HXX_
