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
# -* Makefile *- 
#
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
dist_salomescript_DATA= \
	VERSION \
	orbmodule.py \
	salomeConsole.py \
	salome_session.py

# These files are executable scripts
dist_salomescript_SCRIPTS=\
	envSalome.py \
	runIDLparser \
	runSalome.py \
	killSalome.py \
	killSalomeWithPort.py \
	runSalome \
	runSalome.csh \
	runNS.sh \
	createAppli.sh \
	appli_install.sh \
	appli_clean.sh \
	virtual_salome.py \
	launchConfigureParser.py \
	showNS.py \
	addToKillList.py \
	NSparam.py

EXTRA_DIST = appliskel

dist-hook:
	rm -rf `find $(distdir)/appliskel -name CVS`

DISTCLEANFILES = $(top_builddir)/bin/@PACKAGE@/Makefile.am

clean-local:
	rm -rf $(top_builddir)/bin/@PACKAGE@/appliskel
	cd $(top_builddir)/bin/@PACKAGE@; \
	rm -f $(dist_salomescript_DATA) $(dist_salomescript_SCRIPTS)

# This is an ugly target to avoid exploring the appliskel subdirectory.
install-exec-local:
	$(INSTALL) -d $(salomescriptdir)
	$(RM) -rf $(salomescriptdir)/appliskel 1> /dev/null 2>&1
	cp -r $(srcdir)/appliskel $(salomescriptdir)
	find $(salomescriptdir) -name CVS -prune -exec rm -rf {} \;

uninstall-local:
	find $(salomescriptdir)/appliskel -exec chmod +w {} \;
	rm -rf $(salomescriptdir)/appliskel
