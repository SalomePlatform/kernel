#  -*- coding: iso-8859-1 -*-
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
import re

# ----
# A set of regular expressions used ...
# ----

p_multiline = re.compile(r"""
\\           # One backslash
\s*          # 0 or more space
\n           # One CR
""", re.VERBOSE)

p_dollar = re.compile(r"""
\$\(           # a $ then a (
(?P<val>       # open the group val
[^)]*          # the group contain 0 or more non ) characters
)              # close the group
\)             # a ) at the end
""", re.VERBOSE)

p_arobas = re.compile(r"""
@              # a @
(?P<val>       # open the group val
[^@]*            # the group contain 0 or more non @ characters
)              # close the group
@              # a @ at the end
""", re.VERBOSE)

p_if = re.compile(r"""
^          # beginning of the string
\s*        # 0 or more space
if         # an if
\s+        # 1 or more space
(?P<val>   # open the group val
[^\s]+     # the group contain 1 or more non space characters
)          # close the group
""", re.VERBOSE)

p_else = re.compile(r"""
^          # beginning of the line
\s*        # 0 or more space
else       #
\s*        # 0 or more space
""", re.VERBOSE)

p_endif = re.compile(r"""
^          # beginning of the line
\s*        # 0 or more space
endif      # a endif
\s*        # 0 or more space
""", re.VERBOSE)

p_attribution = re.compile(r"""
^              # beginning of the line
(?P<spaces>    # open the group spaces
\s*            # 0 or more space
)              # close the group
(?P<key>       # open the group key
\w+            # the group contain 1 or more alphanumeric characters
)              # close the group
\s*            # 0 or more space
(?P<method>    # open the group method
\+?            # 0 or 1 +
=              # One =
)              # close the group
(?P<value>     # open the group value
.*             # 0 or more any characters 
)              # close the group
""", re.VERBOSE)

# -----

