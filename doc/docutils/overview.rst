
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
General presentation of the KERNEL python package
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

The KERNEL python package essentially contains:

* Helper functions to manipulate KERNEL objects from python. For
  example, the ``studyedit.py`` module makes you ease with the
  manipulation of the SALOME study, its ``SComponent`` and
  ``SObject``.
* General purpose functions for logging, threading, and other recurent
  stuff in python programming. These function has no dependancy with
  SALOME and could be used from any python interpreter

Note that these functions either encapsulate the python programming
interface of KERNEL core (the CORBA or SWIG interfaces for example) or
extend existing utilities as the ``salome*.py`` modules.

The functions are distributed in the python package
``salome.kernel``. For example, the usage of the study editor to
manipulate some objects can be done with a set of instructions as:

.. code-block:: python
 
 import salome
 salome.salome_init()
 salomeStudyId = salome.myStudyId

 from salome.kernel import studyedit 
 studyEditor = studyedit.getStudyEditor(salomeStudyId)
 
 myStudyComponent = studyEditor.findOrCreateComponent(
        componentName,
        componentLabel,
        componentIcon,
        engineName)

 myStudyItem = studyEditor.createItem(
        myStudyComponent,
        itemName,
        itemComment,
        itemType,
        itemIcon)

The specification of the programming interface of this package is
detailled in the part :doc:`Documentation of the programming interface
(API)</docapi>` of this documentation.

.. note::
   The main package ``salome`` contains other sub-packages that are
   distributed with the other SALOME modules. For example, the GEOM
   module provides the python package ``salome.geom`` and SMESH the
   package ``salome.smesh``.
