//  File      : NOTIFICATION_Consumer.hxx
//  Created   : 1 Avril 2002
//  Author    : Laurent DADA / Francis KLOSS
//  Project   : SALOME
//  Module    : Notification Consumer
//  Copyright : CEA / Open CASCADE

#ifndef NOTIFICATION_Consumer_HXX
#define NOTIFICATION_Consumer_HXX

class NOTIFICATION_Consumer: public POA_CosNotifyComm::StructuredPullConsumer {
  public:
    NOTIFICATION_Consumer();
    virtual ~NOTIFICATION_Consumer();
    bool Receive(char** graph, char** node, char** type, char** message, char** sender, long* counter, char** date, long* stamp);

  private:
    void disconnect_structured_pull_consumer();
    void offer_change(const CosN_EventTypeSeq& added, const CosN_EventTypeSeq& deled);

    bool _ok;
    CosNA_StructuredProxyPullSupplier_var proxy_supplier;
};

#endif
