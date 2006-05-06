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
	cp -fr $(srcdir)/KERNEL ./INPUT; \
	cd INPUT; \
	sed 's|../../../share/salome|@top_srcdir@|' ./doxyfile > ./doxyfile1; \
	sed 's|../../../build/salome|@top_builddir@|' ./doxyfile1 > ./doxyfile2; \
	mv -f doxyfile2 doxyfile1; \
	echo "DOXYGEN SUPPORT PYTHON - $(DOXYGEN_WITH_PYTHON)"; \
	if( test "x$(DOXYGEN_WITH_PYTHON)" = "xyes"); then \
	  sed 's|python_extension_must_be_here|*.py|' ./doxyfile1 > ./doxyfile2; \
	  mv -f doxyfile2 doxyfile1; \
	  $(DOXYGEN) -u ./doxyfile1; \
	else \
	  sed 's|python_extension_must_be_here||' ./doxyfile1 > ./doxyfile2; \
	  mv -f doxyfile2 doxyfile1; \
	fi; \
	if( test "x$(DOXYGEN_WITH_STL)" = "xyes"); then \
	  sed -e 's|BUILTIN_STL_SUPPORT    = NO|BUILTIN_STL_SUPPORT    = YES|' ./doxyfile1 > ./doxyfile2; \
	  mv -f doxyfile2 doxyfile1; \
	fi; \
	mv -f doxyfile1 doxyfile; \
	$(DOXYGEN) ./doxyfile; \
	cd ..;
	$(INSTALL) -d $(docdir)/tui/KERNEL;
	cp -fr KERNEL $(docdir)/tui
	cp -fr $(srcdir)/KERNEL/sources/static/*.* $(docdir)/tui/KERNEL;
	cp -fr $(srcdir)/KERNEL/sources/ $(docdir)/tui/KERNEL;
	cp -fr $(srcdir)/KERNEL/HTML/ $(docdir)/tui/KERNEL;
	cp -f $(srcdir)/pythfilter.py $(docdir)/tui/KERNEL;

