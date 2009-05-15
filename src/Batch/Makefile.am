#  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
#
#  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
#  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
#  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
####################################### library
#  SALOME Container : implementation of container and engine for Kernel
#  File   : Makefile.in
#  Author : EDF
#  Module : SALOME
#  $Header$
#
include $(top_srcdir)/salome_adm/unix/make_common_starter.am

# header files  
LIB_INCLUDES = \
	Batch_APIInternalFailureException.hxx \
	Batch_BatchManager.hxx \
	Batch_BatchManagerCatalog.hxx \
	Batch_BoolType.hxx \
	Batch_CharType.hxx \
	Batch_ConnexionFailureException.hxx \
	Batch_Couple.hxx \
	Batch_CoupleType.hxx \
	Batch_Date.hxx \
	Batch_DateType.hxx \
	Batch_Environnement.hxx \
	Batch_FactBatchManager.hxx \
	Batch_GenericException.hxx \
	Batch_GenericType.hxx \
	Batch_IntType.hxx \
	Batch_InvalidArgumentException.hxx \
	Batch_InvalidKeyException.hxx \
	Batch_Job.hxx \
	Batch_JobId.hxx \
	Batch_JobInfo.hxx \
	Batch_ListIsFullException.hxx \
	Batch_LongType.hxx \
	Batch_MapKey.hxx \
	Batch_NotYetImplementedException.hxx \
	Batch_Parametre.hxx \
	Batch_PyVersatile.hxx \
	Batch_RunTimeException.hxx \
	Batch_StringType.hxx \
	Batch_TypeMismatchException.hxx \
	Batch_BatchManager_eClient.hxx \
	Batch_FactBatchManager_eClient.hxx \
	Batch_BatchManager_eLSF.hxx \
	Batch_FactBatchManager_eLSF.hxx \
	Batch_JobInfo_eLSF.hxx \
	Batch_BatchManager_ePBS.hxx \
	Batch_FactBatchManager_ePBS.hxx \
	Batch_JobInfo_ePBS.hxx \
	MpiImpl.hxx \
	Batch_Defines.hxx \
	Batch_BatchManager_eSGE.hxx \
	Batch_FactBatchManager_eSGE.hxx \
	Batch_JobInfo_eSGE.hxx \
	MpiImpl.hxx


LIB_SRC = \
	Batch_APIInternalFailureException.cxx \
	Batch_BatchManager.cxx \
	Batch_BatchManagerCatalog.cxx \
	Batch_BoolType.cxx \
	Batch_CharType.cxx \
	Batch_ConnexionFailureException.cxx \
	Batch_Couple.cxx \
	Batch_CoupleType.cxx \
	Batch_Date.cxx \
	Batch_DateType.cxx \
	Batch_Environnement.cxx \
	Batch_FactBatchManager.cxx \
	Batch_GenericException.cxx \
	Batch_GenericType.cxx \
	Batch_IntType.cxx \
	Batch_InvalidArgumentException.cxx \
	Batch_InvalidKeyException.cxx \
	Batch_Job.cxx \
	Batch_JobId.cxx \
	Batch_JobInfo.cxx \
	Batch_ListIsFullException.cxx \
	Batch_LongType.cxx \
	Batch_MapKey.cxx \
	Batch_NotYetImplementedException.cxx \
	Batch_Parametre.cxx \
	Batch_PyVersatile.cxx \
	Batch_RunTimeException.cxx \
	Batch_StringType.cxx \
	Batch_TypeMismatchException.cxx \
	Batch_BatchManager_eClient.cxx \
	Batch_FactBatchManager_eClient.cxx \
	Batch_BatchManager_eLSF.cxx \
	Batch_FactBatchManager_eLSF.cxx \
	Batch_JobInfo_eLSF.cxx \
	Batch_BatchManager_ePBS.cxx \
	Batch_FactBatchManager_ePBS.cxx \
	Batch_JobInfo_ePBS.cxx \
	Batch_BatchManager_eSGE.cxx \
	Batch_FactBatchManager_eSGE.cxx \
	Batch_JobInfo_eSGE.cxx \
	MpiImpl.cxx


