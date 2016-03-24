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

/*  
File   : CalciumInterface.hxx
Author : Eric Fayolle (EDF)
Module : KERNEL
Modified by : $LastChangedBy$
Date        : $LastChangedDate: 2007-03-01 13:27:58 +0100 (jeu, 01 mar 2007) $
Id          : $Id$
*/

#ifndef _CALCIUM_MACRO_C_INTERFACE_H_
#define _CALCIUM_MACRO_C_INTERFACE_H_



/****** CALCIUM_C2CPP_INTERFACE_HXX_ :                                  ******/
/****** Declarations: ecp_lecture_... , ecp_ecriture_..., ecp_free_... ******/

#define CALCIUM_C2CPP_INTERFACE_HXX_(_name,_porttype,_type,_qual)                                                 \
  extern "C" CalciumTypes::InfoType ecp_lecture_##_name (void * component, int dependencyType,                    \
                                                         CalTimeType< _type _qual >::TimeType * ti,               \
                                                         CalTimeType< _type _qual >::TimeType * tf, long * i,     \
                                                         const char * const nomvar, size_t bufferLength,          \
                                                         size_t * nRead, _type _qual ** data );                   \
                                                                                                                  \
                                                                                                                  \
  extern "C" void ecp_lecture_##_name##_free ( _type _qual * data);                                               \
                                                                                                                  \
                                                                                                                  \
  extern "C" CalciumTypes::InfoType ecp_ecriture_##_name (void * component, int dependencyType,                   \
                                                          CalTimeType< _type _qual >::TimeType *t,                \
                                                          long  i,                                                \
                                                          const char * const nomvar, size_t bufferLength,         \
                                                          _type _qual * data );                                   \
  



                                                                        
/****** CALCIUM_C2CPP_INTERFACE_CXX_ :                                ******/                                                                        
/******Definitions: ecp_lecture_... , ecp_ecriture_..., ecp_free_... ******/
#define CALCIUM_C2CPP_INTERFACE_CXX_(_name,_porttype,_type,_qual)                                                \
  extern "C" CalciumTypes::InfoType ecp_lecture_##_name (void * component, int dependencyType,                   \
                                                         CalTimeType< _type _qual >::TimeType * ti,              \
                                                         CalTimeType< _type _qual >::TimeType * tf, long * i,    \
                                                         const char * const nomvar, size_t bufferLength,         \
                                                         size_t * nRead, _type _qual ** data )                   \
  {                                                                                                              \
    Superv_Component_i * _component = static_cast<Superv_Component_i *>(component);                              \
    double         _ti=0.;                                                                                       \
    double         _tf=0.;                                                                                       \
    if(dependencyType == CalciumTypes::CP_TEMPS)                                                                               \
      {                                                                                                          \
        _ti=*ti;                                                                                                 \
        _tf=*tf;                                                                                                 \
      }                                                                                                          \
    size_t         _nRead=0;                                                                                     \
    size_t         _bufferLength=bufferLength;                                                                   \
                                                                                                                 \
    if ( IsSameType< _porttype , cplx >::value ) _bufferLength*=2;                                               \
    DEBTRACE( "-------- CalciumInterface(lecture Inter Part) MARK 1 ------------------" )                        \
    try                                                                                                          \
      {                                                                                                          \
        CalciumInterface::ecp_lecture< _type,_porttype >( *_component, dependencyType, _ti, _tf, *i, nomvar,     \
                                                          _bufferLength, _nRead, *data);                         \
      }                                                                                                          \
    catch ( const CalciumException & ex)                                                                         \
      {                                                                                                          \
        DEBTRACE( ex.what() );                                                                                   \
        return ex.getInfo();                                                                                     \
      }                                                                                                          \
    catch ( ... )                                                                                                \
      {                                                                                                          \
        DEBTRACE( "Unexpected exception ") ;                                                                     \
        return CalciumTypes::CPATAL;                                                                             \
      }                                                                                                          \
    if ( IsSameType< _porttype , cplx >::value )                                                                 \
      {                                                                                                          \
        *nRead=_nRead/2;                                                                                         \
        DEBTRACE( "-------- CalciumInterface(lecture Inter Part) IsSameType cplx -------------" )                \
        DEBTRACE( "-------- CalciumInterface(lecture Inter Part) _nRead  : " << _nRead )                         \
        DEBTRACE( "-------- CalciumInterface(lecture Inter Part) *nRead  : " << *nRead )                         \
      }                                                                                                          \
    else                                                                                                         \
      *nRead = _nRead;                                                                                           \
    if (dependencyType == CalciumTypes::CP_SEQUENTIEL )                                                          \
      *ti=(CalTimeType< _type _qual >::TimeType)(_ti);                                                           \
    DEBTRACE( "-------- CalciumInterface(lecture Inter Part), Data Ptr :" << *data )                             \
    return CalciumTypes::CPOK;                                                                                   \
  };                                                                                                             \
                                                                                                                 \
                                                                                                                 \
  extern "C" void ecp_lecture_##_name##_free ( _type _qual * data)                                               \
  {                                                                                                              \
    CalciumInterface::ecp_free< _type, _porttype >(data);                                                        \
  };                                                                                                             \
                                                                                                                 \
                                                                                                                 \
  extern "C" CalciumTypes::InfoType ecp_ecriture_##_name (void * component, int dependencyType,           \
                                                          CalTimeType< _type _qual >::TimeType *t,        \
                                                          long  i,                                        \
                                                          const char * const nomvar, size_t bufferLength, \
                                                          _type _qual * data )                            \
  {                                                                                                       \
    DEBTRACE( "-------- CalciumInterface(ecriture Inter Part) MARK 0 ------------------" )                \
    Superv_Component_i * _component = static_cast<Superv_Component_i *>(component);                       \
    /* Je ne sais pas pourquoi, je n'arrive pas à passer t par valeur : corruption de la pile*/           \
    double         _t=0.;                                                                                 \
    if(dependencyType == CalciumTypes::CP_TEMPS)                                                                        \
      _t=*t;                                                                                              \
    size_t         _bufferLength=bufferLength;                                                            \
    if ( IsSameType< _porttype , cplx >::value ) _bufferLength=_bufferLength*2;                           \
    DEBTRACE( "-------- CalciumInterface(ecriture Inter Part) MARK 1 ------------------" )                \
    try                                                                                                   \
      {                                                                                                   \
        /*printf("-------- CalciumInterface(ecriture Inter Part), cp_name : Nom de la var. de type %s : %s\n",#_type,nomvar);*/ \
        DEBTRACE( "----------->-" << nomvar )                                                             \
        CalciumInterface::ecp_ecriture< _type, _porttype >( *_component, dependencyType,                  \
                                                            _t,i,nomvar,_bufferLength,*data);             \
      }                                                                                                   \
    catch ( const CalciumException & ex)                                                                  \
      {                                                                                                   \
        DEBTRACE( ex.what() );                                                                            \
        return ex.getInfo();                                                                              \
      }                                                                                                   \
    catch ( ... )                                                                                         \
      {                                                                                                   \
        DEBTRACE("Unexpected exception " );                                                               \
        return CalciumTypes::CPATAL;                                                                      \
      }                                                                                                   \
    DEBTRACE( "-------- CalciumInterface(ecriture Inter Part), Valeur de data :" << data )                \
    return CalciumTypes::CPOK;                                                                            \
  };                                                                                                      \


#endif
