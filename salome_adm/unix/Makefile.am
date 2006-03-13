
include $(top_srcdir)/salome_adm/unix/make_common_starter.am

SUBDIRS = config_files

DEPRECATED_FILES=\
	DEPRECATED/F77config.h.in \
	DEPRECATED/sstream.in \
	DEPRECATED/envScript.in \
	DEPRECATED/depend.in \
	DEPRECATED/make_commence.in \
	DEPRECATED/make_conclude.in \
	DEPRECATED/make_module.in \
	DEPRECATED/make_omniorb.in

# The deprecated files are deprecated for the KERNEL but not for the
# related modules (GUI, MED, ...), for now. Some files have to be installed.
dist_salomeadmux_DATA=\
	SALOMEconfig.h.in \
	pythonbe.py \
	$(DEPRECATED_FILES)
