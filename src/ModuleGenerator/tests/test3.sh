#!/bin/bash

echo   "test3:

        remplacement dans un catalogue existant d'un composant existant
        (meme idl)
        "

\rm -f my_catalog.xml*


echo  "
--> creation d'un nouveau catalogue avec un composant
"

${ROOT_BUILDDIR}/bin/runIDLparser \
		-Wbcatalog=my_catalog.xml \
		${ROOT_SRCDIR}/idl/AddComponent.idl

echo  "
--> remplacement du composant dans le catalogue (meme idl)
"

${ROOT_BUILDDIR}/bin/runIDLparser \
		-Wbcatalog=my_catalog.xml \
		${ROOT_SRCDIR}/idl/AddComponent.idl
