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
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
# 

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

SUBDIRS = config_files

DEPRECATED_FILES=\
	DEPRECATED/F77config.h.in \
	DEPRECATED/sstream.in \
	DEPRECATED/envScript.in \
	DEPRECATED/depend.in \
	DEPRECATED/make_commence.in \
	DEPRECATED/make_conclude.in \
	DEPRECATED/make_module.in \
	DEPRECATED/make_omniorb.in

# rules for install and build (see make_common_starter.am)
salomeadmux_DATA = SALOMEconfig.h.in
salomeinclude_DATA = SALOMEconfig.h
BUILT_SOURCES=SALOMEconfig.ref SALOMEconfig.h SALOMEconfig.h.in

mostlyclean-local:
	-rm -rf SALOMEconfig.h SALOMEconfig.h.in

SALOMEconfig.h.in : SALOMEconfig.ref.in
	cp -p $< $@;

# test if SALOMEconfig.ref has changed (contents), at build time
SALOMEconfig.h: $(top_builddir)/salome_adm/unix/SALOMEconfig.ref
	@if ! [ -a $@ ]; then \
	  echo cp -p $< $@;   \
	  cp -p $< $@;        \
	fi;		      \
	if ! cmp -s $< $@; then  \
	  echo cp -p $< $@;   \
	  cp -p $< $@;        \
	fi;

# The deprecated files are deprecated for the KERNEL but not for the
# related modules (GUI, MED, ...), for now. Some files have to be installed.
dist_salomeadmux_DATA=\
	$(DEPRECATED_FILES)
