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

//  SALOME NOTIFICATION_SWIG : wrapping of Notification sevices in order to be available in Python
//  File   : NOTIFICATION_Swig.cxx
//  Author : Francis KLOSS
//  Module : SALOME
//
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
