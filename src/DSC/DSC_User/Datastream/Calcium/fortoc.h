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

#ifndef FORTRAN_H
#define FORTRAN_H

/* Operateur de concatenation */
#define  _(A,B)   A##B

#if defined __linux || defined  __APPLE__
#define F_FUNC(lname,uname) _(lname,_)        /* Fortran function name */
#define F_CALL(lname,uname) _(lname,_)        /* Fortran function call */
#define STR_PSTR(str)       char *str         /* fortran string arg pointer */
#define STR_PLEN(str)       , int _(Len,str)  /* fortran string arg length */
#define STR_PTR(str)        str               /* fortran string pointer */
#define STR_LEN(str)        _(Len,str)        /* fortran string length */


#endif

#endif

