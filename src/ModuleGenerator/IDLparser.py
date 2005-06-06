#  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
#  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
#
#
#
#  File   : IDLparser.py
#  Module : SALOME

import string, sys, fpformat, re, os
import xml.sax
import pdb

from xml.sax.handler import *
from omniidl import idlast, idltype, idlvisitor, idlutil, output

# parameters not found in IDL file, user's specified in optional parameters
common_data={"AUTHOR"     : "",
             "ICON"       : "",
             "VERSION"    : "",
             "COMP_TYPE"  : "",
             "COMP_NAME"  : "",
             "COMP_UNAME" : "",
             "COMP_MULT"  : "",
             "COMP_IMPL"  : ""
             }

nb_components = 0

#--------------------------------------------------
# extract value of <param_name> from <args> list
# it's proposed that the matching <args> item
# looks like <param_name>=<value>, for example,
# catalog=/tmp/myxml.xml
#--------------------------------------------------
def getParamValue( param_name, default_value, args ):
    pattern="^"+param_name+"="

    res = default_value        #initial value
    for opt in args:
        s = re.compile(pattern).search(opt)
        if s:
            res = opt[s.end():]
            break     #found

    return res    


#--------------------------------------------------
# print error message
#--------------------------------------------------
def error (message):
    print "ERROR : ", message


#--------------------------------------------------
# base class implementing tree
#--------------------------------------------------
class Tree:
    
    def __init__(self, name = '', content = '', key = None):
        self.name = name
        self.content = content
        self.key = key
        self.parent = None
        self.childs = []
        self.comments = []
        
    def addChild(self, tree):
        if tree is not None: 
            self.childs.append(tree)
            tree.parent = self
        return tree

    def addNamedChild(self, name, content = ''):
        return self.addChild(Tree(name, content))

    def replaceChild(self, tree):
         if tree is not None:
            pos = 0
            for i in self.childs:
                if (i.name == tree.name) & ((i.key is None) | (i.key == tree.key)):
                    self.childs.pop(pos)
                    self.childs.insert(pos, tree)
                    return tree
                pos += 1

         return self.addChild(tree)
       
    def insertFirstChild(self, tree):
        if tree is not None:
            self.childs.insert(0, tree)
        return tree
    
    def insertFirstNamedChild(self, name, content = ''):
        return self.insertFirstChild(Tree(name, content))

    def output_xml(self, f, depth=0):
        d = depth
        if self.name != '':
            s = string.ljust('', 4*depth)
            s += '<' + self.name + '>'
            if self.content != '':
                s +=  self.content
            else:
                if len(self.childs) > 0:
                    s += '\n'
            f.write(s)
            d +=  1
            
        for i in self.childs:
            i.output_xml(f, d)
            
        if self.name != '':
            s = '</' + self.name + '>\n'
            if len(self.childs) > 0 :
                s = string.ljust('', 4*depth) + s
            f.write(s)

    def Dump(self, levels=-1, depth=0):
        #Dumps the tree contents
        
        if levels == 0: return
        
        s = string.ljust('', 4*depth)
        print s, self, self.content
        for i in self.childs:
            i.Dump(levels-1, depth+1)

    def parents(self):
        #Returns list of the parents
        l = []
        p = self.parent
        while p:
            l.append(p)
            l.append(p.name)
            p = p.parent
        return l
        
    def getChild(self, name, content=None):
        # return child node with a given name
        # if content == None, don't compare contents
        for i in self.childs:
            if (i.name == name):
                if (content is None) | (i.content == content):
                    return i
        return None

    def getNode(self, name, content='', depth=-1):
        # recursive search of a node with a given name
        # content == None, don't compare content
        if (self.name == name):
            if (content is None) | (self.content == content):
                return self
            
        if (depth != 0):
            for i in self.childs:
                n = i.getNode(name, content, depth-1)
                if n:  return n 
            
        return None

    def __repr__(self):
        s = '<'
        if self.name != '':
            s += self.name
        else:
            s +=  'None'
        s += '>'
        return s

    def merge(self, t):
        pass

    def mergeChilds(self, t, list):
        L_ext = t.getChild(list)
        L_int = self.getChild(list)

        L_merge = Tree(list)
        
        for i_ext in L_ext.childs:
            k_ext = i_ext.key
            if k_ext is None:  continue
            present = 0
            
            for i_int in L_int.childs:
                k_int = i_int.key
                if k_int is None:  continue
                
                if (k_int == k_ext):
                    present = 1
                    break;
                
            if present :
                i_int.merge(i_ext)
                L_merge.addChild(i_int)
            else:
                L_merge.addChild(i_ext)
                
        self.replaceChild(L_merge)
            

    
