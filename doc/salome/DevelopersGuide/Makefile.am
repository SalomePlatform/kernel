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
# Date : 16/02/2006
# $Header$
#

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

TEX=DevelopersGuide.tex
TEX_PS=$(TEX:%.tex=%.ps)
TEX_PDF=$(TEX:%.tex=%.pdf)

#docs: $(TEX_PS) $(TEX_PDF)
docs:
	@echo document tex a revoir

clean:
	\rm -f *~ *.bak *.aux *.log *.dvi *.toc $(TEX:%.tex=%.out)
	\rm -f dummy thumb*


SUFFIXES = .tex .dvi .ps .pdf

#implicits rules
.tex.dvi: clean
	$(LATEX) $< 

.dvi.ps:
	$(DVIPS) $< -o $@

.tex.pdf: clean
	$(PDFLATEX) $< 
