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

ADD_SUBDIRECTORY(kernel)

SET(salomepython_PYTHON
  omnipatch.py
)

SET(salomepython_PYTHON
    ${salomepython_PYTHON}
    Help.py
    PyInterp.py
    batchmode_salome.py
    salome_test.py
    salome_kernel.py
    salome_study.py
    salome_iapp.py
    salome_ComponentGUI.py
    iparameters.py
    salome_version.py
    salome_notebook.py
    salome_pynode.py
    salome_genericobj.py
  )

SALOME_INSTALL_SCRIPTS("${salomepython_PYTHON}" ${SALOME_INSTALL_PYTHON})
SALOME_INSTALL_SCRIPTS(__init__.py ${SALOME_INSTALL_PYTHON}/salome)
