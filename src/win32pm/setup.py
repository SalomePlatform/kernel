from distutils.core import setup, Extension

module1 = Extension('win32pm',
                    libraries = ['kernel32'],
                    sources = ['win32pm.c'])

setup (name = 'win32pm',
       version = '1.0',
       description = 'Win32 process managment package',
       ext_modules = [module1])