class CMakeFile(object):
    
    def __init__(self, the_root, root, dirs, files, f, module):
        #
        self.the_root = the_root
        self.root = root
        self.dirs = dirs
        self.files = files
        self.module = module
        #
        from os.path import join
        self.amFile = join(root, f)
        self.listsFile = join(root, "CMakeLists.txt")
        #
        self.parseFile()
        #
        return
    
    def parseFile(self):
        
        # --
        # Read the Makefile.am file
        # --
        amFile = self.amFile
        stream = open(amFile)
        content = stream.read()
        stream.close()
        
        # --
        # Replace the composed lines separated by "\\n" by an unique line
        # --
        content = p_multiline.sub(r' ', content)
        
        # --
        # Compatibility netgen plugin
        # --
        content = content.replace("../NETGEN/libNETGEN.la", "${NETGEN_LIBS}")
        
        # --
        cas_list = [
            "BinLPlugin",
            "BinPlugin",
            "BinTObjPlugin",
            "BinXCAFPlugin",
            "FWOSPlugin",
            "PTKernel",
            "StdLPlugin",
            "StdPlugin",
            "TKAdvTools",
            "TKBin",
            "TKBinL",
            "TKBinTObj",
            "TKBinXCAF",
            "TKBO",
            "TKBool",
            "TKBRep",
            "TKCAF",
            "TKCDF",
            "TKernel",
            "TKFeat",
            "TKFillet",
            "TKG2d",
            "TKG3d",
            "TKGeomAlgo",
            "TKGeomBase",
            "TKHLR",
            "TKIGES",
            "TKLCAF",
            "TKMath",
            "TKMesh",
            "TKMeshVS",
            "TKNIS",
            "TKOffset",
            "TKOpenGl",
            "TKPCAF",
            "TKPLCAF",
            "TKPrim",
            "TKPShape",
            "TKService",
            "TKShapeSchema",
            "TKShHealing",
            "TKStdLSchema",
            "TKStdSchema",
            "TKSTEP",
            "TKSTEP209",
            "TKSTEPAttr",
            "TKSTEPBase",
            "TKSTL",
            "TKTObj",
            "TKTopAlgo",
            "TKV2d",
            "TKV3d",
            "TKVRML",
            "TKXCAF",
            "TKXCAFSchema",
            "TKXDEIGES",
            "TKXDESTEP",
            "TKXMesh",
            "TKXml",
            "TKXmlL",
            "TKXmlTObj",
            "TKXmlXCAF",
            "TKXSBase",
            "XCAFPlugin",
            "XmlLPlugin",
            "XmlPlugin",
            "XmlTObjPlugin",
            "XmlXCAFPlugin",
            ]
        vtk_list = [
            "vtkCommonPythonD",
            "vtkGraphicsPythonD",
            "vtkImagingPythonD",
            ]
        kernel_list  = [
            "DF",
            "Launcher",
            "OpUtil",
            "Registry",
            "ResourcesManager",
            "SALOMEBasics",
            "SalomeBatch",
            "SalomeCatalog",
            "SalomeCommunication",
            "SalomeContainer",
            "SalomeDSCContainer",
            "SalomeDSClient",
            "SalomeDSImpl",
            "SalomeDS",
            "SalomeGenericObj",
            "SalomeHDFPersist",
            "SalomeIDLKernel",
            "SalomeLauncher",
            "SalomeLifeCycleCORBA",
            "SALOMELocalTrace",
            "SalomeLoggerServer",
            "SalomeNotification",
            "SalomeNS",
            "SalomeResourcesManager",
            "TOOLSDS",
            "with_loggerTraceCollector",
            ]
        gui_list = [
            "caf",
            "CAM",
            "CASCatch",
            "DDS",
            "Event",
            "GLViewer",
            "LightApp",
            "LogWindow",
            "ObjBrowser",
            "OCCViewer",
            "Plot2d",
            "PyConsole",
            "PyInterp",
            "QDS",
            "qtx",
            "QxScene",
            "SalomeApp",
            "SalomeIDLGUI",
            "SalomeObject",
            "SalomePrs",
            "SalomeSession",
            "SalomeStyle",
            "SOCC",
            "SPlot2d",
            "std",
            "SUITApp",
            "suit",
            "SUPERVGraph",
            "SVTK",
            "ToolsGUI",
            "VTKViewer",
            ]
        geom_list = [
            "BasicGUI",
            "BlocksGUI",
            "BooleanGUI",
            "BREPExport",
            "BREPImport",
            "BuildGUI",
            "DisplayGUI",
            "DlgRef",
            "EntityGUI",
            "GenerationGUI",
            "GEOMAlgo",
            "GEOMArchimede",
            "GEOMBase",
            "GEOMbasic",
            "GEOMClient",
            "GEOMEngine",
            "GEOMFiltersSelection",
            "GEOMimpl",
            "GEOMObject",
            "GEOMSketcher",
            "GEOM",
            "GEOM_SupervEngine",
            "GEOMToolsGUI",
            "GroupGUI",
            "IGESExport",
            "IGESImport",
            "MeasureGUI",
            "NMTDS",
            "NMTTools",
            "OperationGUI",
            "PrimitiveGUI",
            "RepairGUI",
            "SalomeIDLGEOM",
            "ShHealOper",
            "STEPExport",
            "STEPImport",
            "STLExport",
            "TransformationGUI",
            ]
        med_list = [
            "InterpGeometric2DAlg",
            "interpkernelbases",
            "interpkernel",
            "MEDClientcmodule",
            "medcoupling",
            "MEDEngine",
            "MEDMEMImpl",
            "medmem",
            "MED",
            "med_V2_1",
            "MEDWrapperBase",
            "MEDWrapper",
            "MEDWrapper_V2_1",
            "MEDWrapper_V2_2",
            "SalomeIDLMED",
            ]
        smesh_list = [
            "MEFISTO2D",
            "MeshDriverDAT",
            "MeshDriverMED",
            "MeshDriver",
            "MeshDriverSTL",
            "MeshDriverUNV",
            "SalomeIDLSMESH",
            "SMDS",
            "SMESHClient",
            "SMESHControls",
            "SMESHDS",
            "SMESHEngine",
            "SMESHFiltersSelection",
            "SMESHimpl",
            "SMESHObject",
            "SMESH",
            "StdMeshersEngine",
            "StdMeshersGUI",
            "StdMeshers",
            ]
        full_list  = cas_list + vtk_list
        full_list += kernel_list + gui_list
        full_list += geom_list + med_list + smesh_list
        # --
        # E.A. : sort by len before substitution ...
        # Why ? Thing to "-lMEDWrapper" then "-lMEDWrapper_V2_1" substition
        # And you understand ...
        # --
        full_list.sort(cmp = lambda x, y : cmp(len(y), len(x)))
        # --
        for key in full_list:
            content = content.replace("-l%s"%(key), "${%s}"%(key))
            pass
        
        # --
        # Split content in lines to treat each one separately
        # --
        lines = content.split('\n')
        
        # --
        # newlines contains the lines of the future CMakeLists.txt file
        # --
        newlines = []
        
        # --
        # opened_ifs is used to deals with nested conditionnals
        # --
        opened_ifs = []
        
        # --
        # the __thedict__ dictionnary contains key, val
        # of the Makefile.am file
        # --
        self.__thedict__ = {}
        
        # --
        # Initialize file ... mainly includes other cmake files
        # --
        self.initialize(newlines)
        
        # --
        # Do the job for each line
        # --
        for line in lines:
            self.treatLine(line, newlines, opened_ifs)
            pass
        
        # --
        # Finalize file ... it is in here the cmake job is done
        # --
        self.finalize(newlines)
        
        # --
        # Concatenate newlines into content
        # --
        content = '\n'.join(newlines)
        
        # --
        # Add a CR at end if necessary
        # --
        lines = content.split('\n')
        # lines = [ l.strip() for l in lines ]
        if len(lines[-1]) != 0:
            lines.append('')
            pass
        content = '\n'.join(lines)
        
        # --
        self.content = content
        
        # --
        return
    
    def initialize(self, newlines):
        if self.root == self.the_root:
            # --
            newlines.append("""
            CMAKE_MINIMUM_REQUIRED(VERSION 2.4.7 FATAL_ERROR)
            IF(COMMAND cmake_policy)
            cmake_policy(SET CMP0003 NEW)
            ENDIF(COMMAND cmake_policy)
            """)
            # --
            if self.module == "kernel":
                newlines.append("""
                INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindPLATFORM.cmake)
                INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindPYTHON.cmake)
                INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindOMNIORB.cmake)
                INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindPTHREADS.cmake)
                INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindHDF5.cmake)
                INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindBOOST.cmake)
                INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindLIBXML2.cmake)
                INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindSWIG.cmake)
                INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindCPPUNIT.cmake)
                INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindDOXYGEN.cmake)
                """)
                pass
            else:
                newlines.append("""
                SET(KERNEL_ROOT_DIR $ENV{KERNEL_ROOT_DIR})
                INCLUDE(${KERNEL_ROOT_DIR}/salome_adm/cmake_files/FindPLATFORM.cmake)
                INCLUDE(${KERNEL_ROOT_DIR}/salome_adm/cmake_files/FindPYTHON.cmake)
                INCLUDE(${KERNEL_ROOT_DIR}/salome_adm/cmake_files/FindOMNIORB.cmake)
                INCLUDE(${KERNEL_ROOT_DIR}/salome_adm/cmake_files/FindPTHREADS.cmake)
                INCLUDE(${KERNEL_ROOT_DIR}/salome_adm/cmake_files/FindHDF5.cmake)
                INCLUDE(${KERNEL_ROOT_DIR}/salome_adm/cmake_files/FindBOOST.cmake)
                INCLUDE(${KERNEL_ROOT_DIR}/salome_adm/cmake_files/FindLIBXML2.cmake)
                INCLUDE(${KERNEL_ROOT_DIR}/salome_adm/cmake_files/FindSWIG.cmake)
                INCLUDE(${KERNEL_ROOT_DIR}/salome_adm/cmake_files/FindCPPUNIT.cmake)
                INCLUDE(${KERNEL_ROOT_DIR}/salome_adm/cmake_files/FindDOXYGEN.cmake)
                INCLUDE(${KERNEL_ROOT_DIR}/salome_adm/cmake_files/FindKERNEL.cmake)
                """)
                if self.module == "gui":
                    newlines.append("""
                    INCLUDE(${CMAKE_SOURCE_DIR}/adm_local/cmake_files/FindCAS.cmake)
                    INCLUDE(${CMAKE_SOURCE_DIR}/adm_local/cmake_files/FindQT4.cmake)
                    INCLUDE(${CMAKE_SOURCE_DIR}/adm_local/cmake_files/FindOPENGL.cmake)
                    INCLUDE(${CMAKE_SOURCE_DIR}/adm_local/cmake_files/FindVTK.cmake)
                    INCLUDE(${CMAKE_SOURCE_DIR}/adm_local/cmake_files/FindQWT.cmake)
                    INCLUDE(${CMAKE_SOURCE_DIR}/adm_local/cmake_files/FindSIPPYQT.cmake)
                    """)
                else:
                    newlines.append("""
                    SET(GUI_ROOT_DIR $ENV{GUI_ROOT_DIR})
                    INCLUDE(${GUI_ROOT_DIR}/adm_local/cmake_files/FindCAS.cmake)
                    INCLUDE(${GUI_ROOT_DIR}/adm_local/cmake_files/FindQT4.cmake)
                    INCLUDE(${GUI_ROOT_DIR}/adm_local/cmake_files/FindOPENGL.cmake)
                    INCLUDE(${GUI_ROOT_DIR}/adm_local/cmake_files/FindVTK.cmake)
                    INCLUDE(${GUI_ROOT_DIR}/adm_local/cmake_files/FindQWT.cmake)
                    INCLUDE(${GUI_ROOT_DIR}/adm_local/cmake_files/FindSIPPYQT.cmake)
                    INCLUDE(${GUI_ROOT_DIR}/adm_local/cmake_files/FindGUI.cmake)
                    """)
                    if self.module == "med":
                        newlines.append("""
                        INCLUDE(${CMAKE_SOURCE_DIR}/adm_local/cmake_files/FindMEDFILE.cmake)
                        """)
                        pass
                    if self.module == "smesh":
                        newlines.append("""
                        SET(GEOM_ROOT_DIR $ENV{GEOM_ROOT_DIR})
                        SET(MED_ROOT_DIR $ENV{MED_ROOT_DIR})
                        INCLUDE(${GEOM_ROOT_DIR}/adm_local/cmake_files/FindGEOM.cmake)
                        INCLUDE(${MED_ROOT_DIR}/adm_local/cmake_files/FindMEDFILE.cmake)
                        INCLUDE(${MED_ROOT_DIR}/adm_local/cmake_files/FindMED.cmake)
                        """)
                        pass
                    if self.module == "netgenplugin":
                        newlines.append("""
                        SET(GEOM_ROOT_DIR $ENV{GEOM_ROOT_DIR})
                        SET(MED_ROOT_DIR $ENV{MED_ROOT_DIR})
                        SET(SMESH_ROOT_DIR $ENV{SMESH_ROOT_DIR})
                        INCLUDE(${GEOM_ROOT_DIR}/adm_local/cmake_files/FindGEOM.cmake)
                        INCLUDE(${MED_ROOT_DIR}/adm_local/cmake_files/FindMED.cmake)
                        INCLUDE(${SMESH_ROOT_DIR}/adm_local/cmake_files/FindSMESH.cmake)
                        INCLUDE(${CMAKE_SOURCE_DIR}/adm_local/cmake_files/FindNETGEN.cmake)
                        """)
                        pass
                    if self.module == "blsurfplugin":
                        newlines.append("""
                        SET(GEOM_ROOT_DIR $ENV{GEOM_ROOT_DIR})
                        SET(MED_ROOT_DIR $ENV{MED_ROOT_DIR})
                        SET(SMESH_ROOT_DIR $ENV{SMESH_ROOT_DIR})
                        INCLUDE(${GEOM_ROOT_DIR}/adm_local/cmake_files/FindGEOM.cmake)
                        INCLUDE(${MED_ROOT_DIR}/adm_local/cmake_files/FindMED.cmake)
                        INCLUDE(${SMESH_ROOT_DIR}/adm_local/cmake_files/FindSMESH.cmake)
                        INCLUDE(${CMAKE_SOURCE_DIR}/adm_local/cmake_files/FindBLSURF.cmake)
                        """)
                        pass
                    if self.module == "hexoticplugin":
                        newlines.append("""
                        SET(GEOM_ROOT_DIR $ENV{GEOM_ROOT_DIR})
                        SET(MED_ROOT_DIR $ENV{MED_ROOT_DIR})
                        SET(SMESH_ROOT_DIR $ENV{SMESH_ROOT_DIR})
                        INCLUDE(${GEOM_ROOT_DIR}/adm_local/cmake_files/FindGEOM.cmake)
                        INCLUDE(${MED_ROOT_DIR}/adm_local/cmake_files/FindMED.cmake)
                        INCLUDE(${SMESH_ROOT_DIR}/adm_local/cmake_files/FindSMESH.cmake)
                        """)
                        pass
                    if self.module == "ghs3dplugin":
                        newlines.append("""
                        SET(GEOM_ROOT_DIR $ENV{GEOM_ROOT_DIR})
                        SET(MED_ROOT_DIR $ENV{MED_ROOT_DIR})
                        SET(SMESH_ROOT_DIR $ENV{SMESH_ROOT_DIR})
                        INCLUDE(${GEOM_ROOT_DIR}/adm_local/cmake_files/FindGEOM.cmake)
                        INCLUDE(${MED_ROOT_DIR}/adm_local/cmake_files/FindMED.cmake)
                        INCLUDE(${SMESH_ROOT_DIR}/adm_local/cmake_files/FindSMESH.cmake)
                        """)
                        pass
                    if self.module == "visu":
                        newlines.append("""
                        SET(MED_ROOT_DIR $ENV{MED_ROOT_DIR})
                        INCLUDE(${MED_ROOT_DIR}/adm_local/cmake_files/FindMED.cmake)
                        """)
                        pass
                    if self.module == "yacs":
                        newlines.append("""
                        INCLUDE(${CMAKE_SOURCE_DIR}/adm/cmake/FindEXPAT.cmake)
                        INCLUDE(${CMAKE_SOURCE_DIR}/adm/cmake/FindGRAPHVIZ.cmake)
                        """)
                        pass
                    pass
                pass
            # --
            newlines.append("""
            SET(DOXYGEN_IS_OK 0)
            """)
            if self.module not in ['med']:
                newlines.append("""
                IF(WINDOWS)
                SET(CPPUNIT_IS_OK 0)
                ENDIF(WINDOWS)
                """)
                pass
            # --
            if self.module == "kernel":
                newlines.append("""
                SET(WITH_LOCAL 1)
                SET(WITH_BATCH 1)
                set(VERSION 5.1.3)
                set(XVERSION 0x050103)
                SET(CALCIUM_IDL_INT_F77 long)
                SET(CALCIUM_CORBA_INT_F77 CORBA::Long)
                SET(LONG_OR_INT int)
                """)
            elif self.module == "gui":
                newlines.append("""
                SET(GUI_ENABLE_CORBA ${CORBA_GEN})
                SET(ENABLE_VTKVIEWER ON)
                SET(ENABLE_SALOMEOBJECT ON)
                SET(ENABLE_OCCVIEWER ON)
                SET(ENABLE_GLVIEWER ON)
                SET(ENABLE_PLOT2DVIEWER ON)
                SET(ENABLE_PYCONSOLE ON)
                SET(ENABLE_SUPERVGRAPHVIEWER ON)
                SET(ENABLE_QXGRAPHVIEWER ON)
                set(VERSION 5.1.3)
                set(XVERSION 0x050103)
                """)
                pass
            elif self.module == "geom":
                newlines.append("""
                SET(GEOM_ENABLE_GUI ON)
                """)
                pass
            elif self.module == "medfile":
                newlines.append("""
                SET(MED_NUM_MAJEUR 2)
                SET(MED_NUM_MINEUR 3)
                SET(MED_NUM_RELEASE 5)
                SET(LONG_OR_INT int)
                IF(NOT WINDOWS)
                SET(FLIBS -lgfortranbegin -lgfortran)
                ENDIF(NOT WINDOWS)
                """)
                pass
            elif self.module == "med":
                newlines.append("""
                SET(MED_ENABLE_KERNEL ON)
                IF(NOT WINDOWS)
                SET(MED_ENABLE_SPLITTER ON)
                ENDIF(NOT WINDOWS)
                SET(MED_ENABLE_GUI ON)
                """)
                pass
            elif self.module == "smesh":
                newlines.append("""
                SET(SMESH_ENABLE_GUI ON)
                """)
                pass
            elif self.module == "netgenplugin":
                newlines.append("""
                SET(NETGENPLUGIN_ENABLE_GUI ON)
                """)
                pass
            elif self.module == "blsurfplugin":
                newlines.append("""
                SET(BLSURFPLUGIN_ENABLE_GUI ON)
                """)
                pass
            elif self.module == "ghs3dplugin":
                newlines.append("""
                SET(GHS3DPLUGIN_ENABLE_GUI ON)
                """)
                pass
            elif self.module == "yacs":
                newlines.append("""
                SET(SALOME_KERNEL ON)
                SET(HAS_GUI ON)
                SET(WITH_QT4 ON)
                """)
                pass
            # --
            pass
        # --
        newlines.append("""
        SET(VERSION_INFO 0.0.0)
        SET(SOVERSION_INFO 0)
        SET(SUBDIRS)
        SET(AM_CPPFLAGS)
        SET(AM_CXXFLAGS)
        SET(LDADD)
        """)
        if self.module == "kernel":
            newlines.append(r'''
            SET(AM_CPPFLAGS ${AM_CPPFLAGS} -DHAVE_SALOME_CONFIG -I${CMAKE_BINARY_DIR}/salome_adm/unix -include SALOMEconfig.h)
            SET(AM_CXXFLAGS ${AM_CXXFLAGS} -DHAVE_SALOME_CONFIG -I${CMAKE_BINARY_DIR}/salome_adm/unix -include SALOMEconfig.h)
            ''')
        else:
            if self.module not in ["yacs"]:
                newlines.append(r'''
                SET(AM_CPPFLAGS ${AM_CPPFLAGS} -DHAVE_SALOME_CONFIG -I${KERNEL_ROOT_DIR}/include/salome -include SALOMEconfig.h)
                SET(AM_CXXFLAGS ${AM_CXXFLAGS} -DHAVE_SALOME_CONFIG -I${KERNEL_ROOT_DIR}/include/salome -include SALOMEconfig.h)
                ''')
                pass
            pass
        # --
        return
    
    def treatLine(self, line, newlines, opened_ifs):
        
        # --
        # Print the comment above the line itself
        # --
        if line.find('#') >= 0:
            fields = line.split('#')
            line = fields[0]
            comment = '#'.join([''] + fields[1:])
            newlines.append(comment)
            if len(line) == 0:
                return
            pass
        
        # --
        # If the line begins with 'include ', just comment it
        # --
        if line.find("include ") == 0:
            newlines.append("# " + line)
            return
        
        # --
        # If the line begins with '-include', just comment it
        # --
        if line.find("-include") == 0:
            newlines.append("# " + line)
            return
        
        # --
        # If the line is a definition of a make rule, just comment it
        # --
        if line.count(':') == 1:
            newlines.append("# " + line)
            return
        
        # --
        # A particuliar case where there are two ":" on the same line
        # --
        if line.find('install-exec-local:') == 0:
            newlines.append("# " + line)
            return
        
        # --
        # If the line begin by a tabulation, consider it's a makefile command and comment it
        # --
        if line.find("\t") == 0:
            newlines.append("# " + line)
            return
        
        # --
        # --
        key = "-version-info"
        if line.find(key) >= 0:
            # --
            before = line.split(key)[0]
            after = line[len(before)+len(key):]
            sep = after[0]
            after = after[1:]
            version_info = after.split()[0]
            line = line.replace(key+sep+version_info, "")
            # --
            version_info = version_info.replace(':', '.')
            soversion_info = version_info.split('.')[0]
            newlines.append("SET(VERSION_INFO " + version_info + ")")
            newlines.append("SET(SOVERSION_INFO " + soversion_info + ")")
            # --
            pass
        
        # --
        # Replace the $(TOTO) by ${TOTO}
        # Replace the @TOTO@  by ${TOTO}
        # --
        line = p_dollar.sub(r"${\1}", line)
        line = p_arobas.sub(r"${\1}", line)
        
        # --
        line = line.replace(r"${top_builddir}", r"${CMAKE_BINARY_DIR}")
        line = line.replace(r"${top_srcdir}", r"${CMAKE_SOURCE_DIR}")
        line = line.replace(r"${srcdir}", r"${CMAKE_CURRENT_SOURCE_DIR}")
        line = line.replace(r"${builddir}", r"${CMAKE_CURRENT_BINARY_DIR}")
        line = line.replace(r"${datadir}", r"${CMAKE_INSTALL_PREFIX}/share")
        
        # --
        # Check if the line is a 'if' condition
        # If yes, replace it by a cmake grammar
        # --
        match = p_if.match(line)
        if match:
            theif = match.group("val")
            if theif[0] == "!":
                theif = "NOT " + theif[1:]
                pass
            line = p_if.sub(r"IF(%s)"%(theif), line)
            opened_ifs.append(theif)
            newlines.append(line)
            return
        
        # --
        # Check if the line is a 'else' condition
        # If yes, replace it by a cmake grammar
        # --
        match = p_else.match(line)
        if match:
            line = "ELSE(%s)"%(opened_ifs[-1])
            newlines.append(line)
            return
        
        # --
        # Check if the line is a 'endif' condition
        # If yes, replace it by a cmake grammar
        # --
        match = p_endif.match(line)
        if match:
            line = "ENDIF(%s)"%(opened_ifs[-1])
            opened_ifs[-1:] = []
            newlines.append(line)
            return
        
        # --
        # Check if the line is an attribution '=' or '+='
        # --
        match = p_attribution.match(line)
        if match:
            self.treatAttribution(match, newlines)
            return
        
        # --
        newlines.append(line)
        
        # --
        return
    
    def treatAttribution(self, match, newlines):
        
        spaces = match.group("spaces")
        key = match.group("key")
        method = match.group("method")
        value = match.group("value")
        # print [spaces, key, method, value]
        
        # --
        # Open cmake SET command
        # --
        newlines.append(spaces + "SET(" + key)
        
        # --
        # If method is '+=', put the previous definition as first value
        # --
        if method == "+=":
            newlines.append("%s    ${%s}"%(spaces, key))
            pass
        
        # --
        fields = value.split()
        for i in range(len(fields)):
            newlines.append("%s    %s"%(spaces, fields[i]))
            pass
        
        # --
        if method == "+=":
            # --
            # The try: except KeyError is here if the +=
            # is an error which occurs in salome ...
            # --
            try:
                self.__thedict__[key] += fields[:]
            except KeyError:
                self.__thedict__[key] = fields[:]
                pass
            pass
        else:
            self.__thedict__[key]  = fields[:]
            pass
        
        # --
        # Close cmake SET command
        # --
        
        newlines.append("%s)"%(spaces))
        
        return
    
    def finalize(self, newlines):
        
        # --
        # Convert the .in files in build dir
        # --
        if self.module == "yacs":
            key = "salomegui"
            if self.root[-len(key):] == key:
                self.files.append("resources/YACSCatalog.xml.in")
                pass
            pass
        for f in self.files:
            if f[-3:] == ".in":
                if f == "sstream.in":
                    continue
                if f in ["runContainer.in", "stopContainer.in"]:
                    if self.module == "med":
                        if self.root[-3:] == "csh":
                            continue
                        pass
                    pass
                if f == "SALOMEconfig.ref.in":
                    out = "SALOMEconfig.h"
                else:
                    out = f[:-3]
                    pass
                newlines.append(r'''
                SET(input ${CMAKE_CURRENT_SOURCE_DIR}/%s)
                '''%(f))
                newlines.append(r'''
                SET(output ${CMAKE_CURRENT_BINARY_DIR}/%s)
                '''%(out))
                newlines.append(r'''
                MESSAGE(STATUS "Creation of ${output}")
                CONFIGURE_FILE(${input} ${output})
                ''')
                pass
            pass
        
        # --
        # convert the SUBDIRS in cmake grammar
        # --
        if 1: # self.__thedict__.has_key("SUBDIRS"):
            newlines.append(r'''
            FOREACH(dir ${SUBDIRS})
            IF(NOT dir STREQUAL .)
            ADD_SUBDIRECTORY(${dir})
            ENDIF(NOT dir STREQUAL .)
            ENDFOREACH(dir ${SUBDIRS})
            ''')
            pass
        
        # --
        # --
        for key in ["lib_LTLIBRARIES", "noinst_LTLIBRARIES", "salomepyexec_LTLIBRARIES"]:
            if self.__thedict__.has_key(key):
                self.addLibTarget(key, newlines)
                pass
            pass
        
        # --
        # --
        for key in ["bin_PROGRAMS", "check_PROGRAMS"]:
            if self.__thedict__.has_key(key):
                self.addBinTarget(key, newlines)
                pass
            pass
        
        # --
        # --
        if self.__thedict__.has_key("BASEIDL_FILES"):
            if not self.__thedict__.has_key("IDL_FILES"):
                self.__thedict__["IDL_FILES"] = self.__thedict__["BASEIDL_FILES"]
                newlines.append('''
                SET(IDL_FILES ${BASEIDL_FILES})
                ''')
                pass
            pass
        
        # --
        # --
        
        key = "IDL_FILES"
        if self.__thedict__.has_key(key):
            if self.module == "kernel":
                newlines.append('''
                SET(IDL_FILES ${IDL_FILES} Calcium_Ports.idl)
                ''')
                pass
            newlines.append('''
            FOREACH(input ${IDL_FILES})
            STRING(REGEX REPLACE ".idl" "" base ${input})
            SET(src ${CMAKE_CURRENT_BINARY_DIR}/${base}SK.cc)
            SET(outputs ${src})
            SET(dynsrc ${CMAKE_CURRENT_BINARY_DIR}/${base}DynSK.cc)
            SET(outputs ${outputs} ${dynsrc})
            SET(inc ${CMAKE_CURRENT_BINARY_DIR}/${base}.hh)
            SET(outputs ${outputs} ${inc})
            IF(input STREQUAL Calcium_Ports.idl)
            SET(input ${CMAKE_CURRENT_BINARY_DIR}/${input})
            ELSE(input STREQUAL Calcium_Ports.idl)
            SET(input ${CMAKE_CURRENT_SOURCE_DIR}/${input})
            ENDIF(input STREQUAL Calcium_Ports.idl)
            SET(flags ${IDLCXXFLAGS} ${OMNIORB_IDLCXXFLAGS})
            STRING(REGEX MATCH "-bcxx" ISBCXX ${flags})
            IF(NOT ISBCXX)
            SET(flags -bcxx ${flags})
            ENDIF(NOT ISBCXX)
            ADD_CUSTOM_COMMAND(
            OUTPUT ${outputs}
            COMMAND ${OMNIORB_IDL} ${flags} ${input}
            MAIN_DEPENDENCY ${input}
            )
            ''')
            newlines.append('''
            install(FILES ${input} DESTINATION idl/salome)
            ''')
            if self.module not in ["pyhello"]:
                newlines.append('''
                SET(IDL_HEADER ${CMAKE_CURRENT_BINARY_DIR}/${base}.hh)
                install(FILES ${IDL_HEADER} DESTINATION include/salome)
                ''')
                pass
            newlines.append('''
            INSTALL(CODE "SET(IDL_FILE ${input})")
            INSTALL(CODE "SET(DIR lib/python${PYTHON_VERSION}/site-packages/salome)")
            INSTALL(CODE "SET(CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})")
            INSTALL(CODE "SET(OMNIORB_IDL_PYTHON ${OMNIORB_IDL_PYTHON})")
            # --
            SET(flags)
            FOREACH(f ${IDLPYFLAGS})
            SET(flags "${flags} ${f}")
            ENDFOREACH(f ${IDLPYFLAGS})
            STRING(REGEX MATCH "-bpython" ISBPYTHON ${flags})
            IF(NOT ISBPYTHON)
            SET(flags "-bpython ${flags}")
            ENDIF(NOT ISBPYTHON)
            SET(IDLPYFLAGS ${flags})
            STRING(REPLACE "\\\\" "/" IDLPYFLAGS ${IDLPYFLAGS})
            INSTALL(CODE "SET(IDLPYFLAGS ${IDLPYFLAGS})")
            # --
            ''')
            if self.module == "kernel":
                newlines.append('''
                INSTALL(SCRIPT ${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/install_python_from_idl.cmake)
                ''')
            else:
                newlines.append('''
                STRING(REPLACE "\\\\" "/" KERNEL_ROOT_DIR ${KERNEL_ROOT_DIR})
                INSTALL(SCRIPT ${KERNEL_ROOT_DIR}/salome_adm/cmake_files/install_python_from_idl.cmake)
                ''')
                pass
            newlines.append('''
            ENDFOREACH(input ${IDL_FILES})
            ''')
            pass
        
        # --
        # --
        for key in ["SWIG_SRC", "SWIGSOURCES", "SWIG_DEF"]:
            if self.__thedict__.has_key(key):
                newlines.append('''
                SET(SWIG_SOURCES ${%s})
                '''%(key))
                self.__thedict__["SWIG_SOURCES"] = self.__thedict__[key]
                pass
            pass
        
        # --
        # --
        if self.__thedict__.has_key("SWIG_SOURCES"):
            newlines.append('''
            IF(SWIG_SOURCES MATCHES ";")
            STRING(REGEX REPLACE ";.*" "" SWIG_SOURCES_FIRST "${SWIG_SOURCES}")
            ELSE(SWIG_SOURCES MATCHES ";")
            SET(SWIG_SOURCES_FIRST "${SWIG_SOURCES}")
            ENDIF(SWIG_SOURCES MATCHES ";")
            SET(flags)
            FOREACH(f ${SWIG_FLAGS} ${MY_SWIG_FLAGS})
            SET(test ON)
            IF(flags)
            LIST(FIND flags ${f} index)
            IF(NOT index EQUAL -1)
            SET(test OFF)
            ENDIF(NOT index EQUAL -1)
            ENDIF(flags)
            IF(test)
            SET(flags ${flags} ${f})
            ENDIF(test)
            ENDFOREACH(f ${SWIG_FLAGS} ${MY_SWIG_FLAGS})
            ADD_CUSTOM_COMMAND(
            OUTPUT ${build_srcs}
            COMMAND ${SWIG_EXECUTABLE} ${flags} -o ${build_srcs} ${CMAKE_CURRENT_SOURCE_DIR}/${SWIG_SOURCES_FIRST}
            MAIN_DEPENDENCY ${SWIG_SOURCES}
            )
            ''')
            pass
        
        # --
        # --
        if self.__thedict__.has_key("BUILT_SOURCES"):
            newlines.append('''
            FOREACH(f ${BUILT_SOURCES})
            IF(f MATCHES "WRAP.cxx$")
            # STRING(REGEX REPLACE "WRAP.cxx" "WRAP.h" inc ${f})
            STRING(REGEX REPLACE "WRAP.cxx" ".i" input ${f})
            ADD_CUSTOM_COMMAND(
            OUTPUT ${f} # ${inc}
            COMMAND ${SWIG_EXECUTABLE} ${SWIG_FLAGS} ${SWIG_PYTHON_INCLUDES} ${MYSWIG_FLAGS} -o ${f} ${CMAKE_CURRENT_SOURCE_DIR}/${input}
            MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/${input}
            )
            ENDIF(f MATCHES "WRAP.cxx$")
            ENDFOREACH(f ${BUILT_SOURCES})
            ''')
            pass

        # --
        # --
        key = "MOC_FILES"
        if self.__thedict__.has_key(key):
            newlines.append('''
            FOREACH(output ${MOC_FILES})
            ''')
            if self.module == "yacs":
                newlines.append('''
                STRING(REGEX REPLACE _moc.cxx .hxx input ${output})
                ''')
            else:
                newlines.append('''
                STRING(REGEX REPLACE _moc.cxx .h input ${output})
                ''')
                pass
            newlines.append('''
            SET(input ${CMAKE_CURRENT_SOURCE_DIR}/${input})
            SET(output ${CMAKE_CURRENT_BINARY_DIR}/${output})
            ADD_CUSTOM_COMMAND(
            OUTPUT ${output}
            COMMAND ${QT_MOC_EXECUTABLE} ${MOC_FLAGS} ${input} -o ${output}
            MAIN_DEPENDENCY ${input}
            )
            ENDFOREACH(output ${MOC_FILES})
            ''')
            pass
        
        # --
        # --
        key = "UIC_FILES"
        if self.__thedict__.has_key(key):
            newlines.append('''
            FOREACH(output ${UIC_FILES})
            STRING(REPLACE "ui_" "" input ${output})
            STRING(REPLACE ".h" ".ui" input ${input})
            SET(input ${CMAKE_CURRENT_SOURCE_DIR}/${input})
            SET(output ${CMAKE_CURRENT_BINARY_DIR}/${output})
            ADD_CUSTOM_COMMAND(
            OUTPUT ${output}
            COMMAND ${QT_UIC_EXECUTABLE} -o ${output} ${input}
            MAIN_DEPENDENCY ${input}
            )
            ENDFOREACH(output ${UIC_FILES})
            ''')
            pass
        
        # --
        # --
        key = "QRC_FILES"
        if self.__thedict__.has_key(key):
            newlines.append('''
            FOREACH(output ${QRC_FILES})
            STRING(REGEX REPLACE "qrc_" "" input ${output})
            STRING(REGEX REPLACE ".cxx" ".qrc" input ${input})
            STRING(REGEX REPLACE ".qrc" "" name ${input})
            SET(input ${CMAKE_CURRENT_SOURCE_DIR}/${input})
            SET(output ${CMAKE_CURRENT_BINARY_DIR}/${output})
            ADD_CUSTOM_COMMAND(
            OUTPUT ${output}
            COMMAND ${QT_RCC_EXECUTABLE} ${input} -o ${output} -name ${name}
            MAIN_DEPENDENCY ${input}
            )
            ENDFOREACH(output ${QRC_FILES})
            ''')
            pass
        
        # --
        # --
        key = "SIP_FILES"
        if self.__thedict__.has_key(key):
            newlines.append('''
            FOREACH(input ${SIP_FILES})
            SET(input ${CMAKE_CURRENT_SOURCE_DIR}/${input})
            SET(output)
            FOREACH(out ${SIP_SRC})
            SET(output ${output} ${CMAKE_CURRENT_BINARY_DIR}/${out})
            ENDFOREACH(out ${SIP_SRC})
            ADD_CUSTOM_COMMAND(
            OUTPUT ${output}
            COMMAND ${SIP_EXECUTABLE} ${PYQT_SIPFLAGS} ${input}
            MAIN_DEPENDENCY ${input}
            )
            ENDFOREACH(input ${SIP_FILES})
            ''')
            pass
        
        # --
        # Treat the install targets
        # --
        d = {
            "salomeadmux_DATA"            :  "salome_adm/unix",
            "dist_salomeadmux_DATA"       :  "salome_adm/unix",
            "dist_salome_cmake_DATA"      :  "salome_adm/cmake_files",
            "dist_salomem4_DATA"          :  "salome_adm/unix/config_files",
            "dist_salome4depr_DATA"       :  "salome_adm/unix/config_files/DEPRECATED",
            "dist_admlocalm4_DATA"        :  "adm_local/unix/config_files",
            "dist_admlocal_cmake_DATA"    :  "adm_local/cmake_files",
            "salomeinclude_DATA"          :  "include/salome",
            "salomeinclude_HEADERS"       :  "include/salome",
            "dist_salomeres_DATA"         :  "share/salome/resources/%s"%(self.module),
            "nodist_salomeres_DATA"       :  "share/salome/resources/%s"%(self.module),
            "nodist_salomeres_SCRIPTS"    :  "share/salome/resources/%s"%(self.module),
            "dist_salomescript_SCRIPTS"   :  "bin/salome",
            "dist_salomescript_DATA"      :  "bin/salome",
            "dist_salomescript_PYTHON"    :  "bin/salome",
            "nodist_salomescript_DATA"    :  "bin/salome",
            "salomepython_PYTHON"         :  "lib/python${PYTHON_VERSION}/site-packages/salome",
            "nodist_salomepython_PYTHON"  :  "lib/python${PYTHON_VERSION}/site-packages/salome",
            "dist_salomepython_DATA"      :  "lib/python${PYTHON_VERSION}/site-packages/salome",
            "sharedpkgpython_PYTHON"      :  "lib/python${PYTHON_VERSION}/site-packages/salome/shared_modules",
            }
        if self.module == "medfile":
            d = {
                "include_HEADERS"        :  "include",
                "nodist_include_HEADERS" :  "include",
                "bin_SCRIPTS"            :  "bin",
                "doc_DATA"               :  "${docdir}",
                }
            pass
        for key, value in d.items():
            if self.__thedict__.has_key(key):
                self.addInstallTarget(key, value, newlines)
                pass
            pass
        
        # --
        return
    
    def setLibAdd(self, key, newlines):
        # --
        newlines.append(r'''
        SET(libadd)
        ''')
        # --
        newlines.append(r'''
        IF(WINDOWS)
        SET(targets)
        SET(targets ${targets} MEFISTO2D)
        FOREACH(target ${targets})
        IF(name STREQUAL ${target})
        SET(dir $ENV{F2CHOME})
        STRING(REPLACE "\\\\" "/" dir ${dir})
        SET(libadd ${libadd} ${dir}/LIBF77.lib)
        SET(libadd ${libadd} ${dir}/LIBI77.lib)
        ENDIF(name STREQUAL ${target})
        ENDFOREACH(target ${targets})
        ENDIF(WINDOWS)
        ''')
        # --
        newlines.append(r'''
        SET(libs ${PLATFORM_LIBADD} ${PLATFORM_LDFLAGS} ${${amname}_LIBADD} ${${amname}_LDADD} ${${amname}_LDFLAGS})
        FOREACH(lib SALOMEBasics SalomeBatch)
        IF(name STREQUAL lib)
        SET(libs ${libs} ${PTHREAD_LIBS})
        ENDIF(name STREQUAL lib)
        ENDFOREACH(lib SALOMEBasics SalomeBatch)
        ''')
        if key == "bin_PROGRAMS":
            newlines.append(r'''
            SET(libs ${libs} ${LDADD})
            ''')
            pass
        # --
        newlines.append(r'''
        FOREACH(lib ${libs})
        GET_FILENAME_COMPONENT(ext ${lib} EXT)
        IF(ext STREQUAL .la)
        GET_FILENAME_COMPONENT(lib ${lib} NAME_WE)
        STRING(REGEX REPLACE "^lib" "" lib ${lib})
        ENDIF(ext STREQUAL .la)
        SET(vars)
        SET(vars ${vars} -no-undefined)
        SET(vars ${vars} -lvtkWidgets)
        IF(WINDOWS)
        SET(vars ${vars} -module)
        SET(vars ${vars} -Wl,-E)
        SET(vars ${vars} -Xlinker)
        SET(vars ${vars} -export-dynamic)
        SET(vars ${vars} -lm)
        SET(vars ${vars} -lboost_thread)
        SET(vars ${vars} -lboost_signals)
        SET(vars ${vars} -pthread -lpthread -ldl)
        ENDIF(WINDOWS)
        FOREACH(v ${vars})
        IF(lib STREQUAL v)
        SET(lib)
        ENDIF(lib STREQUAL v)
        ENDFOREACH(v ${vars})
        SET(libadd ${libadd} ${lib})
        ENDFOREACH(lib ${libs})
        TARGET_LINK_LIBRARIES(${name} ${libadd})
        ''')
        # --
        newlines.append(r'''
        IF(WINDOWS)
        SET(targets)
        SET(targets ${targets} MEFISTO2D)
        FOREACH(target ${targets})
        IF(name STREQUAL ${target})
        IF(CMAKE_BUILD_TYPE STREQUAL Debug)
        SET_TARGET_PROPERTIES(${name} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRT")
        ENDIF(CMAKE_BUILD_TYPE STREQUAL Debug)
        ENDIF(name STREQUAL ${target})
        ENDFOREACH(target ${targets})
        ENDIF(WINDOWS)
        ''')
        # --
        return
    
    def setCompilationFlags(self, key, newlines):
        newlines.append(r'''
        SET(var)
        IF(WINDOWS)
        SET(targets)
        SET(targets ${targets} SalomeIDLKernel)
        SET(targets ${targets} SalomeDS)
        SET(targets ${targets} SALOMEDSTest)
        SET(targets ${targets} SALOMEDS_Client_exe)
        SET(targets ${targets} SalomeIDLGEOM)
        SET(targets ${targets} GEOMEngine)
        SET(targets ${targets} MEDEngine)
        SET(targets ${targets} SMESHEngine)
        SET(targets ${targets} SMESH)
        FOREACH(target ${targets})
        IF(name STREQUAL ${target})
        SET(var ${var} -DNOGDI)
        ENDIF(name STREQUAL ${target})
        ENDFOREACH(target ${targets})
        ENDIF(WINDOWS)
        ''')
        # --
        if self.module in ["medfile", "yacs"]:
            newlines.append(r'''
            IF(WINDOWS)
            SET(var ${var} -DNOGDI)
            ENDIF(WINDOWS)
            ''')
            pass
        # --
        newlines.append(r'''
        IF(WINDOWS)
        SET(targets)
        SET(targets ${targets} MEFISTO2D)
        FOREACH(target ${targets})
        IF(name STREQUAL ${target})
        SET(dir $ENV{F2CHOME})
        STRING(REPLACE "\\\\" "/" dir ${dir})
        SET(var ${var} -I${dir})
        SET(var ${var} -DF2C_BUILD)
        ENDIF(name STREQUAL ${target})
        ENDFOREACH(target ${targets})
        ENDIF(WINDOWS)
        ''')
        # --
        if self.module in ["geom", "med"]:
            newlines.append(r'''
            SET(var ${var} -I${CMAKE_CURRENT_SOURCE_DIR})
            SET(var ${var} -I${CMAKE_CURRENT_BINARY_DIR})
            ''')
            pass
        newlines.append(r'''
        SET(var ${var} ${AM_CPPFLAGS})
        SET(var ${var} ${AM_CXXFLAGS})
        ''')
        if self.module == "yacs":
            newlines.append(r'''
            SET(var ${var} -DYACS_PTHREAD)
            SET(var ${var} -DCMAKE_BUILD)
            SET(var ${var} -DSALOME_KERNEL)
            SET(var ${var} -DDSC_PORTS)
            SET(var ${var} -DOMNIORB)
            ''')
            pass
        newlines.append(r'''
	SET(var ${var} ${PLATFORM_CPPFLAGS})
	SET(var ${var} ${PTHREAD_CFLAGS})
	SET(var ${var} ${${amname}_CPPFLAGS})
	SET(var ${var} ${${amname}_CXXFLAGS})
	SET(var ${var} ${${amname}_CFLAGS})
        SET(vars)
        IF(WINDOWS)
        SET(vars ${vars} -include SALOMEconfig.h)
        SET(vars ${vars} -ftemplate-depth-32)
        SET(vars ${vars} -fPIC)
        SET(vars ${vars} -g)
        ENDIF(WINDOWS)
        SET(flags)
        FOREACH(f ${var})
        FOREACH(v ${vars})
        IF(f STREQUAL v)
        SET(f)
        ENDIF(f STREQUAL v)
        ENDFOREACH(v ${vars})
        SET(flags "${flags} ${f}")
        ENDFOREACH(f ${var})
        SET_TARGET_PROPERTIES(${name} PROPERTIES COMPILE_FLAGS "${flags}")
        ''')
        return
    
    def addLibTarget(self, key, newlines):
        newlines.append(r'''
        FOREACH(amname ${%s})
        '''%(key))
        # --
        # Replace .la by _la ...
        # --
        newlines.append(r'''
        STRING(REPLACE .la _la amname ${amname})
        ''')
        # --
        # Remove the _la for the cmake name
        # --
        newlines.append(r'''
        STRING(LENGTH ${amname} len)
        MATH(EXPR newlen "${len}-3")
        STRING(SUBSTRING ${amname} 0 ${newlen} name)
        ''')
        # --
        # Does the target begins with lib ??
        # If yes, remove lib at beginning for cmake name
        # --
        newlines.append(r'''
        STRING(REGEX MATCH "^lib" BEGIN_WITH_lib ${name})
        IF(BEGIN_WITH_lib)
        STRING(LENGTH ${name} len)
        MATH(EXPR newlen "${len}-3")
        STRING(SUBSTRING ${name} 3 ${newlen} name)
        ENDIF(BEGIN_WITH_lib)
        ''')
        # --
        # Does the target is an idl library
        # --
        newlines.append(r'''
        STRING(REGEX MATCH "IDL" ISIDL ${name})
        ''')
        # --
        # Set the type of the library
        # --
        newlines.append(r'''
        IF(ISIDL)
        IF(WINDOWS)
        SET(type STATIC)
        ELSE(WINDOWS)
        SET(type SHARED)
        ENDIF(WINDOWS)
        ELSE(ISIDL)
        SET(type SHARED)
        ENDIF(ISIDL)
        ''')
        # --
        # Set sources for the library
        # --
        newlines.append(r'''
        SET(srcs)
        FOREACH(src ${${amname}_SOURCES} ${dist_${amname}_SOURCES})
        GET_FILENAME_COMPONENT(ext ${src} EXT)
        IF(ext STREQUAL .f)
        IF(src STREQUAL trte.f)
        SET(input ${CMAKE_CURRENT_SOURCE_DIR}/${src})
        STRING(REPLACE ".f" ".c" src ${src})
        SET(src ${CMAKE_CURRENT_BINARY_DIR}/${src})
        SET(output ${src})
        ADD_CUSTOM_COMMAND(
        OUTPUT ${output}
        COMMAND f2c ${input}
        MAIN_DEPENDENCY ${input}
        )
        ELSE(src STREQUAL trte.f)
        SET(input ${CMAKE_CURRENT_SOURCE_DIR}/${src})
        STRING(REPLACE ".f" ".o" src ${src})
        SET(src ${CMAKE_CURRENT_BINARY_DIR}/${src})
        SET(output ${src})
        IF(WINDOWS)
        SET(F77 g77)
        ELSE(WINDOWS)
        SET(F77 gfortran)
        ENDIF(WINDOWS)
        ADD_CUSTOM_COMMAND(
        OUTPUT ${output}
        COMMAND ${F77} -c -o ${output} ${input}
        MAIN_DEPENDENCY ${input}
        )
        ENDIF(src STREQUAL trte.f)
        ENDIF(ext STREQUAL .f)
        SET(srcs ${srcs} ${src})
        ENDFOREACH(src ${${amname}_SOURCES} ${dist_${amname}_SOURCES})
        ''')
        newlines.append(r'''
        SET(l ${nodist_${amname}_SOURCES} ${UIC_FILES})
        FOREACH(f ${l})
        SET(src ${CMAKE_CURRENT_BINARY_DIR}/${f})
        SET(srcs ${srcs} ${src})
        ENDFOREACH(f ${l})
        ''')
        newlines.append(r'''
        SET(build_srcs)
        SET(l ${nodist_${amname}_SOURCES} ${BUILT_SOURCES})
        FOREACH(f ${l})
        GET_FILENAME_COMPONENT(ext ${f} EXT)
        IF(ext STREQUAL .py)
        ELSE(ext STREQUAL .py)
        SET(build_srcs ${build_srcs} ${CMAKE_CURRENT_BINARY_DIR}/${f})
        ENDIF(ext STREQUAL .py)
        ENDFOREACH(f ${l})
        ''')
        # --
        # Add the library to cmake
        # --
        newlines.append(r'''
        ADD_LIBRARY(${name} ${type} ${srcs})
        ''')
        # --
        # The compilation flags
        # --
        self.setCompilationFlags(key, newlines)
        # --
        newlines.append(r'''
        SET_TARGET_PROPERTIES(${name} PROPERTIES VERSION ${VERSION_INFO} SOVERSION ${SOVERSION_INFO})
        ''')
        # --
        self.setLibAdd(key, newlines)
        # --
        if 1: # key != "noinst_LTLIBRARIES":
            if self.module == "medfile":
                newlines.append(r'''
                SET(DEST lib)
                ''')
            else:
                newlines.append(r'''
                SET(DEST lib/salome)
                ''')
                pass
            newlines.append(r'''
            IF(BEGIN_WITH_lib)
            INSTALL(TARGETS ${name} DESTINATION ${DEST})
            ''')
            if self.module == "gui":
                newlines.append(r'''
                FOREACH(lib SalomePyQt)
                IF(name STREQUAL lib)
                IF(WINDOWS)
                IF(CMAKE_BUILD_TYPE STREQUAL Release)
                INSTALL(FILES ${CMAKE_INSTALL_PREFIX}/${DEST}/${name}.dll DESTINATION ${DEST} RENAME ${name}.pyd)
                ELSE(CMAKE_BUILD_TYPE STREQUAL Release)
                INSTALL(FILES ${CMAKE_INSTALL_PREFIX}/${DEST}/${name}.dll DESTINATION ${DEST} RENAME ${name}_d.pyd)
                ENDIF(CMAKE_BUILD_TYPE STREQUAL Release)
                ELSE(WINDOWS)
                INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${name}.so DESTINATION ${DEST} RENAME ${name}.so)
                ENDIF(WINDOWS)
                ENDIF(name STREQUAL lib)
                ENDFOREACH(lib SalomePyQt)
                FOREACH(lib SalomePy)
                IF(name STREQUAL lib)
                IF(WINDOWS)
                IF(CMAKE_BUILD_TYPE STREQUAL Release)
                INSTALL(FILES ${CMAKE_INSTALL_PREFIX}/${DEST}/${name}.dll DESTINATION ${DEST} RENAME lib${name}.pyd)
                ELSE(CMAKE_BUILD_TYPE STREQUAL Release)
                INSTALL(FILES ${CMAKE_INSTALL_PREFIX}/${DEST}/${name}.dll DESTINATION ${DEST} RENAME lib${name}_d.pyd)
                ENDIF(CMAKE_BUILD_TYPE STREQUAL Release)
                ENDIF(WINDOWS)
                ENDIF(name STREQUAL lib)
                ENDFOREACH(lib SalomePy)
                ''')
                pass
            if self.module == "geom":
                newlines.append(r'''
                IF(WINDOWS)
                STRING(REGEX MATCH "Export" ISExport ${name})
                IF(ISExport)
                INSTALL(FILES ${CMAKE_INSTALL_PREFIX}/${DEST}/${name}.dll DESTINATION ${DEST} RENAME lib${name}.dll)
                ENDIF(ISExport)
                STRING(REGEX MATCH "Import" ISImport ${name})
                IF(ISImport)
                INSTALL(FILES ${CMAKE_INSTALL_PREFIX}/${DEST}/${name}.dll DESTINATION ${DEST} RENAME lib${name}.dll)
                ENDIF(ISImport)
                ENDIF(WINDOWS)
                ''')
                pass
            newlines.append(r'''
            ELSE(BEGIN_WITH_lib)
            ''')
            newlines.append(r'''
            IF(WINDOWS)
            INSTALL(TARGETS ${name} DESTINATION lib/python${PYTHON_VERSION}/site-packages/salome)
            IF(CMAKE_BUILD_TYPE STREQUAL Release)
            INSTALL(FILES ${CMAKE_INSTALL_PREFIX}/lib/python${PYTHON_VERSION}/site-packages/salome/${name}.dll DESTINATION lib/python${PYTHON_VERSION}/site-packages/salome RENAME ${name}.pyd)
            ELSE(CMAKE_BUILD_TYPE STREQUAL Release)
            INSTALL(FILES ${CMAKE_INSTALL_PREFIX}/lib/python${PYTHON_VERSION}/site-packages/salome/${name}.dll DESTINATION lib/python${PYTHON_VERSION}/site-packages/salome RENAME ${name}_d.pyd)
            ENDIF(CMAKE_BUILD_TYPE STREQUAL Release)
            ELSE(WINDOWS)
            GET_TARGET_PROPERTY(version ${name} VERSION)
            GET_TARGET_PROPERTY(soversion ${name} SOVERSION)
            INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${name}.so.${version} DESTINATION lib/python${PYTHON_VERSION}/site-packages/salome RENAME ${name}.so.${version})
            INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${name}.so.${version} DESTINATION lib/python${PYTHON_VERSION}/site-packages/salome RENAME ${name}.so.${soversion})
            INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${name}.so.${version} DESTINATION lib/python${PYTHON_VERSION}/site-packages/salome RENAME ${name}.so)
            ENDIF(WINDOWS)
            ''')
            newlines.append(r'''
            ENDIF(BEGIN_WITH_lib)
            ''')
            pass
        # --
        newlines.append(r'''
        ENDFOREACH(amname ${%s})
        '''%(key))
        # --
        return
    
    def addBinTarget(self, key, newlines):
        # --
        newlines.append(r'''
        FOREACH(amname ${bin_PROGRAMS} ${check_PROGRAMS})
        ''')
        # --
        newlines.append(r'''
        SET(name "${amname}_exe")
        SET(srcs ${${amname}_SOURCES} ${dist_${amname}_SOURCES})
        SET(l ${nodist_${amname}_SOURCES})
        FOREACH(f ${l})
        SET(src ${CMAKE_CURRENT_BINARY_DIR}/${f})
        SET(srcs ${srcs} ${src})
        ENDFOREACH(f ${l})
        LIST(LENGTH srcs nb)
        IF(nb)
        ADD_EXECUTABLE(${name} ${srcs})
        ''')
        # --
        self.setCompilationFlags(key, newlines)
        # --
        self.setLibAdd(key, newlines)
        # --
        if self.module == "medfile":
            newlines.append(r'''
            SET(DEST bin)
            ''')
        else:
            newlines.append(r'''
            SET(DEST bin/salome)
            ''')
            pass
        # --
        if key == "bin_PROGRAMS":
            newlines.append(r'''
            IF(WINDOWS)
            INSTALL(TARGETS ${name} DESTINATION ${DEST})
            INSTALL(FILES ${CMAKE_INSTALL_PREFIX}/${DEST}/${name}.exe DESTINATION ${DEST} RENAME ${amname}.exe)
            INSTALL(CODE "FILE(REMOVE ${CMAKE_INSTALL_PREFIX}/${DEST}/${name}.exe)")
            ELSE(WINDOWS)
            SET(PERMS)
            SET(PERMS ${PERMS} OWNER_READ OWNER_WRITE OWNER_EXECUTE)
            SET(PERMS ${PERMS} GROUP_READ GROUP_EXECUTE)
            SET(PERMS ${PERMS} WORLD_READ WORLD_EXECUTE)
            INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/${name} DESTINATION ${DEST} PERMISSIONS ${PERMS} RENAME ${amname})
            ENDIF(WINDOWS)
            ''')
            pass
        # --
        newlines.append(r'''
        ENDIF(nb)
        ''')
        # --
        newlines.append(r'''
        ENDFOREACH(amname ${bin_PROGRAMS} ${check_PROGRAMS})
        ''')
        # --
        return
    
    def addInstallTarget(self, key, destination, newlines):
        newlines.append(r"FOREACH(f ${%s})"%(key))
        newlines.append(r'''
        SET(DEST %s)
        '''%(destination))
        newlines.append(r'''
        STRING(COMPARE EQUAL ${f} SALOMEconfig.h.in test_SALOMEconfig.h.in)
        IF(test_SALOMEconfig.h.in)
        INSTALL(FILES SALOMEconfig.ref.in DESTINATION ${DEST} RENAME SALOMEconfig.h.in)
        ELSE(test_SALOMEconfig.h.in)
        SET(dummy dummy-NOTFOUND)
        MARK_AS_ADVANCED(dummy)
        # FILE(REMOVE ${CMAKE_INSTALL_PREFIX}/${DEST}/${f})
        FIND_FILE(dummy ${f} PATHS ${CMAKE_CURRENT_SOURCE_DIR} NO_DEFAULT_PATH)
        IF(dummy)
        ''')
        if key in ['dist_salomescript_SCRIPTS']:
            newlines.append(r'''
            SET(PERMS)
            SET(PERMS ${PERMS} OWNER_READ OWNER_WRITE OWNER_EXECUTE)
            SET(PERMS ${PERMS} GROUP_READ GROUP_EXECUTE)
            SET(PERMS ${PERMS} WORLD_READ WORLD_EXECUTE)
            INSTALL(FILES ${f} DESTINATION ${DEST} PERMISSIONS ${PERMS})
            ''')
        else:
            newlines.append(r'''
            GET_FILENAME_COMPONENT(ext ${f} EXT)
            IF(ext STREQUAL .py)
            IF(DEST STREQUAL bin/salome)
            SET(PERMS)
            SET(PERMS ${PERMS} OWNER_READ OWNER_WRITE OWNER_EXECUTE)
            SET(PERMS ${PERMS} GROUP_READ GROUP_EXECUTE)
            SET(PERMS ${PERMS} WORLD_READ WORLD_EXECUTE)
            INSTALL(FILES ${f} DESTINATION ${DEST} PERMISSIONS ${PERMS})
            ELSE(DEST STREQUAL bin/salome)
            INSTALL(FILES ${f} DESTINATION ${DEST})
            ENDIF(DEST STREQUAL bin/salome)
            ELSE(ext STREQUAL .py)
            INSTALL(FILES ${f} DESTINATION ${DEST})
            ENDIF(ext STREQUAL .py)
            ''')
            pass
        newlines.append(r'''
        ELSE(dummy)
        GET_FILENAME_COMPONENT(ext ${f} EXT)
        IF(ext STREQUAL .qm)
        STRING(REGEX REPLACE .qm .ts input ${f})
        ''')
        if self.module in ["kernel", "gui", "yacs"]:
            newlines.append(r'''
            SET(input ${CMAKE_CURRENT_SOURCE_DIR}/resources/${input})
            ''')
        else:
            newlines.append(r'''
            SET(input ${CMAKE_CURRENT_SOURCE_DIR}/${input})
            ''')
            pass
        newlines.append(r'''
        SET(output ${CMAKE_CURRENT_BINARY_DIR}/${f})
        # ADD_CUSTOM_COMMAND(
        # OUTPUT ${output}
        # COMMAND ${QT_LRELEASE_EXECUTABLE} ${input} -qm ${output}
        # MAIN_DEPENDENCY ${input}
        # )
        EXECUTE_PROCESS(COMMAND ${QT_LRELEASE_EXECUTABLE} ${input} -qm ${output})
        ENDIF(ext STREQUAL .qm)
        INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/${f} DESTINATION ${DEST})
        ENDIF(dummy)
        ENDIF(test_SALOMEconfig.h.in)
        ''')
        newlines.append(r'''
        GET_FILENAME_COMPONENT(ext ${f} EXT)
        IF(ext STREQUAL .py)
        INSTALL(CODE "SET(PYTHON_FILE ${f})")
        INSTALL(CODE "SET(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})")
        INSTALL(CODE "SET(DEST ${DEST})")
        INSTALL(CODE "SET(PYTHON_EXECUTABLE ${PYTHON_EXECUTABLE})")
        ''')
        if self.module == "kernel":
            newlines.append('''
            IF(f STREQUAL SALOME_ContainerPy.py)
            ELSE(f STREQUAL SALOME_ContainerPy.py)
            IF(f STREQUAL am2cmake.py)
            ELSE(f STREQUAL am2cmake.py)
            INSTALL(SCRIPT ${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/install_and_compile_python_file.cmake)
            ENDIF(f STREQUAL am2cmake.py)
            ENDIF(f STREQUAL SALOME_ContainerPy.py)
            ''')
        else:
            newlines.append('''
            STRING(REPLACE "\\\\" "/" KERNEL_ROOT_DIR ${KERNEL_ROOT_DIR})
            INSTALL(SCRIPT ${KERNEL_ROOT_DIR}/salome_adm/cmake_files/install_and_compile_python_file.cmake)
            ''')
            pass
        newlines.append(r'''
        ENDIF(ext STREQUAL .py)
        ''') 
        newlines.append(r"ENDFOREACH(f ${%s})"%(key))
        return
    
    def writeListsFile(self):
        f = open(self.listsFile, "w")
        f.write(self.content)
        f.close()
        return
    
    pass

