#!/bin/bash

echo   "test2:

        ajout dans un catalogue existant d'un nouveau composant 
        d'un fichier idl + remplacement d'un composant par une 
        nouvelle version
        "

\rm -f my_catalog.xml*

echo  "
--> creation d'un nouveau catalogue avec un composant
"

${ROOT_BUILDDIR}/bin/runIDLparser \
		-Wbcatalog=my_catalog.xml \
		${ROOT_SRCDIR}/idl/AddComponent.idl


echo  "
--> ajout d'un nouveau composant dans le catalogue
"

${ROOT_BUILDDIR}/bin/runIDLparser \
		-Wbcatalog=my_catalog.xml \
		${ROOT_SRCDIR}/idl/SubComponent.idl


echo  "
--> remplacement du premier composant dans le catalogue
"

${ROOT_BUILDDIR}/bin/runIDLparser \
		-Wbcatalog=my_catalog.xml \
		${ROOT_SRCDIR}/idl/AddComponent.idl

