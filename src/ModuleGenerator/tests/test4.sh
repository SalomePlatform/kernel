#!/bin/bash

echo   "test4:

        remplacement dans un catalogue existant d'un composant existant
        (l'idl du composant a ete modifie)
        "

\rm -f my_catalog.xml*

echo  "
--> creation d'un nouveau catalogue avec un composant
"

${ROOT_BUILDDIR}/bin/runIDLparser \
		-Wbcatalog=my_catalog.xml \
		${ROOT_SRCDIR}/idl/AddComponent.idl


mv my_catalog.xml C1
sed -e 's/arameter-comment></arameter-comment>aaa</'   < C1 > C2
sed -e 's/service-comment></service-comment>bbb</'     < C2 > C1
sed -e 's/interface-comment></interface-comment>ccc</' < C1 > C2
sed -e 's/component-comment></component-comment>ddd</' < C2 > C1
mv C1 my_catalog.xml
\rm -f C2

 echo  "
 --> remplacement du composant dans le catalogue (autre idl)
"

${ROOT_BUILDDIR}/bin/runIDLparser \
		-Wbcatalog=my_catalog.xml \
		${SRCDIR}/tests/Truc2Component.idl

