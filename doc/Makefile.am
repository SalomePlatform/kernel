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
# Date : 06/02/2006
# $Header$
#

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

SUBDIRS = salome

# if RST2HTML_IS_OK
#   DOC_FILES = \
# 	index.html \
# 	UnitTests.html \
# 	SALOME_Application.html \
# 	INSTALL.html \
# 	kernel_resources.html \
# 	KERNEL_Services.html
#   CSS_FILE = rst.css
# endif

nodist_doc_DATA = $(DOC_FILES)

EXTRA_DIST=$(DOC_FILES:%.html=%.txt) $(CSS_FILE)

# Building html from restructured text files
STYLESHEET=$(CSS_FILE)
RST2HTML=@RST2HTML@
RSTOPTS=--output-encoding=latin1 --stylesheet=$(STYLESHEET)

%.html: %.txt
	$(RST2HTML) $(RSTOPTS) $< $@

usr_docs:
	(cd salome && $(MAKE) $(AM_MAKEFLAGS) usr_docs)

dev_docs:
	(cd salome && $(MAKE) $(AM_MAKEFLAGS) dev_docs)
