include $(top_srcdir)/salome_adm/unix/make_common_starter.am

usr_docs: doxyfile ../Batch.html
	echo "Running doxygen in directory: "`pwd`; \
	$(DOXYGEN) $< ; \
	cp -rf ../Batch.html KERNEL

docs: usr_docs

info_TEXINFOS = ../Batch.texi

install-data-local: usr_docs
	$(INSTALL) -d $(DESTDIR)$(docdir)/gui
	cp -rp KERNEL $(DESTDIR)$(docdir)/gui

uninstall-local:
	rm -rf $(DESTDIR)$(docdir)/gui/KERNEL

clean-local:
	-rm -fr KERNEL log.txt

dist-hook:
	cp -Rp $(srcdir)/images $(distdir) ; \
	rm -rf $(distdir)/images/CVS ; \
	cp -Rp $(srcdir)/static $(distdir) ; \
	rm -rf $(distdir)/static/CVS

# VSR: nullify these make targets to avoid 'make distcheck' failure
dvi:
pdf:
ps:

