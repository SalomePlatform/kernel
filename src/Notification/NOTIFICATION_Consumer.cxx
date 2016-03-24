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
//  File   : NOTIFICATION_Consumer.cxx
//  Author : Laurent DADA / Francis KLOSS
//  Module : SALOME
//
#include "NOTIFICATION.hxx"

NOTIFICATION_Consumer::NOTIFICATION_Consumer():
    proxy_supplier(0),
    _ok(false)
{
    CosNA_EventChannel_ptr channel = NOTIFICATION_channel();
    if (CORBA::is_nil(channel)) {
        MESSAGE("NOTIFICATION Error : failed to get channel for consumer");
    } else {
        CosNA_ConsumerAdmin_ptr consumerAdmin = channel->default_consumer_admin();

        CosNA_ProxyID proxyId;
        CosNotifyChannelAdmin::ProxySupplier_ptr supplier = consumerAdmin->obtain_notification_pull_supplier(CosNA_STRUCTURED_EVENT, proxyId);
        if (CORBA::is_nil(supplier)) {
            MESSAGE("NOTIFICATION Error : failed to get proxy pull supplier");
        } else {
            proxy_supplier = CosNA_StructuredProxyPullSupplier::_narrow(supplier);
            if (CORBA::is_nil(proxy_supplier)) {
                MESSAGE("NOTIFICATION Error : failed to _narrow proxy pull supplier");
            } else {
                try {
                    proxy_supplier->connect_structured_pull_consumer(_this());
                    _ok = true;
                    MESSAGE("NOTIFICATION Info : successfully connection for pull consumer notification");
                } catch (CORBA::BAD_PARAM&) {
                    MESSAGE("NOTIFICATION Error : pull consumer BAD_PARAM exception while connecting");
                } catch (CosEventChannelAdmin::AlreadyConnected&) {
                    MESSAGE("NOTIFICATION Error : pull consumer already connected");
                } catch (...) {
                    MESSAGE("NOTIFICATION Error : pull consumer failed to connect");
                }
            };
        };
    };
}

NOTIFICATION_Consumer::~NOTIFICATION_Consumer() {
    if (_ok) {
        _ok = false;
        CosNA_StructuredProxyPullSupplier_var proxy = proxy_supplier;
        proxy_supplier = CosNA_StructuredProxyPullSupplier::_nil();

        try {
            if (!CORBA::is_nil(proxy)) {
                proxy->disconnect_structured_pull_supplier();
            };
        } catch(...) {
            MESSAGE("NOTIFICATION Error : while disconnecting proxy pull supplier");
        }
    };
}

bool NOTIFICATION_Consumer::Receive(char** graph, char** node, char** type, char** message, char** sender, long* counter, char** date, long* stamp) {
    bool                  status = false;
    CORBA::Boolean        has_event;
    CosN_StructuredEvent* event;

    if (_ok) {
        try {
            event  = proxy_supplier->try_pull_structured_event(has_event);
            status = has_event;
        } catch (...) {
            MESSAGE("NOTIFICATION Error : while calling try_pull_structured_event");
        };

        if (status) {
            //omniORB 4.1.x requiries using only CORBA types instead C types
            const char* Asender;
            const char* Agraph;
            const char* Atype;
            CORBA::ULong      Acounter;
            CORBA::ULong      Astamp;
            const char* Adate;
            const char* Anode;
            const char* Amessage;           
            event->filterable_data[0].value >>= Asender;
            event->filterable_data[1].value >>= Agraph;
            event->filterable_data[2].value >>= Atype;
            event->filterable_data[3].value >>= Acounter;
            event->filterable_data[4].value >>= Adate;
            event->filterable_data[5].value >>= Anode;
            event->filterable_data[6].value >>= Astamp;
            event->remainder_of_body        >>= Amessage;
            *graph   = CORBA::string_dup(Agraph);
            *node    = CORBA::string_dup(Anode);
            *type    = CORBA::string_dup(Atype);
            *message = CORBA::string_dup(Amessage);
            *sender  = CORBA::string_dup(Asender);
            *counter = (long)Acounter;
            *date    = CORBA::string_dup(Adate);
            *stamp   = (long)Astamp;
        };
    };

    return(status);
}

void NOTIFICATION_Consumer::disconnect_structured_pull_consumer() {
}

void NOTIFICATION_Consumer::offer_change(const CosN_EventTypeSeq& added, const CosN_EventTypeSeq& deled) {
}
