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
             "COMP_MULT"  : ""
             }

nb_components = 0

#--------------------------------------------------
# extract value of <param_name> from <args> list
# it's proposed that the matching <args> item
# looks like <param_name>=<value>, for example,
# catalog=/tmp/myxml.xml
#--------------------------------------------------
def getParamValue( param_name, default_value, args ):
    pattern=param_name+"="

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
    
    def __init__(self, name = '', content = ''):
        self.name = name
        self.content = content
        self.parent = None
        self.childs = []
        
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
                if i.name == tree.name:
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

        # content == None, don't compare content
        for i in self.childs:
            if (i.name == name):
                if (content is None) | (i.content == content):
                    return i
        return None

    def getNode(self, name, content='', depth=-1):

        # recursive search
        # content == None, don't compare content
        if (self.name == name):
            if (content is None) | (self.content == content):
                return self
            
        if (depth != 0):
            for i in self.childs:
                n = i.getNode(name, content, depth-1)
                if n:  return n #return a value 
            
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
    
#--------------------------------------------------
# implements inParameter tree
#--------------------------------------------------
class inParameter(Tree):
    
    def __init__(self, name=None, type='', comment='unknown'):
        Tree.__init__(self, 'inParameter')
        if name is None:  return
        
        self.addNamedChild('inParameter-type', type)
        self.addNamedChild('inParameter-name', name)
        self.addNamedChild('inParameter-comment', comment)
            
    def merge(self, P):

        T = P.getChild('inParameter-type')
        self.replaceChild(T)
    
#--------------------------------------------------
# implements outParameter tree
#--------------------------------------------------
class outParameter(Tree):
    
    def __init__(self, name=None, type='', comment = 'unknown'):
        
        Tree.__init__(self, 'outParameter')
        if name is None:  return
        
        self.addNamedChild('outParameter-type', type)
        self.addNamedChild('outParameter-name', name)
        self.addNamedChild('outParameter-comment', comment)
            
    def merge(self, P):

        T = P.getChild('outParameter-type')
        self.replaceChild(T)
    
#--------------------------------------------------
# implements service tree
#--------------------------------------------------
class Service(Tree):
    
    def __init__(self, name=None, comment = 'unknown'):
        
        Tree.__init__(self, 'component-service')
        if name is None:  return
        
        self.addNamedChild('service-name', name)
        self.addNamedChild('service-author',common_data["AUTHOR"])
        self.addNamedChild('service-version',common_data["VERSION"])
        self.addNamedChild('service-comment', comment)
        self.addNamedChild('service-by-default', "0")
        self.addNamedChild('inParameter-list')
        self.addNamedChild('outParameter-list')
            
    def createInParameter(self, name, type):
        L = self.getChild('inParameter-list')
        if L is None:
            error ("Service.createInParameter() : 'inParameter-list' is not found"); return None;
        p = inParameter(name, type)
        L.addChild(p)
        return p
    
    def createOutParameter(self, name, type):
        L = self.getChild('outParameter-list')
        if L is None:
            error ("Service.createOutParameter() : 'outParameter-list' is not found"); return None;
        p = outParameter(name, type)
        L.addChild(p)
        return p


    def merge(self, S):
        
        L_ext = S.getChild('inParameter-list')
        L_int = self.getChild('inParameter-list')

        if L_ext is not None and L_int is not None:

            L_merge = Tree('inParameter-list')
        
            for i_ext in L_ext.childs:
                # i_ext = <inParameter>
                n_ext = i_ext.getChild('inParameter-name')
                if n_ext is None:  continue
                present = 0
            
                for i_int in L_int.childs:
                    # i_int = <inParameter>
                    n_int = i_int.getChild('inParameter-name')
                    
                    if n_int is None:  continue
                    
                    if (n_int.content == n_ext.content):
                        present = 1
                        break;
                
                if present :
                        i_int.merge(i_ext)
                        L_merge.addChild(i_int)
                else:
                        L_merge.addChild(i_ext)
                        
            self.replaceChild(L_merge)
            
        else : error("Service.merge(): 'inParameter-list' is not found") #either L_ext or  L_int is None
            
        L_ext = S.getChild('outParameter-list')
        L_int = self.getChild('outParameter-list')

        if L_ext is None or L_int is None:
            error ("Service.merge() : 'outParameter-list' is not found")
            
        L_merge = Tree('outParameter-list')
        
        for i_ext in L_ext.childs:
            #i_ext = <outParameter>
            present = 0
            n_ext = i_ext.getChild('outParameter-name')
            if n_ext is None:   continue
            for i_int in L_int.childs:
                n_int = i_int.getChild('outParameter-name')
                if n_int is None:  continue
                if (n_int.content == n_ext.content):
                    present = 1
                    break;
                
            if present :
                i_int.merge(i_ext)
                L_merge.addChild(i_int)
            else:
                L_merge.addChild(i_ext)
                
        self.replaceChild(L_merge)


