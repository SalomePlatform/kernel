using namespace std;
//  File      : NOTIFICATION_Consumer.cxx
//  Created   : 1 Avril 2002
//  Author    : Laurent DADA / Francis KLOSS
//  Project   : SALOME
//  Module    : Notification Consumer
//  Copyright : CEA / Open CASCADE

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
                } catch (CORBA::BAD_PARAM& ex) {
                    MESSAGE("NOTIFICATION Error : pull consumer BAD_PARAM exception while connecting");
                } catch (CosEventChannelAdmin::AlreadyConnected& ex) {
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
            char*        Asender;
            char*        Agraph;
            char*        Atype;
            CORBA::ULong Acounter;
            CORBA::ULong Astamp;
            char*        Adate;
            char*        Anode;
            char*        Amessage;
            event->filterable_data[0].value >>= Asender;
            event->filterable_data[1].value >>= Agraph;
            event->filterable_data[2].value >>= Atype;
            event->filterable_data[3].value >>= Acounter;
            event->filterable_data[4].value >>= Adate;
            event->filterable_data[5].value >>= Anode;
            event->filterable_data[6].value >>= Astamp;
            event->remainder_of_body        >>= Amessage;
            *graph   = strdup(Agraph);
            *node    = strdup(Anode);
            *type    = strdup(Atype);
            *message = strdup(Amessage);
            *sender  = strdup(Asender);
            *counter = (long)Acounter;
            *date    = strdup(Adate);
            *stamp   = (long)Astamp;
        };
    };

    return(status);
}

void NOTIFICATION_Consumer::disconnect_structured_pull_consumer() {
}

void NOTIFICATION_Consumer::offer_change(const CosN_EventTypeSeq& added, const CosN_EventTypeSeq& deled) {
}