#--------------------------------------------------
# implements parameter tree
#--------------------------------------------------
class parameter(Tree):
    
    def __init__(self, name=None, mode = 'in', type='', comment='unknown'):
        Tree.__init__(self, mode + 'Parameter', key=name)
        self.mode = mode
        if name is None:  return
        
        self.addNamedChild(mode + 'Parameter-name', name)
        self.addNamedChild(mode + 'Parameter-type', type)
        self.addNamedChild(mode + 'Parameter-comment', comment)
        
    def merge(self, P):

        self.mode = P.mode
        self.replaceChild(P.getChild(P.mode + 'Parameter-name'))
        self.replaceChild(P.getChild(P.mode + 'Parameter-type'))
        C = P.getChild(P.mode + 'Parameter-comment')
        if C.content != 'unkonwn':
            self.replaceChild(C)
    
#--------------------------------------------------
# implements dataStreamParameter tree
#--------------------------------------------------
class dataStreamParameter(parameter):
    
    def __init__(self, name=None, mode='in', type='', dependency='', comment='unknown'):
        parameter.__init__(self, name, mode, type, comment)
        if name is None:  return
        
        self.addNamedChild(mode + 'Parameter-dependency', dependency)
        self.mode = mode
            
    def merge(self, P):

        parameter.merge(self, P)
        self.replaceChild(P.getChild(mode + 'Parameter-dependency'))


def parseComment(comment):

    spaces = '[\t\n ]*'
    word = spaces + '([a-zA-Z][a-zA-Z0-9_]*)' + spaces
    
    result = []
    type = None
    key = None
    
    ## match :  // followed by a 'DataStreamPorts' string,
    ## the service name, and a list of ports
    pattern = '// *DataStreamPorts{,1}' + word
    m = re.match(pattern, comment)

    ## if there is a match, parse remaining part of comment
    if m:
        ## service
        type = 'DataStreamPorts'
        key = m.group(1)
        
        sPorts = comment[m.end():]
        pattern = word + '\('+word+','+word +','+word+'\)' \
                  + spaces + ',{,1}' + spaces
        while len(sPorts) > 0:
            ## process next DataStreamPort
            ## match a definition like xx(a,b,c) with a possible trailing ,
            ## returns a tuple (xx, a, b, c) and
            ## the remaining part of input string
            m = re.match(pattern, sPorts)
            if m is None:
                raise LookupError, \
                      'format error in DataStreamPort definition : '+sPorts
            sPorts = sPorts[m.end():]
            result.append(m.groups())
            
    return type, key, result;

#--------------------------------------------------
# implements service tree
#--------------------------------------------------
class Service(Tree):
    
    def __init__(self, name=None, comment = 'unknown'):
        
        Tree.__init__(self, 'component-service', key=name)
        if name is None:  return
        
        self.addNamedChild('service-name', name)
        self.addNamedChild('service-author',common_data["AUTHOR"])
        self.addNamedChild('service-version',common_data["VERSION"])
        self.addNamedChild('service-comment', comment)
        self.addNamedChild('service-by-default', "0")
        self.addNamedChild('inParameter-list')
        self.addNamedChild('outParameter-list')
        self.addNamedChild('DataStream-list')
            
    def createInParameter(self, name, type):
        L = self.getChild('inParameter-list')
        p = parameter(name, 'in', type)
        L.replaceChild(p)
        return p
    
    def createOutParameter(self, name, type):
        L = self.getChild('outParameter-list')
        p = parameter(name, 'out', type)
        L.replaceChild(p)
        return p

    def createDataStreamParameter(self, p):
        L = self.getChild('DataStream-list')
        p = dataStreamParameter(p[0], p[2], p[1], p[3])
        L.replaceChild(p)
        return p
            
    def merge(self, S):

        self.replaceChild(S.getChild('service-author'))
        self.replaceChild(S.getChild('service-version'))
        self.replaceChild(S.getChild('service-by-default'))
        C = S.getChild('service-comment')
        if C.content != 'unkonwn':
            self.replaceChild(C)
            
        for L in ['inParameter-list', 'outParameter-list', 'DataStream-list']:
           self.mergeChilds(S, L)
            


