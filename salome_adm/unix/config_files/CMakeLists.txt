# Copyright (C) 2012-2016  CEA/DEN, EDF R&D, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
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
 
# The deprecated files are deprecated for the KERNEL but not for the
# related modules (GUI, MED, ...). Some files have to be installed.
SET(DEPRECATED_FILES
  DEPRECATED/ac_cc_warnings.m4
  DEPRECATED/ac_cxx_bool.m4
  DEPRECATED/ac_cxx_mutable.m4
  DEPRECATED/ac_cxx_partial_specialization.m4
  DEPRECATED/ac_cxx_typename.m4
  DEPRECATED/check_mico.m4
  DEPRECATED/check_pthreads.m4
  DEPRECATED/config.sub
  DEPRECATED/config.guess
  DEPRECATED/install-sh
  DEPRECATED/libtool.m4
  DEPRECATED/ltconfig
  DEPRECATED/ltmain.sh
  DEPRECATED/check_Salome.m4
  DEPRECATED/check_java.m4
  DEPRECATED/missing
  DEPRECATED/mkinstalldirs
)

SET(m4_DATA
  ac_cxx_depend_flag.m4
  ac_cxx_have_sstream.m4
  ac_cxx_namespaces.m4
  ac_cxx_option.m4
  ac_cxx_template_options.m4
  ac_cxx_use_std_iostream.m4
  ac_cxx_warnings.m4
  ac_linker_options.m4
  ac_check_sizeof_fortran.m4
  acx_pthread.m4
  check_Kernel.m4
  check_boost.m4
  check_cas.m4
  check_corba.m4
  check_cppunit.m4
  check_f77.m4
  check_calcium.m4
  check_hdf5.m4
  check_htmlgen.m4
  check_sphinx.m4
  check_lam.m4
  check_libbatch.m4
  check_mpi.m4
  check_openmpi.m4
  check_mpich.m4
  check_omniorb.m4
  check_sockets.m4
  check_swig.m4
  check_vtk.m4
  check_withihm.m4
  enable_pthreads.m4
  production.m4
  pyembed.m4
  python.m4
  check_libxml.m4
  check_paco++.m4
  local_install.m4
  hack_libtool.m4
  check_tbb.m4
  check_med3.m4
)
INSTALL(FILES ${DEPRECATED_FILES} DESTINATION ${SALOME_INSTALL_AMCONFIG}/config_files/DEPRECATED)
INSTALL(FILES ${m4_DATA} DESTINATION ${SALOME_INSTALL_AMCONFIG}/config_files)
