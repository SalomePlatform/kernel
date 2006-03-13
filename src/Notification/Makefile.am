#  SALOME Notification : wrapping of Notification service services
#
#  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
#  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
# 
#  This library is free software; you can redistribute it and/or 
#  modify it under the terms of the GNU Lesser General Public 
#  License as published by the Free Software Foundation; either 
#  version 2.1 of the License. 
# 
#  This library is distributed in the hope that it will be useful, 
#  but WITHOUT ANY WARRANTY; without even the implied warranty of 
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
#  Lesser General Public License for more details. 
# 
#  You should have received a copy of the GNU Lesser General Public 
#  License along with this library; if not, write to the Free Software 
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
# 
#  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
#
#
#
#  File   : Makefile.am
#  Author : Guillaume Boulant (CSSI)
#  Module : KERNEL
#  $Header$

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

#
# ===============================================================
# Header to be installed
# ===============================================================
#
# header files  
salomeinclude_HEADERS = \
	NOTIFICATION.hxx          \
	NOTIFICATION_Supplier.hxx \
	NOTIFICATION_Consumer.hxx \
	CosNotifyShorthands.h

#
# ===============================================================
# Libraries targets
# ===============================================================
#

lib_LTLIBRARIES = libSalomeNotification.la
libSalomeNotification_la_SOURCES =\
	NOTIFICATION.cxx          \
	NOTIFICATION_Supplier.cxx \
	NOTIFICATION_Consumer.cxx

libSalomeNotification_la_CPPFLAGS =\
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(srcdir)/../Utils \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@


libSalomeNotification_la_LDFLAGS  = -no-undefined -version-info=0:0:0

libSalomeNotification_la_LIBADD =\
	../Utils/libOpUtil.la \
	../SALOMELocalTrace/libSALOMELocalTrace.la \
	../Basics/libSALOMEBasics.la \
	@CORBA_LIBS@


#LDFLAGS+= -lOpUtil -lSALOMELocalTrace
#OMNIORB_IDLCXXFLAGS+= -Wbtp
