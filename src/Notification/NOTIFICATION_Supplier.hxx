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

//  SALOME Notification : wrapping of Notification service services
//  File   : NOTIFICATION_Supplier.hxx
//  Author : Laurent DADA / Francis KLOSS
//  Module : SALOME
//
#ifndef NOTIFICATION_Supplier_HXX
#define NOTIFICATION_Supplier_HXX

#include "SALOME_NOTIFICATION.hxx"

#ifdef WIN32
#pragma warning(disable:4275) // Disable warning interface non dll
#endif

class NOTIFICATION_EXPORT NOTIFICATION_Supplier: public POA_CosNotifyComm::StructuredPushSupplier {
  public:
    NOTIFICATION_Supplier(const char* instanceName, bool notif);
    virtual ~NOTIFICATION_Supplier();
    void Send(const char* graph, const char* node, const char* type, const char* message);

  private:
    void disconnect_structured_push_supplier();
    void subscription_change(const CosN_EventTypeSeq& added, const CosN_EventTypeSeq& deled);

    bool  _ok;
    const char* _sender;
    long  _counter;
    CosNA_StructuredProxyPushConsumer_var proxy_consumer;
    static long _stamp;
};

#endif
