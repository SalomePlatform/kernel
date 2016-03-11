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

#ifndef _RECEIVER_HXX_
#define _RECEIVER_HXX_

#include "SALOME_Comm_i.hxx"

/*! Abstract class factorizing common methods of all the receivers. All of the receivers have to inheritate from it.
 */
template<class T,class senderPtr, class senderSrv>
class Receiver
{
public:
  virtual T *getValue(long &size)=0;
  virtual ~Receiver() {}
protected:
  virtual T *getValue(long &size,senderPtr sender);
  static inline T *getLocalValue(long &size,senderSrv* servant);
  virtual T *getDistValue(long &size)=0;
};

template<class T,class senderPtr, class senderSrv>
T *Receiver<T,senderPtr,senderSrv>::getLocalValue(long &size,senderSrv* servant)
{
  const T *src=(const T *)servant->getData(size);
  long lgr=size*servant->getSizeOf();
  void *ret=new char[lgr];
  memcpy(ret,src,lgr);
  return (T *)ret;
  //return (void *)servant->getData(size);
}

template<class T,class senderPtr, class senderSrv>
T *Receiver<T,senderPtr,senderSrv>::getValue(long &size,senderPtr sender)
{
  senderSrv* data=senderSrv::find(sender);
  if(data)
    return getLocalValue(size,data);
  else
    return getDistValue(size);
}

#endif

