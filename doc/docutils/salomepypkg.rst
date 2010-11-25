
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Complement A: Organizing the SALOME python functions in a packaged structure
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

This chapter contains the instruction notes to organise the python
files of SALOME in a packaged python structure. This is the first step
of the development process, whose goal is to validate the principles
and show a possible way.

:Contacts: Guillaume Boulant, Christian Caremoli, Renaud Barate

Objectives
==========

The main idea is to import SALOME python functions by doing:

.. code-block:: python
   
   from salome.kernel.<myPythonModule> import <myFunction>

instead of:

.. code-block:: python

   from <myPythonModule> import <myFunction>

as it must be done up to now because of the flat organisation of
python files in the installation folders of SALOME modules.

To reach this target, the files ``<myPythonModule>.py`` have to be
organised in a packaged python structure where the main package is
named ``salome``, and then sub-packages could be created for each
SALOME module:

* ``salome.kernel``: for kernel python functions, embedded in the
  KERNEL module
* ``salome.gui``: for gui python functions, embedded in the GUI module
* ``salome.geom``: for geom python functions, embedded in the GEOM
  module
* and so on ...

The motivations of this objective are twice:

* Definitively prevent the risk of naming conflict between python
  modules coming from different SALOME modules. Today, the developper
  of a module has to take care of the names used in other modules to
  choose a name.
* Integrate in SALOME some python modules initialy developed in the
  context of domain specific SALOME applications (SALOME-MECA,
  SALOME-CFD, OPENTURN, PANTHERE) and whose source files are organized
  in a packaged python structure.

The starting point then is a python library named ``nepal`` that
provides SALOME helper functions classified by modules
(KERNEL,GEOM,...) and organized in a packaged python structure:

* ``salome.kernel``: helper functions for manipulating the SALOME
  study and its components (SComponents and SObject). This provides
  also general purpose utilities for logging and threading.
* ``salome.gui``:  helper functions to manipulate the graphical
  representation of studies and the general behavior of the graphical
  interface. This provides also generic templates for implementing
  dialog box with the MVC pattern.
* ``salome.geom``: essentially contains a function called
  "visualization of structural elements". This is used by mechanical
  ingeneers to create the 3D geometrical object corresponding to the
  numerical model of a given structural element.
* ``salome.smesh``: to manipulated smesh data handled from the SObject
  in the SALOME study.

The target point is to have the ``salome.kernel`` part in the KERNEL
module, the ``salome.geom`` part in the GEOM module, and so on. And
with **no impact on SALOME scripts** that already exists (import salome,
and all other stuff should be imported and work as before).


Problems
========

To reach this target, we have to face two problems:

* A naming conflict with the instruction ``import salome``. The result
  is unpredictible because of the existance in the ``sys.path`` of
  both a file ``salome.py`` and a package ``salome``.
* The dispatching of ``salome.*`` sub-packages in the different SALOME
  modules.

Naming conflict between ``salome.py`` module and ``salome`` package
-------------------------------------------------------------------

The problem is solved by installing the ``salome.py`` file under the
name ``__init__.py`` in a folder named ``${salomepythondir}/salome``.

By this operation, the ``${salomepythondir}/salome`` directory is
transformed in a python package and the instruction ``import salome``
do the same things as before this modification, without any
modification of the ``sys.path``.

Dispatching of ``salome.*`` sub-packages in different SALOME modules
--------------------------------------------------------------------

When we use a SALOME virtual application, the problem is naturally
solved by the fact that every sub-packages are virtually installed in
the same directory, the directory ``${salomepythondir}/salome``
containing the file ``__init__.py``.

Nevertheless, some people doesn't use the virtual application. To get
a robust configuration in any case, one can use the python namespace
pattern. This consists in creating a virtual python package that
aggregates all the sub-packages.

Technically speaking, this consists in implementing in the file
``${salomepythondir}/salome/__init__.py`` (new version of
``salome.py``) a function that automatically extend the ``__path__``
variable with sub-packages that can be found in SALOME modules
installation paths. The code looks something like that:

.. code-block:: python
 
 import os, sys
 
 MATCH_ENDING_PATTERN="site-packages/salome"
 
 def extend_path(pname):
     for dir in sys.path:
         if not isinstance(dir, basestring) or not os.path.isdir(dir) or not dir.endswith(MATCH_ENDING_PATTERN):
             continue
         subdir = os.path.join(dir, pname)
         # WARN: This may still add duplicate entries to path on
         # case-insensitive filesystems
         if os.path.isdir(subdir) and subdir not in __path__:
             print "INFO - The directory %s is appended to sys.path" % subdir
             __path__.append(subdir)
 
 extend_path(ROOT_PYTHONPACKAGE_NAME)


