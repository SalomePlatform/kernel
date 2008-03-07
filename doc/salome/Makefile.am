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
# -* Makefile *- 
#
# Author : Paul RASCLE (EDF)
# Date : 16/02/2006
# $Header$
#

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

SUBDIRS= tui
SUBDIRSGUI= tui
SUBDIRSTUI= tui

index_html:
	$(INSTALL) -d $(DESTDIR)$(docdir); 

usr_docs: index_html
	list='$(SUBDIRSGUI)'; for subdir in $$list; do \
	  echo "Making usr_docs in $$subdir"; \
	  (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) usr_docs) \
	done;

dev_docs: index_html
	list='$(SUBDIRSTUI)'; for subdir in $$list; do \
	  echo "Making dev_docs in $$subdir"; \
	  (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) dev_docs) \
	done;


info_TEXINFOS = Batch.texi

install-data-local:  html usr_docs
	cp -rp $(top_builddir)/doc/salome/Batch.html $(DESTDIR)$(docdir)
uninstall-local:
	rm -rf $(DESTDIR)$(docdir)/Batch.html

EXTRA_DIST= main.dox install.dox \
	    kernel_resources.dox kernel_services.dox \
	    salome_application.dox unittests.dox \
	    salome_file.dox batch.dox
