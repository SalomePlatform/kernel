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

//  File   : calcium.h
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
//
/* Outils d'Aide au Couplage de Code de Calcul : $Id$ */

#ifndef __CALCIUMF_H
#define __CALCIUMF_H

#include "CalciumFortranInt.h"

/*TODO CPCD... */

#define STAR *
#define LCH_LAST_PARAM ,int strsize


#define CALCIUM_ECR_INTERFACE_C_H(_name,_timeType,_calInt,_type,_typeName,_qual,lastarg) \
  extern _calInt cp_##_name (void * component, _calInt mode,            \
                       _timeType t, _calInt i,                          \
                       char * nomvar, _calInt nbelem,                   \
                       _type _qual * data                               \
                       lastarg ) ;                                      \


CALCIUM_ECR_INTERFACE_C_H(een_fort_,float ,cal_int,cal_int,integer,,);
CALCIUM_ECR_INTERFACE_C_H(elg_fort_,float ,cal_int,long   ,long2integer,,);
CALCIUM_ECR_INTERFACE_C_H(eln_fort_,float ,cal_int,long   ,long,,);
CALCIUM_ECR_INTERFACE_C_H(ein_fort_,float ,cal_int,int    ,int2integer,,);
CALCIUM_ECR_INTERFACE_C_H(ere_fort_,float ,cal_int,float ,float,,);
CALCIUM_ECR_INTERFACE_C_H(erd_fort_,float ,cal_int,float ,float2double,,);
CALCIUM_ECR_INTERFACE_C_H(edb_fort_,double,cal_int,double,double,,);
CALCIUM_ECR_INTERFACE_C_H(elo_fort_,float ,cal_int,int   ,bool,,);  /*int pour bool ou cal_int */
CALCIUM_ECR_INTERFACE_C_H(ecp_fort_,float ,cal_int,float ,cplx,,);
CALCIUM_ECR_INTERFACE_C_H(ech_fort_,float ,cal_int,char  ,str,STAR,LCH_LAST_PARAM );



#define CALCIUM_LECT_INTERFACE_C_H(_name,_timeType,_calInt,_type,_typeName,_qual,lastarg) \
  extern void cp_##_name##_free ( _type _qual * data);                  \
  extern _calInt cp_##_name (void * component, _calInt mode,            \
                             _timeType * ti, _timeType * tf, _calInt * i, \
                             char * nomvar, _calInt bufferLength,       \
                             _calInt * nRead, _type _qual * data        \
                             lastarg ) ;                                \
                                                                        \


CALCIUM_LECT_INTERFACE_C_H(len_fort_,float ,cal_int,cal_int ,integer,,);
CALCIUM_LECT_INTERFACE_C_H(llg_fort_,float ,cal_int,long    ,long2integer,,);
CALCIUM_LECT_INTERFACE_C_H(lln_fort_,float ,cal_int,long    ,long,,);
CALCIUM_LECT_INTERFACE_C_H(lin_fort_,float ,cal_int,int     ,int2integer,,);
CALCIUM_LECT_INTERFACE_C_H(lre_fort_,float ,cal_int,float   ,float,,);
CALCIUM_LECT_INTERFACE_C_H(lrd_fort_,float ,cal_int,float   ,float2double,,);
CALCIUM_LECT_INTERFACE_C_H(ldb_fort_,double,cal_int,double  ,double,,);
CALCIUM_LECT_INTERFACE_C_H(llo_fort_,float ,cal_int,int     ,bool,,);   /*int pour bool ou cal_int */
CALCIUM_LECT_INTERFACE_C_H(lcp_fort_,float ,cal_int,float   ,cplx,,);
CALCIUM_LECT_INTERFACE_C_H(lch_fort_,float ,cal_int,char    ,str,STAR, LCH_LAST_PARAM );



#endif
