# Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
#
# This library is distributed in the hope that it will be useful,
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

include $(top_srcdir)/salome_adm/unix/make_common_starter.am


# The deprecated files are deprecated for the KERNEL but not for the
# related modules (GUI, MED, ...). Some files have to be installed.
DEPRECATED_FILES=\
	DEPRECATED/ac_cc_warnings.m4 \
	DEPRECATED/ac_cxx_bool.m4 \
	DEPRECATED/ac_cxx_mutable.m4 \
	DEPRECATED/ac_cxx_partial_specialization.m4 \
	DEPRECATED/ac_cxx_typename.m4 \
	DEPRECATED/check_mico.m4 \
	DEPRECATED/check_pthreads.m4 \
	DEPRECATED/config.sub \
	DEPRECATED/config.guess \
	DEPRECATED/install-sh \
	DEPRECATED/libtool.m4 \
	DEPRECATED/ltconfig \
	DEPRECATED/ltmain.sh \
	DEPRECATED/check_Salome.m4 \
	DEPRECATED/check_java.m4 \
	DEPRECATED/missing \
	DEPRECATED/mkinstalldirs

dist_salomem4_DATA=\
ac_cxx_depend_flag.m4 \
ac_cxx_have_sstream.m4 \
ac_cxx_namespaces.m4 \
ac_cxx_option.m4 \
ac_cxx_template_options.m4 \
ac_cxx_use_std_iostream.m4 \
ac_cxx_warnings.m4 \
ac_linker_options.m4 \
ac_check_sizeof_fortran.m4 \
acx_pthread.m4 \
check_Kernel.m4 \
check_boost.m4 \
check_cas.m4 \
check_corba.m4 \
check_cppunit.m4 \
check_f77.m4 \
check_calcium.m4 \
check_hdf5.m4 \
check_htmlgen.m4 \
check_sphinx.m4 \
check_lam.m4 \
check_libbatch.m4 \
check_mpi.m4 \
check_openmpi.m4 \
check_mpich.m4 \
check_omniorb.m4 \
check_sockets.m4 \
check_swig.m4 \
check_vtk.m4 \
check_withihm.m4 \
enable_pthreads.m4 \
production.m4 \
pyembed.m4 \
python.m4 \
check_libxml.m4 \
check_paco++.m4 \
local_install.m4 \
hack_libtool.m4 \
check_tbb.m4

dist_salome4depr_DATA=\
$(DEPRECATED_FILES)
