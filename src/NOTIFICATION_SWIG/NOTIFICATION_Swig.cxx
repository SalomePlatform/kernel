using namespace std;
//  File      : NOTIFICATION_Swig.cxx
//  Created   : 10 Juin 2002
//  Author    : Francis KLOSS
//  Project   : SALOME
//  Module    : Notification Swig
//  Copyright : Open CASCADE

#include "NOTIFICATION_Swig.hxx"

// Swig notification supplier
// --------------------------

NOTIFICATION_Supplier_Swig::NOTIFICATION_Supplier_Swig(const char* instanceName, bool notif) {
    _supplier = new NOTIFICATION_Supplier(instanceName, notif);
}

NOTIFICATION_Supplier_Swig::~NOTIFICATION_Supplier_Swig() {
    delete _supplier;
}

void NOTIFICATION_Supplier_Swig::Send(const char* graph, const char* node, const char* type, const char* message) {
    _supplier->Send(graph, node, type, message);
}

// Swig notification consumer
// --------------------------

NOTIFICATION_Consumer_Swig::NOTIFICATION_Consumer_Swig() {
    _consumer = new NOTIFICATION_Consumer();
}

NOTIFICATION_Consumer_Swig::~NOTIFICATION_Consumer_Swig() {
    delete _consumer;
}

bool NOTIFICATION_Consumer_Swig::Receive(char** graph, char** node, char** type, char** message, char** sender, long* counter, char** date, long* stamp) {
    return(_consumer->Receive(graph, node, type, message, sender, counter, date, stamp));
}