Adaptation of the ``apply_gen`` utility
----------------------------------------

Due to the specific above choices, the ``apply_gen`` utility must be
modified so that the sub-folder ``salome`` in ``${salomepythondir}``
is not generated as a symbolic link any longer but as a real folder
containing symbolic links towards the module specific python
sub-packages (``kernel``, ``geom``, ``smesh``, ...) and to the single
file ``__init__.py`` provided by the KERNEL module.

This adaptation can be done in the ``virtual_salome.py`` script.


What to do with already existing python files?
----------------------------------------------

Do nothing at this step, it works fine because the files are installed
in a path included in the ``sys.path``.

In a future version, it should be nice to reverse all the python files
of the KERNEL library in this packaged structure. But this can't be
done without impact on existing python user scripts.

Instructions
============

Instructions for creating the python packages
---------------------------------------------

Considering the elements described above, a procedure that works to
get the packaged python structure is:

* Rename the file ``salome.py`` in ``__init__.py`` (and adapt the
  Makefile.am). This is located in the source directory
  ``src/KERNEL_PY``.
* Copy the sources files of the kernel part in the source directory
  ``src/KERNEL_PY`` starting with a stage named ``kernel`` including
  its own packaged structure (only python files and a file
  ``__init__.py`` for now)
* Copy the sources files of the geom part in the source directory
  ``src/GEOM_PY`` (to be created) of the GEOM module. In this case, we
  copy the python files directly in the directory (no stage named
  ``geom``, it's not required for source organisation, and will be
  created only for installation by makefile).
* Apply the same procedure for every other SALOME modules (it concerns
  only SMESH up to now).
* Apply the "namespace pattern" by implementing and invoking the
  ``extend_path`` function in the newly created file ``__init__.py``
* Adapt the ``apply_gen`` utility to take into account the finer
  folder hierarchy in ``site-packages``.

The naming convention for source folder is here the convention in
place in the KERNEL module: the source code of the python packages of
a SALOME module <MODULE_NAME> is located in the source directory
``<srcdir>/src/<MODULE_NAME>_PY``.

Note also that all python files that were existing in the KERNEL
module are leaft untouched but the file ``salome.py``.

Instructions for the associated documentation
---------------------------------------------

One special point for the documentation:

* The documentation of the python package API is writen in rst
  (restructured text) and generated form the source code with sphinx.
* The rst source files are located in the directory
  ``<srcdir>/doc/docutils``.
* The html generated files are installed in the directory
  ``<installdir>/share/doc/salome/docutils`` but are not connected to
  the in-line documentation of the SALOME associated module (menu help
  of the SALOME application).

Any suggestion on this point would be appreciated.

TODO (by someone):

* Move all files ``*.txt`` from the ``<srcdir>/doc`` folder to the
  ``<srcdir>/doc/docutils`` folder and analyse what is still to date
  and usefull.
* Integrate in this part the reference documentation of the ``salome``
  utility and all documentation associated to the launching process
  (in particular virtual application)
* Connect this part of the documentation to the main part (doxygen
  part).


Synthesis
---------

Finaly, here is a synthesis of modifications in source files.

Files modified:

* See the CVS patch files KERNEL.patch, GEOM.patch and SMESH.patch
  (the only SALOME modules modified today).

Files to be added:

* KERNEL: file ``src/KERNEL_PY/__init__.py`` (``salome.py`` renamed)
* KERNEL: directory ``src/KERNEL_PY/kernel``
* KERNEL: directory ``doc/docutils``
* KERNEL: file ``salome_adm/unix/config_files/check_sphinx.m4``
* GEOM  : directory ``src/GEOM_PY``
* GEOM  : directory ``doc/docutils``
* SMESH : directory ``src/SMESH_PY``
* SMESH : directory ``doc/docutils``

Files to be delete:

* file ``src/KERNEL_PY/salome.py``


Tests and usage
===============

The instructions above provides you with a SALOME application whose
modules embed there dedicated python packages. This installation can
can be tested using some test use cases. For example, the
visualisation of structural elements (provided by the package
``salome.geom`` can be tested by:

.. code-block:: python

   from salome.geom.structelem import TEST_StructuralElement
   TEST_StructuralElement()

This can be enter in the GUI python console or in a python interpreter
executed in a SALOME session.

For more details, read the API documentation in
``<installdir>/share/doc/salome/docutils``.
