using namespace std;
//  File      : NOTIFICATION_Supplier.cxx
//  Created   : 1 Avril 2002
//  Author    : Laurent DADA / Francis KLOSS
//  Project   : SALOME
//  Module    : Notification Supplier
//  Copyright : CEA / Open CASCADE

#include "NOTIFICATION.hxx"

long NOTIFICATION_Supplier::_stamp = 0;

NOTIFICATION_Supplier::NOTIFICATION_Supplier(const char* instanceName, bool notif):
    _sender(instanceName),
    _counter(0),
    proxy_consumer(0),
    _ok(false)
{
    if (notif) {
        CosNA_EventChannel_ptr channel = NOTIFICATION_channel();
        if (CORBA::is_nil(channel)) {
            MESSAGE("NOTIFICATION Error : failed to get channel for supplier");
        } else {
            CosNA_SupplierAdmin_var supplierAdmin = channel->default_supplier_admin();

            CosNA_ProxyID proxyId;
            CosNotifyChannelAdmin::ProxyConsumer_ptr consumer = supplierAdmin->obtain_notification_push_consumer(CosNA_STRUCTURED_EVENT, proxyId);
            if (CORBA::is_nil(consumer)) {
                MESSAGE("NOTIFICATION Error : failed to get proxy push consumer");
            } else {
                proxy_consumer = CosNA_StructuredProxyPushConsumer::_narrow(consumer);
                if ( CORBA::is_nil(proxy_consumer)) {
                    MESSAGE("NOTIFICATION Error : failed to _narrow proxy push consumer");
                } else {
                    try {
                        proxy_consumer->connect_structured_push_supplier(_this());
                        _ok = true;
                        MESSAGE("NOTIFICATION Info : successfully connection for push supplier notification");
                    } catch (CORBA::BAD_PARAM& ex) {
                        MESSAGE("NOTIFICATION Error : push supplier BAD_PARAM Exception while connecting");
                    } catch (CosEventChannelAdmin::AlreadyConnected& ex) {
                        MESSAGE("NOTIFICATION Error : push supplier already connected");
                    } catch (...) {
                        MESSAGE("NOTIFICATION Error : push supplier failed to connect");
                    }
                };
            };
        };
    };
}

NOTIFICATION_Supplier::~NOTIFICATION_Supplier() {
    if (_ok) {
        _ok = false;
        CosNA_StructuredProxyPushConsumer_var proxy = proxy_consumer;
        proxy_consumer = CosNA_StructuredProxyPushConsumer::_nil();

        try {
            if (!CORBA::is_nil(proxy)) {
                proxy->disconnect_structured_push_consumer();
            };
        } catch(...) {
            MESSAGE("NOTIFICATION Error : while disconnecting proxy push consumer");
        }
    };
}

void NOTIFICATION_Supplier::Send(const char* graph, const char* node, const char* type, const char* message) {
    if (_ok) {
        _stamp++;
        _counter++;
        CosN_StructuredEvent* event = new CosN_StructuredEvent;

        event->header.fixed_header.event_type.domain_name = CORBA::string_dup("SALOME");
        event->header.fixed_header.event_type.type_name   = CORBA::string_dup("ComponentMessage");

        event->header.variable_header.length(0);

        event->filterable_data.length(7);
        event->filterable_data[0].name    = CORBA::string_dup("SenderName");
        event->filterable_data[0].value <<= _sender;
        event->filterable_data[1].name    = CORBA::string_dup("DestinationGroup");
        event->filterable_data[1].value <<= graph;
        event->filterable_data[2].name    = CORBA::string_dup("EventType");
        event->filterable_data[2].value <<= type;
        event->filterable_data[3].name    = CORBA::string_dup("EventNumber");
        event->filterable_data[3].value <<= (CORBA::ULong)_counter;
        event->filterable_data[4].name    = CORBA::string_dup("SendingDate");
        event->filterable_data[4].value <<= (const char*)NOTIFICATION_date();
        event->filterable_data[5].name    = CORBA::string_dup("DepartGroup");
        event->filterable_data[5].value <<= node;
        event->filterable_data[6].name    = CORBA::string_dup("Stamp");
        event->filterable_data[6].value <<= (CORBA::ULong)_stamp;
        event->remainder_of_body        <<= message;

        try {
            proxy_consumer->push_structured_event(*event);
        } catch (...) {
            MESSAGE("NOTIFICATION Error : can't send a message with the component instance : " << _sender);
        }
    };
}

void NOTIFICATION_Supplier::disconnect_structured_push_supplier() {
}

void NOTIFICATION_Supplier::subscription_change(const CosN_EventTypeSeq& added, const CosN_EventTypeSeq& deled) {
}
