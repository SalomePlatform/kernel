# -* Makefile *- 
#
# Author : Paul RASCLE (EDF)
# Date : 16/02/2006
# $Header$
#

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

SUBDIRS= tui DevelopersGuide gui

index_html:
	$(INSTALL) -d $(docdir); 
	cp -f $(srcdir)/KERNEL_index.html $(docdir)

userdoc: index_html
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  echo "Making userdoc in $$subdir"; \
	  (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) userdoc) \
	done;
