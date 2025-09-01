# Copyright (C) 2012-2025  CEA, EDF, OPEN CASCADE
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

SET(SALOME_VIRT_PYTHON_PACKAGE ${SALOME_KERNEL_INSTALL_PYTHON}/salome)

ADD_SUBDIRECTORY(kernel)

SET(salomepython_PYTHON
  omnipatch.py
)

SET(salomepython_PYTHON
    ${salomepython_PYTHON}
    Help.py
    PyInterp.py
    batchmode_salome.py
    kernel_test.py
    salome_test.py
    salome_kernel_utils.py
    salome_study.py
    salome_iapp.py
    salome_ComponentGUI.py
    iparameters.py
    salome_version.py
    salome_notebook.py
    salome_pynode.py
    salome_genericobj.py
    salome_psutil.py
  )

SALOME_INSTALL_SCRIPTS("${salomepython_PYTHON}" ${SALOME_KERNEL_INSTALL_PYTHON} DEF_PERMS)

SALOME_INSTALL_SCRIPTS(__init__.py ${SALOME_VIRT_PYTHON_PACKAGE} DEF_PERMS)

SALOME_CONFIGURE_FILE(version_and_config.py version_and_config.py INSTALL ${SALOME_KERNEL_INSTALL_PYTHON})
