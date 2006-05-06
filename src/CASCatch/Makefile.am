#  File   : Makefile.in
#  Author : Sergey RUIN (OCN)
#  Module : SALOME

include $(top_srcdir)/salome_adm/unix/make_common_starter.am


salomeinclude_HEADERS   = CASCatch.hxx \
			CASCatch_CatchSignals.hxx \
			CASCatch_ErrorHandler.hxx \
			CASCatch_Failure.hxx

lib_LTLIBRARIES             = libCASCatch.la
libCASCatch_la_SOURCES  = \
			CASCatch_CatchSignals.cxx \
			CASCatch_ErrorHandler.cxx \
			CASCatch_Failure.cxx

COMMON_CPPFLAGS = \
	@CAS_CPPFLAGS@ @CAS_CXXFLAGS@

libCASCatch_la_CPPFLAGS = $(COMMON_CPPFLAGS)
libCASCatch_la_LIBADD  = $(CAS_KERNEL)
