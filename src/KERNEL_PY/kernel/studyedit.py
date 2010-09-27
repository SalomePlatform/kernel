# -*- coding: utf-8 -*-
#
#  Copyright (C) 2007-2009	 EDF R&D
# 
#    This file is part of PAL_SRC.
#
#    PAL_SRC is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    PAL_SRC is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with PAL_SRC; if not, write to the Free Software
#    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA
#
"""
This module provides a new class :class:`StudyEditor` to complement
:class:`Study` and :class:`StudyBuilder` classes.
"""

import re

import salome
from salome.kernel.logger import Logger
from salome.kernel import termcolor
logger = Logger("salome.kernel.studyedit", color = termcolor.PURPLE)

_editors = {}
_DEFAULT_CONTAINER = "FactoryServer"

def getActiveStudyId():
    """
    Return the ID of the active study. In GUI mode, this function is equivalent
    to ``salome.sg.getActiveStudyId()``. Outside GUI, it returns
    ``salome.myStudyId`` variable.
    """
    salome.salome_init()
    if salome.hasDesktop():
        return salome.sg.getActiveStudyId()
    else:
        return salome.myStudyId

def getStudyEditor(studyId = None):
    """
    Return a :class:`StudyEditor` instance to edit the study with ID
    `studyId`. If `studyId` is :const:`None`, return an editor for the current
    study.
    """
    if studyId is None:
        studyId = getActiveStudyId()
    if not _editors.has_key(studyId):
        _editors[studyId] = StudyEditor(studyId)
    return _editors[studyId]


