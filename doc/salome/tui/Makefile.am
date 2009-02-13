#  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
#
#  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
#  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
# -* Makefile *- 
# Author : Paul RASCLE (EDF)
# Date : 16/02/2006
# $Header$
#
include $(top_srcdir)/salome_adm/unix/make_common_starter.am

dev_docs:
	echo "DOXYGEN SUPPORT PYTHON - $(DOXYGEN_WITH_PYTHON)"; \
	if( test "x$(DOXYGEN_WITH_PYTHON)" = "xyes"); then \
	  sed 's|python_extension_must_be_here|*.py|' ./doxyfile > ./doxyfile1; \
	  $(DOXYGEN) -u ./doxyfile1; \
	else \
	  sed 's|python_extension_must_be_here||' ./doxyfile > ./doxyfile1; \
	fi; \
	echo "DOXYGEN SUPPORT STL - $(DOXYGEN_WITH_STL)"; \
	if( test "x$(DOXYGEN_WITH_STL)" = "xyes"); then \
	  sed -e 's|BUILTIN_STL_SUPPORT    = NO|BUILTIN_STL_SUPPORT    = YES|' ./doxyfile1 > ./doxyfile2; \
	  mv -f doxyfile2 doxyfile1; \
	fi; \
	$(DOXYGEN) ./doxyfile1

clean-local:
	-rm -fr KERNEL log.txt doxyfile1.bak doxyfile1

install-data-local:
	if test -d KERNEL; then \
	$(INSTALL) -d $(DESTDIR)$(docdir)/tui ; \
	cp -rp KERNEL $(DESTDIR)$(docdir)/tui ; \
	fi;

uninstall-local:
	rm -rf $(DESTDIR)$(docdir)/tui/KERNEL

EXTRA_DIST=static KERNEL/exemple

dist-hook:
	rm -rf $(distdir)/static/CVS ; \
	rm -rf $(distdir)/KERNEL/exemple/CVS

