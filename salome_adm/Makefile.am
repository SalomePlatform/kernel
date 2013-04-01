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
include $(top_srcdir)/salome_adm/unix/make_common_starter.am

SUBDIRS = unix cmake_files
AUTOMAKE_OPTIONS=filename-length-max=255

# rules for install and build (see make_common_starter.am)
salomeadmux_DATA = SALOMEconfig.h.in
salomeinclude_DATA = SALOMEconfig.h
BUILT_SOURCES=SALOMEconfig.ref SALOMEconfig.h SALOMEconfig.h.in

mostlyclean-local:
	-rm -rf SALOMEconfig.h SALOMEconfig.h.in

SALOMEconfig.h.in : SALOMEconfig.ref.in
	cp -pf $< $@;

# test if SALOMEconfig.ref has changed (contents), at build time
SALOMEconfig.h: $(top_builddir)/salome_adm/SALOMEconfig.ref
	@if ! [ -a $@ ]; then    \
	  echo cp -pf $< $@;     \
	  cp -pf $< $@;          \
	fi;		         \
	if ! cmp -s $< $@; then  \
	  echo cp -pf $< $@;     \
	  cp -pf $< $@;          \
	fi;
