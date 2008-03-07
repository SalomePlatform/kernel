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

doc_DATA =

if RST2HTML_IS_OK
#  doc_DATA += \
#	index.html \
#	UnitTests.html \
#	SALOME_Application.html \
#	INSTALL.html \
#	kernel_resources.html \
#	KERNEL_Services.html
endif

EXTRA_DIST=$(doc_DATA)


# Building html from restructured text files
STYLESHEET=$(srcdir)/rst.css
RST2HTML=@RST2HTML@
RSTOPTS=--output-encoding=latin1 --stylesheet=$(STYLESHEET)
#RSTOPTS=--output-encoding=latin1

%.html: $(srcdir)/%.txt
	$(RST2HTML) $(RSTOPTS) $< $@

usr_docs:
	(cd salome && $(MAKE) $(AM_MAKEFLAGS) usr_docs)

dev_docs:
	(cd salome && $(MAKE) $(AM_MAKEFLAGS) dev_docs)
