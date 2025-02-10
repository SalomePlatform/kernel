// Copyright (C) 2007-2025  CEA, EDF, OPEN CASCADE
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

/*  
File   : CalciumInterface.hxx
Author : Eric Fayolle (EDF)
Module : KERNEL
Modified by : $LastChangedBy$
Date        : $LastChangedDate: 2007-03-01 13:27:58 +0100 (jeu, 01 mar 2007) $
Id          : $Id$
*/

#define _CALCIUM_MACRO_C_INTERFACE_H_

/* template <> struct CalTimeType<float> {
  typedef float TimeType;
};
template <> struct CalTimeType<double> {
  typedef double TimeType;
}; */

/****** CALCIUM_C2CPP_INTERFACE_HXX_ :                                  ******/
/****** Declarations: ecp_lecture_... , ecp_ecriture_..., ecp_free_... ******/

#define CALCIUM_C2CPP_INTERFACE_C_(_name,_porttype,_type,_timeType,_qual)                                                 \
  InfoType ecp_lecture_##_name (void * component, int dependencyType,                    \
                                                         _timeType * ti,               \
                                                         _timeType * tf, long * i,     \
                                                         const char * const nomvar, size_t bufferLength,          \
                                                         size_t * nRead, _type _qual ** data );                   \
                                                                                                                  \
                                                                                                                  \
  void ecp_lecture_##_name##_free ( _type _qual * data);                                               \
                                                                                                                  \
                                                                                                                  \
  InfoType ecp_ecriture_##_name (void * component, int dependencyType,                   \
                                                          _timeType *t,                \
                                                          long  i,                                                \
                                                          const char * const nomvar, size_t bufferLength,         \
                                                          _type _qual * data );                                   \
  


                                                                        
/****** CALCIUM_C2CPP_INTERFACE_CXX_ :                                ******/                                                                        
/******Definitions: ecp_lecture_... , ecp_ecriture_..., ecp_free_... ******/