LIB_CPPFLAGS = ${PYTHON_INCLUDES}
if !WITHONLYLAUNCHER
  LIB_CPPFLAGS += -I$(top_builddir)/salome_adm/unix
endif

LIB_LIBADD = $(PYTHON_LIBS)

#
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Special add for local batch system
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
if WITH_LOCAL
LIB_INCLUDES +=\
	Batch_Versatile.hxx \
	Batch_BatchManager_Local.hxx \
	Batch_BatchManager_Local_RSH.hxx \
	Batch_BatchManager_Local_SH.hxx \
	Batch_BatchManager_Local_SSH.hxx \
	Batch_FactBatchManager_Local.hxx \
	Batch_FactBatchManager_Local_RSH.hxx \
	Batch_FactBatchManager_Local_SH.hxx \
	Batch_FactBatchManager_Local_SSH.hxx \
	Batch_JobInfo_Local.hxx \
	Batch_Job_Local.hxx \
	Batch_IOMutex.hxx

LIB_SRC +=\
	Batch_Versatile.cxx \
	Batch_BatchManager_Local.cxx \
	Batch_BatchManager_Local_RSH.cxx \
	Batch_BatchManager_Local_SH.cxx \
	Batch_BatchManager_Local_SSH.cxx \
	Batch_FactBatchManager_Local.cxx \
	Batch_FactBatchManager_Local_RSH.cxx \
	Batch_FactBatchManager_Local_SH.cxx \
	Batch_FactBatchManager_Local_SSH.cxx \
	Batch_JobInfo_Local.cxx \
	Batch_Job_Local.cxx \
	Batch_IOMutex.cxx

if !WITHONLYLAUNCHER
  LIB_CPPFLAGS += -DHAVE_CONFIG_H
endif

endif

#
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Special add for openpbs batch system
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
if WITH_OPENPBS
LIB_INCLUDES += \
	Batch_BatchManager_PBS.hxx \
	Batch_FactBatchManager_PBS.hxx \
	Batch_JobInfo_PBS.hxx \
	Batch_Job_PBS.hxx

LIB_SRC +=\
	Batch_BatchManager_PBS.cxx \
	Batch_FactBatchManager_PBS.cxx \
	Batch_JobInfo_PBS.cxx \
	Batch_Job_PBS.cxx

LIB_CPPFLAGS += @OPENPBS_INCLUDES@
LIB_LIBADD   += @OPENPBS_LIBDIR@ @OPENPBS_LIBS@

endif

#
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Special add for lsf batch system
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
if WITH_LSF
LIB_INCLUDES += \
	Batch_BatchManager_LSF.hxx \
	Batch_FactBatchManager_LSF.hxx \
	Batch_JobInfo_LSF.hxx \
	Batch_Job_LSF.hxx

LIB_SRC += \
	Batch_BatchManager_LSF.cxx \
	Batch_FactBatchManager_LSF.cxx \
	Batch_JobInfo_LSF.cxx \
	Batch_Job_LSF.cxx

LIB_CPPFLAGS += ${LSF_INCLUDES}
LIB_LIBADD   += ${LSF_LDFLAGS} ${LSF_LIBS}
endif




salomeinclude_HEADERS = $(LIB_INCLUDES)

#
# ===============================================================
# Libraries targets
# ===============================================================
#
lib_LTLIBRARIES = libSalomeBatch.la 
libSalomeBatch_la_SOURCES = $(LIB_SRC)
libSalomeBatch_la_CPPFLAGS = ${LIB_CPPFLAGS}
libSalomeBatch_la_LDFLAGS  = -no-undefined -version-info=0:0:0
libSalomeBatch_la_LIBADD   = $(LIB_LIBADD)
