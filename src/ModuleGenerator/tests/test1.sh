#!/bin/bash

echo   "test1:

         creation d'un nouveau catalogue avec un composant unique
         dans un fichier idl
         "

\rm -f my_catalog.xml*

${ROOT_BUILDDIR}/bin/runIDLparser \
		-Wbcatalog=my_catalog.xml \
		${ROOT_SRCDIR}/idl/AddComponent.idl