#--------------------------------------------------
# implements interface tree
#--------------------------------------------------
class Interface(Tree):
    
    def __init__(self, name=None, comment='unknown'):
               
        Tree.__init__(self, key=name)

        if name is None:  return
        
        self.addNamedChild('component-interface-name', name)
        self.addNamedChild('component-interface-comment', comment);
        self.addNamedChild('component-service-list')
            
    def createService(self, name):
        L = self.getChild('component-service-list')

        if L is None:
            error ("Interface.createService() : 'component-service-list' is not found")
            return None

        s = Service(name)
        L.addChild(s)
        return s

    def findService(self, key):
        L = self.getChild('component-service-list')
        for S in L.childs:
            if S.key == key:
                return S
        return None
    
    def merge(self, I):

        C = S.getChild('component-interface-comment')
        if C.content != 'unkonwn':
            self.replaceChild(C)

        self.mergeChilds(I, 'component-service-list')
    
    def processDataStreams(self):
        for sComment in self.comments:

            type, key, result = parseComment(sComment)

            if type == 'DataStreamPorts':
                Service = self.findService(key)
                if Service is None:
                    raise LookupError, \
                          'service ' + key + \
                          ' not found in interface : ' + self.key
                for p in result:
                ## process next DataStreamPort
                    Service.createDataStreamParameter(p)


#--------------------------------------------------
# implements Component tree
#--------------------------------------------------
class Component(Tree):
    def __init__(self, name=None):
        Tree.__init__(self, 'component', key=name)
        if name is None:  return
                 
# ASV : fix for bug PAL8922 (Component name indicated by user in GUI is not taken into account
	if common_data["COMP_NAME"] != '':
	    self.addNamedChild('component-name', common_data["COMP_NAME"])
	else:
	    self.addNamedChild('component-name', name)

# ASV : if user name is NOT set, then use component-name instead.  Else - default.
        if common_data["COMP_UNAME"] != '':
            self.addNamedChild('component-username',   common_data["COMP_UNAME"])
        else:
            if common_data["COMP_NAME"] != '':
                self.addNamedChild('component-username', common_data["COMP_NAME"] )
            else:
                self.addNamedChild('component-username',   name)
            
        self.addNamedChild('component-type',       common_data["COMP_TYPE"])
        self.addNamedChild('component-author',     common_data["AUTHOR"])
        self.addNamedChild('component-version',    common_data["VERSION"])
        self.addNamedChild('component-comment',    'unknown')
        self.addNamedChild('component-multistudy', common_data["COMP_MULT"])
        self.addNamedChild('component-icone',      common_data["ICON"])
        self.addNamedChild('constraint')
        self.addNamedChild('component-interface-list')
            
    def createInterface(self, name):
        L = self.getChild('component-interface-list')
        if L is None:
            error("createInterface: No component-interface-list is found")
            return None
        i = Interface(name)
        L.addChild(i)
        return i

    def merge(self, C):

        for i in ['component-username', 'component-author',
                  'component-type', 'component-icone', 'component-version',
                  'component-multistudy', 'constraint']:
            ext = C.getChild(i)
            int = self.getChild(i)
            if int is None:
                int = ext
            elif ext is not None and len(ext.content):
                int.content = ext.content
                
        Cc = C.getChild('component-comment')
        if Cc.content != 'unkonwn':
            self.replaceChild(Cc)
                
        self.mergeChilds(C, 'component-interface-list')
    
