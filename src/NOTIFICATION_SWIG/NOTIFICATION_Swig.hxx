//  File      : NOTIFICATION_Swig.hxx
//  Created   : 10 Juin 2002
//  Author    : Francis KLOSS
//  Project   : SALOME
//  Module    : Notification Swig
//  Copyright : Open CASCADE
using namespace std;

#include "NOTIFICATION.hxx"

class NOTIFICATION_Supplier_Swig {
  public:
    NOTIFICATION_Supplier_Swig(const char* instanceName, bool notif);
    ~NOTIFICATION_Supplier_Swig();
    void Send(const char* graph, const char* node, const char* type, const char* message);

  private:
    NOTIFICATION_Supplier* _supplier;
};

class NOTIFICATION_Consumer_Swig {
  public:
    NOTIFICATION_Consumer_Swig();
    ~NOTIFICATION_Consumer_Swig();
    bool Receive(char** graph, char** node, char** type, char** message, char** sender, long* counter, char** date, long* stamp);

  private:
    NOTIFICATION_Consumer* _consumer;
};
