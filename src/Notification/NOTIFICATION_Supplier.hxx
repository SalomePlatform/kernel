//  File      : NOTIFICATION_Supplier.hxx
//  Created   : 1 Avril 2002
//  Author    : Laurent DADA / Francis KLOSS
//  Project   : SALOME
//  Module    : Notification Supplier
//  Copyright : CEA / Open CASCADE

#ifndef NOTIFICATION_Supplier_HXX
#define NOTIFICATION_Supplier_HXX

class NOTIFICATION_Supplier: public POA_CosNotifyComm::StructuredPushSupplier {
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
