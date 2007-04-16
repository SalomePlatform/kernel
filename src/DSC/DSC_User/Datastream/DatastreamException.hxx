//  Copyright (C) 2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  File   : DatastreamException.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-02-06 17:20:28 +0100 (mar, 06 fév 2007) $
// Id          : $Id$

#ifndef DATASTREAM_EXCEPTION_HXX
#define DATASTREAM_EXCEPTION_HXX

#include "DSC_Exception.hxx"
#include "CalciumTypes.hxx"

struct DatastreamException  : public DSC_Exception {

  DatastreamException( const CalciumTypes::InfoType info,
		       const string &     text, 
		       const char        *fileName, 
		       const unsigned int lineNumber,
		       const char        *funcName):
  DSC_Exception(text,fileName,lineNumber,funcName),_info(info)
  {};


  DatastreamException( const CalciumTypes::InfoType info,
		       const string & text ):
    DSC_Exception(text),_info(info)
  {};

  DatastreamException(CalciumTypes::InfoType info, const DSC_Exception & ex ):
    DSC_Exception(ex),_info(info) {};
 
  virtual ~DatastreamException() throw() {};
  CalciumTypes::InfoType getInfo() const { return _info;}
  void setInfo(CalciumTypes::InfoType info) {_info=info;}

private:
  CalciumTypes::InfoType _info;
};


#endif /* DATASTREAM_EXCEPTION_HXX */
