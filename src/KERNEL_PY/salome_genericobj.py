"""
 When imported this module adds to CORBA objref from GenericObj type
 automatic management of reference count
 The reference count is incremented when the local proxy is created (assignment of __omni_obj attribute)
 and is decremented when the local proxy is deleted (call of __del__) 

 The GenericObj class for proxy is modified by adding two methods : __del__ and __setattr__
 The module must be imported before any other import of SALOME CORBA module
"""
import omniORB
import SALOME

def mydel(self):
    self.Destroy()
    omniORB.CORBA.Object.__del__(self)

def mysetattr(self,attr,value):
    self.__dict__[attr]=value
    if attr == "__omni_obj":
      self.Register()

SALOME._objref_GenericObj.__del__=mydel
SALOME._objref_GenericObj.__setattr__=mysetattr

