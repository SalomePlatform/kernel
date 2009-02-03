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
        content = content.replace("-no-undefined -version-info=0:0:0", "")
        content = content.replace("-include SALOMEconfig.h", "")

        # --
        # Compatibility medfile
        # --
        content = content.replace("-no-undefined -version-info 0:0:0", "")
        content = content.replace("-no-undefined -version-info 2:5:1", "")
        
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
            "interpkernel",
            "InterpKernelTest",
            "MEDMEMCppTest",
            "medmem",
            "med_V2_1",
            "MEDWrapperBase",
            "MEDWrapper",
            "MEDWrapper_V2_1",
            "MEDWrapper_V2_2",
            "SalomeIDLMED",
            ]
        full_list = cas_list + kernel_list + gui_list
        full_list += geom_list + med_list
        # --
        full_list += [
            "boost_thread",
            ]
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
        # Add a last CR at the end of the file
        # --
        newlines.append('\n')
        
        # --
        # Concatenate newlines into content
        # --
        content = '\n'.join(newlines)

        # --
        self.content = content
        
        # --
        return
    
    def initialize(self, newlines):
        if self.root == self.the_root:
            # --
            newlines.append("""
            CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
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
                    pass
                pass
            # --
            if self.module == "kernel":
                newlines.append("""
                SET(WITH_LOCAL 1)
                SET(WITH_BATCH 1)
                set(VERSION 4.1.3)
                set(XVERSION 0x040103)
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
                # SET(ENABLE_QXGRAPHVIEWER ON)
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
                SET(MED_ENABLE_GUI ON)
                """)
                pass
            # --
            pass
        # --
        newlines.append("""
        SET(SUBDIRS)
        SET(AM_CPPFLAGS)
        SET(AM_CXXFLAGS)
        SET(LDADD)
        """)
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
        # A particuliar case
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
        for f in self.files:
            if f[-3:] == ".in":
                if f == "sstream.in":
                    continue
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
        for key in ["bin_PROGRAMS"]:
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
            newlines.append('''
            FOREACH(input ${IDL_FILES})
            STRING(REGEX REPLACE ".idl" "" base ${input})
            SET(src ${CMAKE_CURRENT_BINARY_DIR}/${base}SK.cc)
            SET(outputs ${src})
            SET(dynsrc ${CMAKE_CURRENT_BINARY_DIR}/${base}DynSK.cc)
            SET(outputs ${outputs} ${dynsrc})
            ADD_CUSTOM_COMMAND(
            OUTPUT ${outputs}
            COMMAND ${OMNIORB_IDL} ${IDLCXXFLAGS} ${OMNIORB_IDLCXXFLAGS} ${CMAKE_CURRENT_SOURCE_DIR}/${input}
            MAIN_DEPENDENCY ${input}
            )
            install(FILES ${input} DESTINATION idl/salome)
            SET(IDL_HEADER ${CMAKE_CURRENT_BINARY_DIR}/${base}.hh)
            install(FILES ${IDL_HEADER} DESTINATION include/salome)
            INSTALL(CODE "SET(IDL_FILE ${input})")
            INSTALL(CODE "SET(DIR lib/python${PYTHON_VERSION}/site-packages/salome)")
            INSTALL(CODE "SET(CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})")
            INSTALL(CODE "SET(OMNIORB_IDL_PYTHON ${OMNIORB_IDL_PYTHON})")
            # --
            SET(flags)
            FOREACH(f ${IDLPYFLAGS})
            SET(flags "${flags} ${f}")
            ENDFOREACH(f ${IDLPYFLAGS})
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
            ADD_CUSTOM_COMMAND(
            OUTPUT ${build_srcs}
            COMMAND ${SWIG_EXECUTABLE} ${SWIG_FLAGS} ${MY_SWIG_FLAGS} -o ${build_srcs} ${CMAKE_CURRENT_SOURCE_DIR}/${SWIG_SOURCES_FIRST}
            MAIN_DEPENDENCY ${SWIG_SOURCES}
            )
            ''')
            pass
        
        # --
        # --
        key = "MOC_FILES"
        if self.__thedict__.has_key(key):
            newlines.append('''
            FOREACH(output ${MOC_FILES})
            STRING(REGEX REPLACE _moc.cxx .h input ${output})
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
        SET(libadd ${libadd} Userenv.lib Ws2_32.lib)
        ELSE(WINDOWS)
        SET(libadd ${libadd} -ldl -lpthread)
        ENDIF(WINDOWS)
        ''')
        # --
        newlines.append(r'''
        SET(libs ${${amname}_LIBADD} ${${amname}_LDADD} ${${amname}_LDFLAGS})
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
        STRING(REPLACE "lib" "" lib ${lib})
        ENDIF(ext STREQUAL .la)
        IF(WINDOWS)
        SET(vars -Xlinker -export-dynamic -module -Wl,-E)
        SET(vars ${vars} -lutil -lm)
        FOREACH(v ${vars})
        IF(lib STREQUAL v)
        SET(lib)
        ENDIF(lib STREQUAL v)
        ENDFOREACH(v ${vars})
        ENDIF(WINDOWS)
        SET(libadd ${libadd} ${lib})
        ENDFOREACH(lib ${libs})
        TARGET_LINK_LIBRARIES(${name} ${PTHREADS_LIBRARY} ${libadd})
        ''')
        # --
        newlines.append(r'''
        IF(WINDOWS)
        SET(targets)
        SET(targets ${targets} SalomeHDFPersist)
        SET(targets ${targets} medC)
        SET(targets ${targets} medimport)
        SET(targets ${targets} medimportcxx)
        FOREACH(target ${targets})
        IF(name STREQUAL ${target})
        SET_TARGET_PROPERTIES(${name} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:LIBCMTD")
        ENDIF(name STREQUAL ${target})
        ENDFOREACH(target ${targets})
        ENDIF(WINDOWS)
        ''')
        # --
        if self.module == "med":
            newlines.append(r'''
            IF(WINDOWS)
            SET_TARGET_PROPERTIES(${name} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:LIBCMTD")
            ENDIF(WINDOWS)
            ''')
            pass
        # --
        return
    
    def setCompilationFlags(self, key, newlines):
        newlines.append(r'''
        SET(var)
        IF(WINDOWS)
        SET(targets)
        SET(targets ${targets} SalomeIDLKernel)
        SET(targets ${targets} SalomeDS)
        SET(targets ${targets} SALOMEDS_Client_exe)
        SET(targets ${targets} SalomeIDLGEOM)
        SET(targets ${targets} GEOMEngine)
        SET(targets ${targets} MEDEngine)
        SET(targets ${targets} SMESHEngine)
        FOREACH(target ${targets})
        IF(name STREQUAL ${target})
        SET(var ${var} -DNOGDI)
        ENDIF(name STREQUAL ${target})
        ENDFOREACH(target ${targets})
        ENDIF(WINDOWS)
        ''')
        # --
        newlines.append(r'''
        IF(WINDOWS)
        SET(var ${var} -D_USE_MATH_DEFINES)
        ENDIF(WINDOWS)
        ''')
        # --
        if self.module in ["geom", "med"]:
            newlines.append(r'''
            SET(var ${var} -I${CMAKE_CURRENT_SOURCE_DIR})
            SET(var ${var} -I${CMAKE_CURRENT_BINARY_DIR})
            ''')
            pass
        if key == "bin_PROGRAMS":
            newlines.append(r'''
            SET(var ${var} ${AM_CPPFLAGS})
            SET(var ${var} ${AM_CXXFLAGS})
            ''')
            pass
        newlines.append(r'''
	SET(var ${var} ${PLATFORM_CPPFLAGS})
	SET(var ${var} ${PTHREADS_INCLUDES})
	SET(var ${var} ${${amname}_CPPFLAGS})
	SET(var ${var} ${${amname}_CXXFLAGS})
	SET(var ${var} ${${amname}_CFLAGS})
        SET(flags)
        FOREACH(f ${var})
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
        ENDIF(ext STREQUAL .f)
        SET(srcs ${srcs} ${src})
        ENDFOREACH(src ${${amname}_SOURCES} ${dist_${amname}_SOURCES})
        SET(build_srcs)
        FOREACH(f ${nodist_${amname}_SOURCES} ${BUILT_SOURCES})
        GET_FILENAME_COMPONENT(ext ${f} EXT)
        IF(ext STREQUAL .py)
        ELSE(ext STREQUAL .py)
        SET(build_srcs ${build_srcs} ${CMAKE_CURRENT_BINARY_DIR}/${f})
        ENDIF(ext STREQUAL .py)
        ENDFOREACH(f ${nodist_${amname}_SOURCES})
        SET(srcs ${build_srcs} ${srcs})
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
        SET_TARGET_PROPERTIES(${name} PROPERTIES VERSION 0.0.0 SOVERSION 0)
        FOREACH(lib medC med)
        IF(lib STREQUAL ${name})
        SET_TARGET_PROPERTIES(${name} PROPERTIES VERSION 1.1.5 SOVERSION 1)
        ENDIF(lib STREQUAL ${name})
        ENDFOREACH(lib medC med)
        ''')
        # --
        from os.path import basename
        upper_name = basename(self.root).upper()
        # --
        if upper_name in ["2.1.X", "2.3.1"]:
            upper_name = "D_" + upper_name
            pass
        # --
        newlines.append(r'''
        SET_TARGET_PROPERTIES(${name} PROPERTIES DEFINE_SYMBOL %s_EXPORTS)
        '''%(upper_name))
        newlines.append(r'''
        IF(name STREQUAL SalomeLauncher)
        SET_TARGET_PROPERTIES(${name} PROPERTIES DEFINE_SYMBOL SALOME%s_EXPORTS)
        ENDIF(name STREQUAL SalomeLauncher)
        '''%(upper_name))
        newlines.append(r'''
        IF(name STREQUAL SalomeResourcesManager)
        SET_TARGET_PROPERTIES(${name} PROPERTIES DEFINE_SYMBOL SALOME%s_EXPORTS)
        ENDIF(name STREQUAL SalomeResourcesManager)
        '''%(upper_name))
        newlines.append(r'''
        IF(name STREQUAL GEOMObject)
        SET_TARGET_PROPERTIES(${name} PROPERTIES DEFINE_SYMBOL GEOM_%s_EXPORTS)
        ENDIF(name STREQUAL GEOMObject)
        '''%(upper_name))
        newlines.append(r'''
        IF(name STREQUAL medC)
        SET_TARGET_PROPERTIES(${name} PROPERTIES DEFINE_SYMBOL MEDC_DLL_EXPORTS)
        ENDIF(name STREQUAL medC)
        ''')
        newlines.append(r'''
        IF(name STREQUAL med)
        SET_TARGET_PROPERTIES(${name} PROPERTIES DEFINE_SYMBOL MED_DLL_EXPORTS)
        ENDIF(name STREQUAL med)
        ''')
        newlines.append(r'''
        IF(name STREQUAL medimport)
        SET_TARGET_PROPERTIES(${name} PROPERTIES DEFINE_SYMBOL MEDIMPORT_DLL_EXPORTS)
        ENDIF(name STREQUAL medimport)
        ''')
        newlines.append(r'''
        IF(name STREQUAL medimportcxx)
        SET_TARGET_PROPERTIES(${name} PROPERTIES DEFINE_SYMBOL MEDIMPORTCXX_DLL_EXPORTS)
        ENDIF(name STREQUAL medimportcxx)
        ''')
        newlines.append(r'''
        IF(name STREQUAL MEDWrapperBase)
        SET_TARGET_PROPERTIES(${name} PROPERTIES DEFINE_SYMBOL MEDWRAPPER_BASE_EXPORTS)
        ENDIF(name STREQUAL MEDWrapperBase)
        ''')
        newlines.append(r'''
        IF(name STREQUAL MEDWrapper_V2_1)
        SET_TARGET_PROPERTIES(${name} PROPERTIES DEFINE_SYMBOL MEDWRAPPER_V2_1_EXPORTS)
        ENDIF(name STREQUAL MEDWrapper_V2_1)
        ''')
        newlines.append(r'''
        IF(name STREQUAL med_V2_1)
        SET_TARGET_PROPERTIES(${name} PROPERTIES DEFINE_SYMBOL MEDWRAPPER_V2_1_CORE_EXPORTS)
        ENDIF(name STREQUAL med_V2_1)
        ''')
        newlines.append(r'''
        IF(name STREQUAL MEDWrapper_V2_2)
        SET_TARGET_PROPERTIES(${name} PROPERTIES DEFINE_SYMBOL MEDWRAPPER_V2_2_EXPORTS)
        ENDIF(name STREQUAL MEDWrapper_V2_2)
        ''')
        newlines.append(r'''
        IF(name STREQUAL MEDWrapper)
        SET_TARGET_PROPERTIES(${name} PROPERTIES DEFINE_SYMBOL MEDWRAPPER_FACTORY_EXPORTS)
        ENDIF(name STREQUAL MEDWrapper)
        ''')
        newlines.append(r'''
        IF(name STREQUAL interpkernel)
        SET_TARGET_PROPERTIES(${name} PROPERTIES DEFINE_SYMBOL INTERPKERNEL_EXPORTS)
        ENDIF(name STREQUAL interpkernel)
        ''')
        newlines.append(r'''
        IF(name STREQUAL SMESHControls)
        SET_TARGET_PROPERTIES(${name} PROPERTIES DEFINE_SYMBOL SMESHCONTROLS_EXPORTS)
        ENDIF(name STREQUAL SMESHControls)
        ''')
        newlines.append(r'''
        IF(name STREQUAL MeshDriver)
        SET_TARGET_PROPERTIES(${name} PROPERTIES DEFINE_SYMBOL MESHDRIVER_EXPORTS)
        ENDIF(name STREQUAL MeshDriver)
        ''')
        newlines.append(r'''
        IF(name STREQUAL MeshDriverMED)
        SET_TARGET_PROPERTIES(${name} PROPERTIES DEFINE_SYMBOL MESHDRIVERMED_EXPORTS)
        ENDIF(name STREQUAL MeshDriverMED)
        ''')
        newlines.append(r'''
        IF(name STREQUAL MeshDriverDAT)
        SET_TARGET_PROPERTIES(${name} PROPERTIES DEFINE_SYMBOL MESHDRIVERDAT_EXPORTS)
        ENDIF(name STREQUAL MeshDriverDAT)
        ''')
        newlines.append(r'''
        IF(name STREQUAL MeshDriverUNV)
        SET_TARGET_PROPERTIES(${name} PROPERTIES DEFINE_SYMBOL MESHDRIVERUNV_EXPORTS)
        ENDIF(name STREQUAL MeshDriverUNV)
        ''')
        newlines.append(r'''
        IF(name STREQUAL MeshDriverSTL)
        SET_TARGET_PROPERTIES(${name} PROPERTIES DEFINE_SYMBOL MESHDRIVERSTL_EXPORTS)
        ENDIF(name STREQUAL MeshDriverSTL)
        ''')
        newlines.append(r'''
        IF(name STREQUAL MEFISTO2D)
        SET_TARGET_PROPERTIES(${name} PROPERTIES DEFINE_SYMBOL MEFISTO2D_EXPORTS)
        ENDIF(name STREQUAL MEFISTO2D)
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
            IF(name STREQUAL SalomePyQt)
            INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${name}.so DESTINATION ${DEST} RENAME ${name}.so)
            ENDIF(name STREQUAL SalomePyQt)
            ELSE(BEGIN_WITH_lib)
            IF(WINDOWS)
            INSTALL(TARGETS ${name} DESTINATION lib/python${PYTHON_VERSION}/site-packages/salome)
            INSTALL(FILES ${CMAKE_INSTALL_PREFIX}/lib/python${PYTHON_VERSION}/site-packages/salome/${name}.dll DESTINATION lib/python${PYTHON_VERSION}/site-packages/salome RENAME ${name}_d.pyd)
            ELSE(WINDOWS)
            GET_TARGET_PROPERTY(version ${name} VERSION)
            GET_TARGET_PROPERTY(soversion ${name} SOVERSION)
            INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${name}.so.${version} DESTINATION lib/python${PYTHON_VERSION}/site-packages/salome RENAME ${name}.so.${version})
            INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${name}.so.${version} DESTINATION lib/python${PYTHON_VERSION}/site-packages/salome RENAME ${name}.so.${soversion})
            INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${name}.so.${version} DESTINATION lib/python${PYTHON_VERSION}/site-packages/salome RENAME ${name}.so)
            ENDIF(WINDOWS)
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
        FOREACH(amname ${bin_PROGRAMS})
        ''')
        # --
        newlines.append(r'''
        SET(name "${amname}_exe")
        SET(srcs ${${amname}_SOURCES} ${dist_${amname}_SOURCES})
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
        # --
        newlines.append(r'''
        ENDIF(nb)
        ''')
        # --
        newlines.append(r'''
        ENDFOREACH(amname ${bin_PROGRAMS})
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
            INSTALL(FILES ${f} DESTINATION ${DEST})
            ''')
            pass
        newlines.append(r'''
        ELSE(dummy)
        GET_FILENAME_COMPONENT(ext ${f} EXT)
        IF(ext STREQUAL .qm)
        STRING(REGEX REPLACE .qm .ts input ${f})
        ''')
        if self.module in ["kernel", "gui"]:
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
        from os.path import basename
        if basename(root) == "CVS": continue
        for f in files:
            from os.path import basename
            if basename(f) == "Makefile.am":
                convertAmFile(the_root, root, dirs, files, f, module)
                pass
            pass
        pass
    #
    pass
