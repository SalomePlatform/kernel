using namespace std;
//  File      : NOTIFICATION.cxx
//  Created   : 1 Avril 2002
//  Author    : Laurent DADA / Francis KLOSS
//  Project   : SALOME
//  Module    : Notification General
//  Copyright : CEA / Open CASCADE

#include "NOTIFICATION.hxx"

using namespace std;
#include "Utils_ORB_INIT.hxx"
#include "Utils_SINGLETON.hxx"

CosNA_EventChannel_ptr NOTIFICATION_channel() {
    ORB_INIT&      init = *SINGLETON_<ORB_INIT>::Instance(); ASSERT(SINGLETON_<ORB_INIT>::IsAlreadyExisting());
    CORBA::ORB_ptr orb  = init(0, 0);

    CosNA_EventChannel_ptr       channel = CosNA_EventChannel::_nil();
    CosNaming::NamingContext_var name_context;
    CosNaming::Name              name;
    CORBA::Object_var            name_service;

    try {
        name_service = orb->resolve_initial_references("NameService");
        name_context = CosNaming::NamingContext::_narrow(name_service);
        if (CORBA::is_nil(name_context)) {
            MESSAGE("NOTIFICATION Error : failed to obtain context for NameService");
            return(channel);
        };
    } catch(CORBA::ORB::InvalidName& ex) {
        MESSAGE("NOTIFICATION Error : service required is invalid [does not exist]");
        return(channel);
    } catch (CORBA::COMM_FAILURE& ex) {
        MESSAGE("NOTIFICATION Error : caught system exception COMM_FAILURE");
        return(channel);
    } catch (...) {
        MESSAGE("NOTIFICATION Error : caught exception while resolving the naming service");
        return(channel);
    }

    name.length(1);
    name[0].id   = CORBA::string_dup((const char*)NOTIFICATION_ChannelName);
    name[0].kind = CORBA::string_dup((const char*)NOTIFICATION_ChannelName);

    try {
        CORBA::Object_var channel_ref = name_context->resolve(name);
        channel = CosNA_EventChannel::_narrow(channel_ref);
        if (CORBA::is_nil(channel)) {
            MESSAGE("NOTIFICATION Error : failed to narrow object found in naming service");
        };
    } catch(CORBA::ORB::InvalidName& ex) {
        MESSAGE("NOTIFICATION Error : invalid name");
    } catch (CORBA::COMM_FAILURE& ex) {
        MESSAGE("NOTIFICATION Error : caught system exception COMM_FAILURE while resolving event channel name");
    } catch (...) {
        MESSAGE("NOTIFICATION Error : caught exception while resolving event channel name");
    }

    return(channel);
}

#include <time.h>
#include <stdio.h>

static char JourSemaine[7][4] = {"Sun", "Mon", "Tue", "Wed", "Thu" , "Fri", "Sat"};

static char Mois[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

static char NOTIFICATION_DATE[50];

char* NOTIFICATION_date() {
    time_t aTime;
    time(&aTime);
    struct tm* temps = localtime(&aTime);

    sprintf(NOTIFICATION_DATE, "%4d %3d %3s %2d %3s %02d:%02d:%02d", 1900+temps->tm_year, temps->tm_mon+1, Mois[temps->tm_mon], temps->tm_mday, JourSemaine[temps->tm_wday], temps->tm_hour, temps->tm_min, temps->tm_sec);

    return(NOTIFICATION_DATE);
}
