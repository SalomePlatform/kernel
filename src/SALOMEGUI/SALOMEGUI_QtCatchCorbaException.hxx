//=============================================================================
// File      : SALOMEGUI_QtCatchCorbaException.hxx
// Created   : mer déc 19 14:31:42 CET 2001
// Author    : Paul RASCLE, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

#ifndef _SALOMEGUI__QTCATCHCORBAEXCEPTION_HXX_
#define _SALOMEGUI__QTCATCHCORBAEXCEPTION_HXX_

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_Exception)

void QtCatchCorbaException(const SALOME::SALOME_Exception & S_ex);

#endif
