//  KERNEL Utils : common utils for KERNEL
//  Copyright (C) 2003  CEA
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
//  See http://www.salome-platform.org or email : webmaster.salome@opencascade.org
//
//
//
//  File   : Utils_CatchSignals.h
//  Author : Oksana Tchebanova
//  Module : KERNEL
//  $Header:

#ifndef _UTILS_CATCHSIGNALS_H_
#define _UTILS_CATCHSIGNALS_H_

#include <stdio.h>
#define SIG_MAX_NUM 11 

typedef void (* SIG_PFV) (int);

class Utils_CatchSignals {

 public:
  Utils_CatchSignals();
  void Destroy() ;
  ~Utils_CatchSignals()
    {
      Destroy();
    }

  static  void SetCallBack(const int theAddressHiPart,const int theAddressLowPart) ;
  static  void UnsetCallBack() ;
  void Activate();
  void Deactivate() ;
  bool ReserveMemory();
  void FreeReserved();

private: 

  SIG_PFV mySigStates[SIG_MAX_NUM];
  int  myFloatOpWord;
  bool myIsActivated;
};

#endif