#--------------------------------------------------
# implements document tree
#--------------------------------------------------
class Catalog(ContentHandler, Tree):
    def __init__(self, filename = None):
        Tree.__init__(self)
        self.buffer = ''
        self.list = []
        if (filename):
            parser = xml.sax.make_parser()
            parser.setContentHandler(self)
            parser.parse(filename)
        else:
            t = self.addNamedChild('begin-catalog')
            t.addNamedChild('component-list')

        n = self.getChild('begin-catalog')
        if n is None:
            error("Catalog.__init__ : No 'begin-catalog' is found!")
            return
        if n.getChild('path-prefix-list') is None:
            n.insertFirstNamedChild('path-prefix-list')
        if n.getChild('component-list') is None:
            n.addNamedChild('component-list')
            
    def removeComponent(self, name):
        complist = self.getNode('component-list')
        idx = 0
        if complist is None:
            print "Catalog.removeComponent() : 'component-list' is not found"
            return
        for comp in complist.childs:
            cname = comp.getChild('component-name')
            if cname is not None:
                if cname.content == name:
                    complist.childs.pop(idx)
                    print "Component " + name + " is removed"
            idx += 1       
 
    def startDocument(self):
        self.list.append(self)
    
    def startElement(self, name, attrs):
        p = self.list[len(self.list)-1]
        if name == 'component':
            e = p.addChild(Component())
        elif name == 'component-interface-name':
            e = p.addNamedChild(name)
        elif name == 'component-service':
            e = p.addChild(Service())
        elif name == 'inParameter':
            e = p.addChild(parameter(mode='in'))
        elif name == 'outParameter':
            e = p.addChild(parameter(mode='out'))
        else:
            e = p.addNamedChild(name)
        self.list.append(e)
        self.buffer = ''
        
    def endElement(self, name):
        self.buffer = string.join(string.split(self.buffer), ' ')
        p = self.list[len(self.list)-1]
        p.content = self.buffer
        if name == 'component':
            p.key = p.getChild('component-name').content
        self.buffer = ''
        e = self.list.pop()
        
    def characters(self, ch):
        self.buffer += ch

    def mergeComponent(self, comp):
        
        L_int = self.getNode('component-list')
        if   L_int is None:
            error("Catalog.mergeComponent : 'component-list' is not found")
            return
        
        i_ext = comp
        present = 0
        n_ext = i_ext.key
        for i_int in L_int.childs:
            if (i_int.key == n_ext):
                present = 1
                break;
                
        if present == 0:
            print '   add component', i_ext.getChild('component-name').content
            L_int.addChild(i_ext)
        else:
            print '   replace component', i_ext.getChild('component-name').content
            i_int.merge(i_ext)
            

            

# IDL file reader

ttsMap = {
    idltype.tk_void:       "void",
    idltype.tk_short:      "short",
    idltype.tk_long:       "long",
    idltype.tk_ushort:     "unsigned short",
    idltype.tk_ulong:      "unsigned long",
    idltype.tk_float:      "float",
    idltype.tk_double:     "double",
    idltype.tk_boolean:    "boolean",
    idltype.tk_char:       "char",
    idltype.tk_octet:      "octet",
    idltype.tk_any:        "any",
    idltype.tk_TypeCode:   "CORBA::TypeCode",
    idltype.tk_Principal:  "CORBA::Principal",
    idltype.tk_longlong:   "long long",
    idltype.tk_ulonglong:  "unsigned long long",
    idltype.tk_longdouble: "long double",
    idltype.tk_wchar:      "wchar"
    }