def convertAmFile(the_root, root, dirs, files, f, module):
    cmake = CMakeFile(the_root, root, dirs, files, f, module)
    cmake.writeListsFile()
    return

def usage(exit_status):
    from sys import exit
    from sys import argv
    print "Usage: %s --module"%(argv[0])
    exit(exit_status)
    return

if __name__ == "__main__":
    #
    from sys import argv
    if len(argv) != 2:
        usage(1)
        pass
    #
    module = argv[1]
    if module.find('--') != 0:
        usage(1)
        pass
    module = module[2:]
    if len(module) == 0:
        usage(1)
        pass
    #
    from os import getcwd
    the_root = getcwd()
    #
    from os import walk
    for root, dirs, files in walk(the_root):
        # --
        # E.A. : Remove 'CVS' in dirs
        # E.A. : It allows to not recurse in CVS dirs
        # E.A. : See os module python documentation
        # --
        try:
            dirs.remove('CVS')
        except ValueError:
            pass
        # --
        if "Makefile.am.cmake" in files:
            if "Makefile.am" in files:
                files.remove("Makefile.am")
                pass
            pass
        # --
        for f in files:
            if f in ["Makefile.am", "Makefile.am.cmake"]:
                convertAmFile(the_root, root, dirs, files, f, module)
                pass
            pass
        pass
    #
    pass
