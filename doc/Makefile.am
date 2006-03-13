# -* Makefile *- 
#
# Author : Paul RASCLE (EDF)
# Date : 06/02/2006
# $Header$
#

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

SUBDIRS = salome

doc_DATA = \
	index.html \
	UnitTests.html \
	SALOME_Application.html \
	INSTALL.html \
	kernel_resources.html

EXTRA_DIST=$(doc_DATA)


# Building html from restructured text files
STYLESHEET=rst.css
RST2HTML=@RST2HTML@
RSTOPTS=--output-encoding=latin1 --stylesheet=$(STYLESHEET)
#RSTOPTS=--output-encoding=latin1

%.html: $(srcdir)/%.txt
	$(RST2HTML) $(RSTOPTS) $< $@

userdoc:
	(cd salome && $(MAKE) $(AM_MAKEFLAGS) userdoc)