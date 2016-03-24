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

#ifndef __SALOME_MATRIX_I_HXX__
#define __SALOME_MATRIX_I_HXX__

#include <string>
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Comm)
#include "SALOMEMultiComm.hxx"

class SALOME_Matrix_i : public virtual POA_SALOME::Matrix
{
private:
  const double *_tabToSend;
  int _nbOfRow;
  int _nbOfColumn;
  bool _ownTabToSend;
  SALOMEMultiComm _type;
protected:
  ~SALOME_Matrix_i();
public:
  SALOME_Matrix_i(const SALOMEMultiComm& multiCommunicator,const double *tabToSend,int nbOfRow,int nbOfColumn,bool ownTabToSend=false);
  SALOME::SenderDouble_ptr getData();
  CORBA::Long getSizeOfColumn();
  void release();
};

#endif