class StudyEditor:
    """
    This class provides utility methods to complement :class:`Study` and
    :class:`StudyBuilder` classes. Those methods may be moved in those classes
    in the future. The parameter `studyId` defines the ID of the study to
    edit. If it is :const:`None`, the edited study will be the current study.
    The preferred way to get a StudyEditor object is through the method
    :meth:`getStudyEditor` which allows to reuse existing instances.

    .. attribute:: studyId
    
       This instance attribute contains the ID of the edited study. This
       attribute should not be modified.

    .. attribute:: study
    
       This instance attribute contains the underlying :class:`Study` object.
       It can be used to access the study but the attribute itself should not
       be modified.

    .. attribute:: builder

       This instance attribute contains the underlying :class:`StudyBuilder`
       object. It can be used to edit the study but the attribute itself
       should not be modified.

    """
    def __init__(self, studyId = None):
        salome.salome_init()
        if studyId is None:
            studyId = getActiveStudyId()
        self.studyId = studyId
        self.study = salome.myStudyManager.GetStudyByID(studyId)
        if self.study is None:
            raise Exception("Can't create StudyEditor object: "
                            "Study %d doesn't exist" % studyId)
        self.builder = self.study.NewBuilder()

    def findOrCreateComponent(self, componentName, componentLabel=None, icon=None,
                              loadEngine = True, engineName = None,
                              containerName = _DEFAULT_CONTAINER):
        """
        Find a component corresponding to the specified name `componentName` in the
        study, or create it if none is found. Then eventually load the
        corresponding engine and the CORBA objects of this component.

        :type  componentName: string
        :param componentName: name of the new component if created. The name is
                              a string used to identify the study component.
                              Generally, it corresponds to the name of the SALOME
                              module to be associated to this component.

        :type  componentLabel: string
        :param componentLabel: label of the new component if created. The label
                               is the string used in the object browser.
                               (attribute "AttributeName")

        :type  icon: string
        :param icon: icon for the new component (attribute "AttributePixMap").

        :type  loadEngine: boolean
        :param loadEngine: If :const:`True`, find or load the corresponding
                           engine and associate it with the component. Default
                           is True.

        :type  engineName: string
        :param engineName: name of the engine to be associated to the component.
                           If :const:`None`, use `componentName` instead because
                           in most of the SALOME modules, the names are identical.

        :type  containerName: string
        :param containerName: name of the container in which the engine should be
                              loaded.

        :return: the SComponent found or created.

        """
        sComponent = self.study.FindComponent(componentName)
        if sComponent is None:
            sComponent = self.builder.NewComponent(componentName)
            # Note that the NewComponent method set the "comment" attribute to the
            # value of its argument (componentName here)
            self.builder.SetName(sComponent, componentLabel)
            if icon is not None:
                # _MEM_ : This will be effective if and only if the componentName
                # corresponds to the module name (as specified in the SalomeApp.xml)
                self.setIcon(sComponent, icon)

            if engineName is None:
                engineName = componentName

            engine = salome.lcc.FindOrLoadComponent(containerName,engineName)
            self.builder.DefineComponentInstance(sComponent, engine)

            # An internal convention (in this class) is to store the name of the
            # associated engine in the parameter attribute of the scomponent (so that
            # it could be retrieved in a future usage of this scomponent, for example,
            # for the need of the function loadComponentEngine). The comment attribute
            # SHOULD NOT be used for this purpose  because it's used by the SALOME
            # resources manager to identify the SALOME module and then localized
            # the resource files
            attr = self.builder.FindOrCreateAttribute( sComponent, "AttributeParameter" )
            attr.SetString( "ENGINE_NAME", engineName )

            if loadEngine:
                self.loadComponentEngine(sComponent, containerName)

        return sComponent

    def loadComponentEngine(self, sComponent,
                            containerName = _DEFAULT_CONTAINER):
        """
        Load the engine corresponding to `sComponent` in the container
        `containerName`, associate the engine with the component and load the
        CORBA objects of this component in the study.
        """
        attr = self.builder.FindOrCreateAttribute( sComponent, "AttributeParameter" )
        engineName = attr.GetString( "ENGINE_NAME" )
        engine = salome.lcc.FindOrLoadComponent(containerName,
                                                engineName)
        self.builder.LoadWith(sComponent, engine)

    def getOrLoadObject(self, item):
        """
        Get the CORBA object associated with the SObject `item`, eventually by
        first loading it with the corresponding engine.
        """
        object = item.GetObject()
        if object is None: # the engine has not been loaded yet
            sComponent = item.GetFatherComponent()
            self.loadComponentEngine(sComponent)
            object = item.GetObject()
        return object

    def findOrCreateItem(self, fatherItem, name, fileType = None,
                         fileName = None, comment = None, icon = None,
                         IOR = None, typeId = None):
        """
        Find an object under `fatherItem` in the study with the given
        attributes. Return the first one found if at least one exists,
        otherwise create a new one with the given attributes and return it.
        
        See :meth:`setItem` for the description of the parameters.
        """
        sObject = self.findItem(fatherItem, name, fileType, fileName, comment,
                                icon, IOR, typeId)
        if sObject is None:
            sObject = self.createItem(fatherItem, name, fileType, fileName,
                                      comment, icon, IOR, typeId)
        return sObject

    def findItem(self, fatherItem, name = None, fileType = None,
                 fileName = None, comment = None, icon = None, IOR = None,
                 typeId = None):
        """
        Find an item with given attributes under `fatherItem` in the study. If
        none is found, return :const:`None`. If several items correspond to
        the parameters, only the first one is returned. The search is made
        only on given parameters (i.e. not :const:`None`). To look explicitly
        for an empty attribute, use an empty string in the corresponding
        parameter.
        
        See :meth:`setItem` for the description of the parameters.
        """
        foundItem = None;
        childIterator = self.study.NewChildIterator(fatherItem)
        while childIterator.More() and foundItem is None:
            childItem = childIterator.Value()
            if childItem and \
               (name is None or childItem.GetName() == name) and \
               (fileType is None or \
                self.getFileType(childItem) == fileType) and \
               (fileName is None or \
                self.getFileName(childItem) == fileName) and \
               (comment is None or childItem.GetComment() == comment) and \
               (icon is None or \
                self.getIcon(childItem) == icon) and \
               (IOR is None or childItem.GetIOR() == IOR and \
               (typeId is None or \
                self.getTypeId(childItem) == typeId)):
                foundItem = childItem
            childIterator.Next()
        return foundItem

    def createItem(self, fatherItem, name, fileType = None, fileName = None,
                   comment = None, icon = None, IOR = None, typeId = None):
        """
        Create a new object named `name` under `fatherItem` in the study, with
        the given attributes. If an object named `name` already exists under
        the father object, the new object is created with a new name `name_X`
        where X is the first available index.
        
        :type  fatherItem: SObject
        :param fatherItem: item under which the new item will be added.
                
        :return: new SObject created in the study
        
        See :meth:`setItem` for the description of the other parameters.
        """
        aSObject = self.builder.NewObject(fatherItem)

        aChildIterator = self.study.NewChildIterator(fatherItem)
        aMaxId = -1
        aLength = len(name)
        aDelim = "_"
        anIdRE = re.compile("^" + aDelim + "[0-9]+")
        aNameRE = re.compile("^" + name)
        while aChildIterator.More():
            aSObj = aChildIterator.Value()
            aChildIterator.Next()
            aName = aSObj.GetName()
            if re.match(aNameRE,aName):
                aTmp = aName[aLength:]
                if re.match(anIdRE,aTmp):
                    import string
                    anId = string.atol(aTmp[1:])
                    if aMaxId < anId:
                        aMaxId = anId
                        pass
                    pass
                elif aMaxId < 0:
                    aMaxId = 0
                    pass
                pass
            pass
        
        aMaxId = aMaxId + 1
        aName = name
        if aMaxId > 0:
            aName = aName + aDelim + str(aMaxId)
            pass
        
        self.setItem(aSObject, aName, fileType, fileName, comment, icon,
                     IOR, typeId)
    
        return aSObject

    def setItem(self, item, name = None, fileType = None, fileName = None,
                comment = None, icon = None, IOR = None, typeId = None):
        """
        Modify the attributes of an item in the study. Unspecified attributes
        (i.e. those set to :const:`None`) are left unchanged.

        :type  item: SObject
        :param item: item to modify.

        :type  name: string
        :param name: item name (attribute 'AttributeName').

        :type  fileType: string
        :param fileType: item file type (attribute 'AttributeFileType').

        :type  fileName: string
        :param fileName: item file name (attribute
                         'AttributeExternalFileDef').

        :type  comment: string
        :param comment: item comment (attribute 'AttributeComment'). Note that
                        this attribute will appear in the 'Value' column in
                        the object browser.

        :type  icon: string
        :param icon: item icon name (attribute 'AttributePixMap').

        :type  IOR: string
        :param IOR: IOR of a CORBA object associated with the item
                    (attribute 'AttributeIOR').

        :type  typeId: integer
        :param typeId: item type (attribute 'AttributeLocalID').
        """
        logger.debug("setItem (ID=%s): name=%s, fileType=%s, fileName=%s, "
                     "comment=%s, icon=%s, IOR=%s" %
                     (item.GetID(), name, fileType, fileName, comment,
                      icon, IOR))
        # Explicit cast is necessary for unicode to string conversion
        if name is not None:
            self.builder.SetName(item, str(name))
        if fileType is not None:
            self.setFileType(item, fileType)
        if fileName is not None:
            self.setFileName(item, fileName)
        if comment is not None:
            self.builder.SetComment(item, str(comment))
        if icon is not None:
            self.setIcon(item, icon)
        if IOR is not None:
            self.builder.SetIOR(item, str(IOR))
        if typeId is not None:
            self.setTypeId(item, typeId)

    def removeItem(self, item, withChildren = False ):
        """
        Remove the given item from the study (the item still is in
        the study after the removal)
        @param   item: the browser object to be removed
        @param   withChildren: remove children if True
        """
        ok = False
        try:
            if withChildren:
                self.builder.RemoveObjectWithChildren(item)
            else:
                self.builder.RemoveObject(item)
            ok = True
        except:
            ok = False

        return ok

    def setItemAtTag(self, fatherItem, tag, name = None, fileType = None,
                     fileName = None, comment = None, icon = None, IOR = None,
                     typeId = None):
        """
        Find an item tagged `tag` under `fatherItem` in the study tree or
        create it if there is none, then set its attributes.
        
        :type  fatherItem: SObject
        :param fatherItem: item under which the tagged item will be looked for
                           and eventually created.

        :type  tag: integer
        :param tag: tag of the item to look for.

        :return: the SObject at `tag` if found or created successfully, or
                 :const:`None` if an error happened.
        
        See :meth:`setItem` for the description of the other parameters.
        """
        found, sObj = fatherItem.FindSubObject(tag)
        if not found:
            sObj = self.builder.NewObjectToTag(fatherItem, tag)
        self.setItem(sObj, name, fileType, fileName, comment, icon,
                     IOR, typeId)
        return sObj

    def getAttributeValue(self, sObject, attributeName, default = None):
        """
        Return the value of the attribute named `attributeName` on the object
        `sObject`, or `default` if the attribute doesn't exist.
        """
        value = default
        found, attr = self.builder.FindAttribute(sObject, attributeName)
        if found:
            value = attr.Value()
        return value

    def setAttributeValue(self, sObject, attributeName, attributeValue):
        """
        Set the value of the attribute named `attributeName` on the object
        `sObject` to the value `attributeValue`.
        """        
        attr = self.builder.FindOrCreateAttribute(sObject, attributeName)
        attr.SetValue(attributeValue)

    def getTypeId(self, sObject):
        """
        Return the value of the attribute "AttributeLocalID" of the object
        `sObject`, or :const:`None` if it is not set.
        """
        return self.getAttributeValue(sObject, "AttributeLocalID")

    def setTypeId(self, sObject, value):
        """
        Set the attribute "AttributeLocalID" of the object `sObject` to the
        value `value`.
        """
        self.setAttributeValue(sObject, "AttributeLocalID", value)

    def getFileType(self, sObject):
        """
        Return the value of the attribute "AttributeFileType" of the object
        `sObject`, or an empty string if it is not set.
        """
        return self.getAttributeValue(sObject, "AttributeFileType", "")

    def setFileType(self, sObject, value):
        """
        Set the attribute "AttributeFileType" of the object `sObject` to the
        value `value`.
        """
        # Explicit cast is necessary for unicode to string conversion
        self.setAttributeValue(sObject, "AttributeFileType", str(value))

    def getFileName(self, sObject):
        """
        Return the value of the attribute "AttributeExternalFileDef" of the
        object `sObject`, or an empty string if it is not set.
        """
        return self.getAttributeValue(sObject, "AttributeExternalFileDef", "")

    def setFileName(self, sObject, value):
        """
        Set the attribute "AttributeExternalFileDef" of the object `sObject`
        to the value `value`.
        """
        # Explicit cast is necessary for unicode to string conversion
        self.setAttributeValue(sObject, "AttributeExternalFileDef",
                               str(value))

    def getIcon(self, sObject):
        """
        Return the value of the attribute "AttributePixMap" of the object
        `sObject`, or an empty string if it is not set.
        """
        value = ""
        found, attr = self.builder.FindAttribute(sObject, "AttributePixMap")
        if found and attr.HasPixMap():
            value = attr.GetPixMap()
        return value

    def setIcon(self, sObject, value):
        """
        Set the attribute "AttributePixMap" of the object `sObject` to the
        value `value`.
        """
        attr = self.builder.FindOrCreateAttribute(sObject, "AttributePixMap")
        # Explicit cast is necessary for unicode to string conversion
        attr.SetPixMap(str(value))
