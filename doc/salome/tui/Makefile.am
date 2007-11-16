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

EXTRA_DIST = KERNEL pythfilter.py

dist-hook:
	rm -rf `find $(distdir) -name CVS`

usr_docs:
	cp -fr $(srcdir)/KERNEL/* ./INPUT; \
	cd ./INPUT; \
        pwd; ls;\
	echo "DOXYGEN SUPPORT PYTHON - $(DOXYGEN_WITH_PYTHON)"; \
	if( test "x$(DOXYGEN_WITH_PYTHON)" = "xyes"); then \
	  sed 's|python_extension_must_be_here|*.py|' ./doxyuser > ./doxyuser1; \
	  $(DOXYGEN) -u ./doxyuser1; \
	else \
	  sed 's|python_extension_must_be_here||' ./doxyuser > ./doxyuser1; \
	fi; \
	echo "DOXYGEN SUPPORT STL - $(DOXYGEN_WITH_STL)"; \
	if( test "x$(DOXYGEN_WITH_STL)" = "xyes"); then \
	  sed -e 's|BUILTIN_STL_SUPPORT    = NO|BUILTIN_STL_SUPPORT    = YES|' ./doxyuser1 > ./doxyuser2; \
	  mv -f doxyuser2 doxyuser1; \
	fi; \
	$(DOXYGEN) ./doxyuser1; \
	cd ..;
#	$(INSTALL) -d $(docdir)/tui/KERNEL;
	cp -fr $(srcdir)/KERNEL/sources/static/*.* $(docdir);
	cp -fr $(srcdir)/KERNEL/sources/ $(docdir);
	cp -fr KERNEL/[a-f]*.* $(docdir)
	cp -fr KERNEL/[g-l]*.* $(docdir)
	cp -fr KERNEL/[m-z]*.* $(docdir)
	cp -fr $(srcdir)/KERNEL/HTML/ $(docdir);
	cp -f $(srcdir)/pythfilter.py $(docdir);
	cp -fr $(srcdir)/KERNEL/exemple/ $(docdir);
#	cp -fr $(srcdir)/KERNEL/sources/static/*.* $(docdir)/tui/KERNEL;
#	cp -fr $(srcdir)/KERNEL/sources/ $(docdir)/tui/KERNEL;
#	cp -fr KERNEL $(docdir)/tui
#	cp -fr $(srcdir)/KERNEL/HTML/ $(docdir)/tui/KERNEL;
#	cp -f $(srcdir)/pythfilter.py $(docdir)/tui/KERNEL;
#	cp -fr $(srcdir)/KERNEL/exemple/ $(docdir)/tui/KERNEL;

dev_docs:
	cp -fr $(srcdir)/KERNEL/* ./INPUT; \
	cd ./INPUT; \
        pwd; ls;\
	echo "DOXYGEN SUPPORT PYTHON - $(DOXYGEN_WITH_PYTHON)"; \
	if( test "x$(DOXYGEN_WITH_PYTHON)" = "xyes"); then \
	  sed 's|python_extension_must_be_here|*.py|' ./doxydev > ./doxydev1; \
	  $(DOXYGEN) -u ./doxydev1; \
	else \
	  sed 's|python_extension_must_be_here||' ./doxydev > ./doxydev1; \
	fi; \
	echo "DOXYGEN SUPPORT STL - $(DOXYGEN_WITH_STL)"; \
	if( test "x$(DOXYGEN_WITH_STL)" = "xyes"); then \
	  sed -e 's|BUILTIN_STL_SUPPORT    = NO|BUILTIN_STL_SUPPORT    = YES|' ./doxydev1 > ./doxydev2; \
	  mv -f doxydev2 doxydev1; \
	fi; \
	$(DOXYGEN) ./doxydev1; \
	cd ..;
	$(INSTALL) -d $(docdir)/tui/KERNEL;
#	cp -fr KERNEL $(docdir)/tui
#	cp -fr $(srcdir)/KERNEL/sources/static/*.* $(docdir)/tui/KERNEL;
#	cp -fr $(srcdir)/KERNEL/sources/ $(docdir)/tui/KERNEL;
#	cp -fr $(srcdir)/KERNEL/HTML/ $(docdir)/tui/KERNEL;
#	cp -f $(srcdir)/pythfilter.py $(docdir)/tui/KERNEL;
