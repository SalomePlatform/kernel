# -* Makefile *- 
#
# Author : Guillaume BOULANT (CSSI)
# Date : 09/11/2005
# $Header$
#

if CORBA_GEN
  IDLDIR = idl
endif

SUBDIRS = $(IDLDIR) src doc salome_adm bin resources

DIST_SUBDIRS=   idl src doc salome_adm bin resources

DISTCLEANFILES = a.out

EXTRA_DIST = \
	build_configure \
	clean_configure \
	LICENCE \
	README.FIRST.txt \
	DEPRECATED

dist-hook:
	rm -rf `find $(distdir) -name CVS`

userdoc:
	(cd doc && $(MAKE) $(AM_MAKEFLAGS) userdoc)