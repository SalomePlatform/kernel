//  File      : NOTIFICATION.hxx
//  Created   : 1 Avril 2002
//  Author    : Laurent DADA / Francis KLOSS
//  Project   : SALOME
//  Module    : Notification
//  Copyright : CEA / Open CASCADE

#ifndef NOTIFICATION_HXX
#define NOTIFICATION_HXX

#define NOTIF_WARNING "Warning"
#define NOTIF_STEP    "Step"
#define NOTIF_TRACE   "Trace"
#define NOTIF_VERBOSE "Verbose"

#define NOTIFICATION_ChannelName "EventChannel"

#include "utilities.h"

// kloss #include <omnithread.h>
#include "CosNotifyShorthands.h"

#include "NOTIFICATION_Supplier.hxx"
#include "NOTIFICATION_Consumer.hxx"

char* NOTIFICATION_date();
CosNA_EventChannel_ptr NOTIFICATION_channel();

#endif
