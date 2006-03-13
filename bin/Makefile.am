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
	salome.launch \
	salomeConsole.py

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
	cp -r $(srcdir)/appliskel $(salomescriptdir)
	find $(salomescriptdir) -name CVS | xargs rm -rf

uninstall-local:
	find $(salomescriptdir)/appliskel -exec chmod +w {} \;
	rm -rf $(salomescriptdir)/appliskel
