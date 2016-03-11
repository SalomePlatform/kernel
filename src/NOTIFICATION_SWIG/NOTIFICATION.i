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
//  File   : NOTIFICATION.i
//  Author : Francis KLOSS
//  Module : SALOME
//
%module libNOTIFICATION

%feature("autodoc", "1");

%{
//#define __declspec(a)
#include "NOTIFICATION_Swig.hxx"
%}

%include SALOME_NOTIFICATION.hxx
%include SALOME_NOTIFICATION_SWIG.hxx
%include NOTIFICATION.hxx
%include NOTIFICATION_Swig.hxx
