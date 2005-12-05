// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/
//
#include "Receiver.hxx"
#include <string.h>
using namespace std;

/*!
  return a deep copy of the array contained in the servant.
 */
void *Receiver::getLocalValue(long &size,SALOME_Sender_i* servant)
{
  const void *src=servant->getData(size);
  long lgr=size*servant->getSizeOf();
  void *ret=new char[lgr];
  memcpy(ret,src,lgr);
  return ret;
  //return (void *)servant->getData(size);
}

void *Receiver::getValue(long &size,SALOME::Sender_ptr sender)
{
  SALOME_Sender_i* data=SALOME_Sender_i::find(sender);
  if(data)
    return getLocalValue(size,data);
  else
    return getDistValue(size);
}