#--------------------------------------------------
# class ModuleCatalogVisitor
#--------------------------------------------------
class ModuleCatalogVisitor (idlvisitor.AstVisitor):
    
    def __init__(self, catalog):
        self.catalog = catalog
        self.EngineType = 0
        
    def visitAST(self, node):
        for n in node.declarations():
            n.accept(self)
            
    def visitModule(self, node):
        for n in node.definitions():
            n.accept(self)
                
    def visitInterface(self, node):
            
        if node.mainFile():

            self.EngineType = 0
            
            for i in node.inherits():
                s = i.scopedName();
                if ((s[0] == "Engines") & (s[1] == "Component")):
                    self.EngineType = 1; break
                
            Comp = Component(node.identifier())
            
            self.currentInterface = Comp.createInterface(node.identifier())
        
            for c in node.callables():
                if isinstance(c, idlast.Operation):
                    c.accept(self)

            for c in node.declarations():
                if isinstance(c, idlast.Struct):
                    c.accept(self)
                
            for i in node.comments():
                self.currentInterface.comments.append(str(i))

            self.currentInterface.processDataStreams()
            
            if (self.EngineType):    
                global nb_components
                nb_components = nb_components + 1
                self.catalog.mergeComponent(Comp)

            self.EngineType = 0
            

    def visitOperation(self, node):

        self.currentService = self.currentInterface.createService \
                                       (node.identifier())

        for c in node.parameters():
            c.accept(self)
            
        node.returnType().accept(self)
        if (self.currentType != "void"):
            self.currentService.createOutParameter \
                ("return", self.currentType)
            
        for i in node.comments():
            self.currentInterface.comments.append(str(i))
        

    def visitDeclaredType(self, type):
        self.currentType = type.name()
            
    def visitBaseType(self, type):
        self.currentType = ttsMap[type.kind()]
    
    def visitStringType(self, type):
        self.currentType = "string"
        
    def visitParameter(self, node):
        node.paramType().accept(self)
        if node.is_in():
            self.currentService.createInParameter \
                     (node.identifier(), self.currentType)
        if node.is_out():
            self.currentService.createOutParameter \
                     (node.identifier(), self.currentType)
        
#--------------------------------------------------
# parse idl and store xml file
#--------------------------------------------------
def run(tree, args):
    
    CatalogFileName=getParamValue("catalog", "CatalogModulePersonnel.xml", args)
    if re.compile(".*?.xml$").match(CatalogFileName, 1) is None:
        CatalogFileName = CatalogFileName + '.xml'

    #=========  Read parameters  ======================    
    common_data["ICON"]       = getParamValue("icon",       "",                args)
    common_data["AUTHOR"]     = getParamValue("author",     os.getenv("USER"), args)
    common_data["VERSION"]    = getParamValue("version",    "1",               args)
    common_data["COMP_NAME"]  = getParamValue("name",       "",                args) 
    common_data["COMP_UNAME"] = getParamValue("username",   "",                args)
    common_data["COMP_TYPE"]  = getParamValue("type",       "OTHER",           args)
    common_data["COMP_MULT"]  = getParamValue("multistudy", "1",               args)
    common_data["COMP_IMPL"]  = getParamValue("impltype",   "1",               args)

    print common_data
    
    remove_comp = getParamValue("remove", "", args)
    
    #==================================================    
    
    if (os.path.exists(CatalogFileName)):
        print "Importing", CatalogFileName
        C = Catalog(CatalogFileName)
    else:
        print "Creating ",CatalogFileName
        C = Catalog()

    print "Reading idl file"
    
    visitor = ModuleCatalogVisitor(C)
    tree.accept(visitor)

##    C.Dump()
    
    if remove_comp :
        C.removeComponent(remove_comp)
    
    if (os.path.exists(CatalogFileName)):
        print "Updating", CatalogFileName
        CatalogFileName_old = CatalogFileName + '_old'
        os.rename(CatalogFileName, CatalogFileName_old)
    else:
        CatalogFileName_old = ""
        print "Writing", CatalogFileName
        
    CatalogFileName_new = CatalogFileName + '_new'
    f=open(CatalogFileName_new, 'w')
    f.write("<?xml version='1.0' encoding='us-ascii' ?>\n\n")
    C.output_xml(f)
    f.close()

    os.rename(CatalogFileName_new, CatalogFileName)
    if ((CatalogFileName_old != "") & os.path.exists(CatalogFileName_old)):
        os.unlink(CatalogFileName_old)
        
    print


if __name__ == "__main__":
    print
    print "Usage : omniidl -bIDLparser [-I<catalog files directory>]* -Wbcatalog=<my_catalog.xml>[,icon=<pngfile>][,version=<num>][,author=<name>][,name=<component_name>][,username=<component_username>][,multistudy=<component_multistudy>] <file.idl>"
    print


