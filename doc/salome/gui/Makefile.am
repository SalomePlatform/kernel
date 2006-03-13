# -* Makefile *- 
#
# Author : Paul RASCLE (EDF)
# Date : 16/02/2006
# $Header$
#

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

EXTRA_DIST = $(srcdir)/KERNEL

dist-hook:
	rm -rf `find $(distdir) -name CVS`

userdoc:
	$(INSTALL) -d $(docdir)/gui; 
	cp -fr $(srcdir)/KERNEL $(docdir)/gui; 
	rm -rf `find $(docdir)/gui -name CVS`