#--------------------------------------------------
# implements interface tree
#--------------------------------------------------
class Interface(Tree):
    
    def __init__(self, name=None, comment='unknown'):
               
        Tree.__init__(self)

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

    def merge(self, I):

        L_ext = I.getChild('component-service-list')
        L_int = self.getChild('component-service-list')
      
        if L_ext is None or L_int is None:
           error("Interface.merge() : 'component-service-list' is not found!")
           return
       
        L_merge = Tree('component-service-list')
        
        for i_ext in L_ext.childs:
            
            present = 0
            n_ext = i_ext.getChild('service-name')
            if n_ext is None: continue
            
            for i_int in L_int.childs:
                n_int = i_int.getChild('service-name')
                if n_int is None:  continue
                if (n_int.content == n_ext.content):
                    present = 1
                    break;
                
            if present == 0:
                i_int.merge(i_ext)
                L_merge.addChild(i_int)
            else:
                L_merge.addChild(i_ext)
                
        self.replaceChild(L_merge)


#--------------------------------------------------
# implements Component tree
#--------------------------------------------------
class Component(Tree):
    def __init__(self):
        Tree.__init__(self, 'component')
                 
        self.addNamedChild('component-name',       common_data["COMP_NAME"]) 
        self.addNamedChild('component-username',   common_data["COMP_UNAME"])
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
                  'component-comment', 'component-multistudy', 'constraint']:
            ext = C.getChild(i)
            int = self.getChild(i)
            if int is None:
                int = ext
            elif ext is not None and len(ext.content):
                int.content = ext.content
                
        L_ext = C.getChild('component-interface-list')
        L_int = self.getChild('component-interface-list')
        if L_ext is None or L_int is None:
            error("Component.merge : No component-interface-list is found")
            return
        L_merge = Tree('component-interface-list')
        
        for i_ext in L_ext.childs:
            present = 0
            n_ext = i_ext.getChild('component-interface-name')

            if n_ext is None:  continue
            
            for i_int in L_int.childs:
                n_int = i_int.getChild('component-interface-name')
                if n_int is None:  continue
                if (n_int.content == n_ext.content):
                    present = 1
                    break;
                
            if present :
                i_int.merge(i_ext)
                L_merge.addChild(i_int)
            else:
                L_merge.addChild(i_ext)
                
        self.replaceChild(L_merge)
    
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
            e = p.addChild(inParameter())
        elif name == 'outParameter':
            e = p.addChild(outParameter())
        else:
            e = p.addNamedChild(name)
        self.list.append(e)
        self.buffer = ''
        
    def endElement(self, name):
        self.buffer = string.join(string.split(self.buffer), ' ')
        p = self.list[len(self.list)-1]
        p.content = self.buffer
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
        n_ext = i_ext.getChild('component-name')
        if n_ext is None:
            error("Catalog.mergeComponent : 'component-name' is not found")
            return
        for i_int in L_int.childs:
            n_int = i_int.getChild('component-name')
            if n_int is None:  continue
            
            if (n_int.content == n_ext.content):
                present = 1
                break;
                
        if present == 0:
            print '   add component', n_ext.content
            L_int.addChild(i_ext)
        else:
            print '   replace component', n_ext.content
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
                
            Comp = Component()
            
            self.currentInterface = Comp.createInterface(node.identifier())
        
            for c in node.callables():
                if isinstance(c, idlast.Operation):
                    c.accept(self)

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
    print args
    
    CatalogFileName=getParamValue("catalog", "CatalogModulePersonnel.xml", args)
    print CatalogFileName
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
