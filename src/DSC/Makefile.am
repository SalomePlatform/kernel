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

#  File   : Makefile.am
#  Author : André RIBES (EDF)
#  Module : KERNEL
#
include $(top_srcdir)/salome_adm/unix/make_common_starter.am

SUBDIRS_COMMON = DSC_Basic 

if !WINDOWS
  SUBDIRS_ADD = DSC_User DSC_Python
endif

if WITH_PACO_PARALLEL
  SUBDIR_PAR = ParallelDSC
endif

SUBDIRS = $(SUBDIRS_COMMON) $(SUBDIRS_ADD) $(SUBDIR_PAR) 

EXTRA_DIST+= DSC.dox

DIST_SUBDIRS = DSC_Basic DSC_User DSC_Python ParallelDSC
