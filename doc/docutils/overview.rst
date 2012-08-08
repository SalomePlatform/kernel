
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
General presentation of the KERNEL python package
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

The KERNEL python package essentially contains:

* Helper functions to manipulate KERNEL objects from python. For
  example, the ``studyedit.py`` module facilitates the
  manipulation of components and items in SALOME study.
* General purpose functions for logging and other recurrent
  stuff in python programming.

Note that these functions either encapsulate the python programming
interface of KERNEL core (the CORBA or SWIG interfaces for example) or
extend existing utilities as the ``salome*.py`` modules.

The functions are distributed in the python package
``salome.kernel``. For example, the usage of the study editor to
manipulate some objects can be done with a set of instructions as:

.. code-block:: python
 
 from salome.kernel.studyedit import getStudyEditor

 studyEditor = getStudyEditor()  # Get an editor for the current study
 
 myStudyComponent = studyEditor.findOrCreateComponent(
        moduleName,
        componentName,
        componentIcon)

 myStudyItem = studyEditor.createItem(
        myStudyComponent,
        itemName,
        comment = itemComment,
        icon = itemIcon)

The specification of the programming interface of this package is
detailed in the part :doc:`Documentation of the programming interface
(API)</docapi>` of this documentation.

.. note::
   The main package ``salome`` contains other sub-packages that are
   distributed with the other SALOME modules. For example, the GEOM
   module provides the python package ``salome.geom`` and SMESH the
   package ``salome.smesh``.
