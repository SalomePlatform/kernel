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

#userdoc: $(TEX_PS) $(TEX_PDF)
userdoc:
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
