//  SALOME Basics : general SALOME definitions and tools (C++ part - no CORBA)
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : SALOME_Basics.hxx
//  Author : Alexander A. BORODIN
//  Module : SALOME
//  $Header$

#ifndef _SALOME_BASICS_HXX_
#define _SALOME_BASICS_HXX_


#ifdef WIN32
# ifdef BASICS_EXPORTS
#  define BASICS_EXPORT __declspec( dllexport )
# else
#  define BASICS_EXPORT __declspec( dllimport )
# endif
#else
# define BASICS_EXPORT
#endif

#endif //_SALOME_BASICS_HXX_
