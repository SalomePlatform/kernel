# Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

# -* Makefile *- 
# Author : Guillaume BOULANT (CSSI)
# Date : 09/11/2005
# $Header$
#
include $(top_srcdir)/salome_adm/unix/make_common_starter.am
#
if CORBA_GEN
  IDLDIR = idl
endif

ACLOCAL_AMFLAGS = -I salome_adm/unix/config_files

if WITHONLYLAUNCHER
  SUBDIRS = src resources
else
  SUBDIRS = salome_adm $(IDLDIR) src bin doc resources
endif

DIST_SUBDIRS= salome_adm idl src doc bin resources

DISTCLEANFILES = a.out local-install.sh hack_libtool aclocal.m4 configure salome_adm/unix/config_files/config.guess salome_adm/unix/config_files/config.sub salome_adm/unix/config_files/depcomp salome_adm/unix/config_files/install-sh salome_adm/unix/config_files/libtool.m4 salome_adm/unix/config_files/ltmain.sh salome_adm/unix/config_files/ltoptions.m4 salome_adm/unix/config_files/ltsugar.m4 salome_adm/unix/config_files/ltversion.m4 salome_adm/unix/config_files/lt~obsolete.m4 salome_adm/unix/config_files/missing salome_adm/unix/config_files/py-compile

salomeinclude_DATA = KERNEL_version.h

EXTRA_DIST += \
	build_cmake \
	build_cmake.bat \
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
