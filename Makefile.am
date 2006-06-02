# Copyright (C) 2005  OPEN CASCADE, CEA, EDF R&D, LEG
#           PRINCIPIA R&D, EADS CCR, Lip6, BV, CEDRAT
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either 
# version 2.1 of the License.
# 
# This library is distributed in the hope that it will be useful 
# but WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public  
# License along with this library; if not, write to the Free Software 
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
# 
# http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
# -* Makefile *- 
#
# Author : Guillaume BOULANT (CSSI)
# Date : 09/11/2005
# $Header$
#

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

if CORBA_GEN
  IDLDIR = idl
endif

SUBDIRS = $(IDLDIR) salome_adm src doc bin resources

DIST_SUBDIRS=   idl salome_adm src doc bin resources

DISTCLEANFILES = a.out

salomeinclude_DATA = KERNEL_version.h

EXTRA_DIST = \
	build_configure \
	clean_configure \
	LICENCE \
	README.FIRST.txt \
	DEPRECATED

dist-hook:
	rm -rf `find $(distdir) -name CVS`

usr_docs:
	(cd doc && $(MAKE) $(AM_MAKEFLAGS) usr_docs)

docs:usr_docs

dev_docs:
	(cd doc && $(MAKE) $(AM_MAKEFLAGS) dev_docs)