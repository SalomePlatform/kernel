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

#include "SALOME_Matrix_i.hxx"
#include "SenderFactory.hxx"

SALOME_Matrix_i::SALOME_Matrix_i(const SALOMEMultiComm& multiCommunicator,const double *tabToSend,int nbOfRow,int nbOfColumn,bool ownTabToSend):_tabToSend(tabToSend),
                                                                                                                                                _nbOfRow(nbOfRow),
                                                                                                                                                _nbOfColumn(nbOfColumn),
                                                                                                                                                _ownTabToSend(ownTabToSend),
                                                                                                                                                _type(multiCommunicator)
{
}

SALOME_Matrix_i::~SALOME_Matrix_i()
{
  if(_ownTabToSend)
    delete [] _tabToSend;
}

SALOME::SenderDouble_ptr SALOME_Matrix_i::getData()
{
  return SenderFactory::buildSender(_type,_tabToSend,_nbOfRow*_nbOfColumn,false);
}

CORBA::Long SALOME_Matrix_i::getSizeOfColumn()
{
  return _nbOfColumn;
}

void SALOME_Matrix_i::release()
{
  PortableServer::ObjectId_var oid = _default_POA()->servant_to_id(this);
  _default_POA()->deactivate_object(oid);
  _remove_ref();
}
