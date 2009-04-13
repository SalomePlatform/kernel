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
# Author : Guillaume Boulant (CSSI)
# Module : KERNEL
# $Header$
#
include $(top_srcdir)/salome_adm/unix/make_common_starter.am

#
# ===============================================================
# Files to be installed
# ===============================================================
#

# These files are data, module or lib files
dist_salomescript_DATA =	\
	config_appli.xml	\
	salome.launch

nodist_salomescript_DATA =	\
	VERSION

# These files are executable scripts
dist_salomescript_SCRIPTS =		\
	appli_clean.sh			\
	appli_install.sh		\
	createAppli.sh			\
	runIDLparser			\
	runNS.sh			\
	runSalome			\
	runSalome.csh			\
	runSalome.ksh			\
	waitNS.sh

# These files are python files
dist_salomescript_PYTHON =		\
	addToKillList.py		\
	appli_gen.py			\
	envSalome.py			\
	killSalome.py			\
	killSalomeWithPort.py		\
	launchConfigureParser.py	\
	launchSalome.py			\
	nameserver.py			\
	NSparam.py			\
	orbmodule.py			\
	runNS.py			\
	runSalome.py			\
	salomeConsole.py		\
	salome_session.py		\
	salome_utils.py	        	\
	server.py			\
	setenv.py			\
	showNS.py			\
	shutdownSalome.py		\
	virtual_salome.py		\
	waitContainers.py		\
	waitNS.py

EXTRA_DIST = appliskel

dist-hook:
	rm -rf `find $(distdir)/appliskel -name CVS`

DISTCLEANFILES = $(top_builddir)/bin/salome/Makefile.am

# This is an ugly target to avoid exploring the appliskel subdirectory.
install-exec-local:
	$(INSTALL) -d $(DESTDIR)$(salomescriptdir)
	$(RM) -rf $(DESTDIR)$(salomescriptdir)/appliskel 1> /dev/null 2>&1
	cp -r $(srcdir)/appliskel $(DESTDIR)$(salomescriptdir)
	chmod a+x -R $(DESTDIR)$(salomescriptdir)/appliskel
	find $(DESTDIR)$(salomescriptdir) -name CVS -prune -exec rm -rf {} \;

uninstall-local:
	if test -d $(DESTDIR)$(salomescriptdir)/appliskel; then \
	  find $(DESTDIR)$(salomescriptdir)/appliskel -exec chmod +w {} ';' ; \
	  rm -rf $(DESTDIR)$(salomescriptdir)/appliskel; \
	fi;

install-data-hook:
	@for f in $(dist_salomescript_PYTHON) ; do \
          chmod -f a+x $(DESTDIR)$(salomescriptdir)/$$f ; \
        done
