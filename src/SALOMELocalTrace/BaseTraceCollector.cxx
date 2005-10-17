//  Copyright (C) 2004  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : BaseTraceCollector.cxx
//  Author : Paul RASCLE (EDF)
//  Module : KERNEL
//  $Header$

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>

using namespace std;

#include "BaseTraceCollector.hxx"
#include "LocalTraceBufferPool.hxx"

// Class attributes initialisation, for class method BaseTraceCollector::run

BaseTraceCollector* BaseTraceCollector::_singleton = 0;
pthread_mutex_t BaseTraceCollector::_singletonMutex;
sem_t BaseTraceCollector::_sem;
int BaseTraceCollector::_threadToClose = 0;
pthread_t* BaseTraceCollector::_threadId = 0; // used to control single run

// ============================================================================
/*!
 *  Destructor: virtual, implemented in derived classes.
 *  Wait until printing thread ends (BaseTraceCollector::run)
 */
// ============================================================================

BaseTraceCollector:: ~BaseTraceCollector()
{
}

// ============================================================================
/*!
 * Constructor: no need of LocalTraceBufferPool object initialization here,
 * thread safe singleton used in LocalTraceBufferPool::instance()
 * See derived classes.
 */
// ============================================================================

BaseTraceCollector::BaseTraceCollector()
{
}


