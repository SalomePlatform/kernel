# Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
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

mypkgpythondir =$(salomepypkgdir)/kernel

# This first set of modules could be used in any context, you just
# have to get a python environement.
mypkgpython_PYTHON = \
	__init__.py      \
	deprecation.py   \
	logger.py        \
	termcolor.py     \
	unittester.py    \
	pyunittester.py  \
	enumerate.py     \
	uiexception.py   \
	datamodeler.py   \
	testdata.py      \
	diclookup.py     \
	threadhelper.py  \
	syshelper.py

nodist_mypkgpython_PYTHON = \
	logconfig.py

# This set of modules required a running SALOME application.
mypkgpython_PYTHON += \
	studyedit.py     \
	services.py

bin_SCRIPTS=\
	kernelpy_autotest.sh

SUBDIRS = parametric
