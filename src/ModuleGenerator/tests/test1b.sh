#!/bin/bash

echo   "test6:

        test derivation Engine::Component
        "

\rm -f my_catalog.xml*


echo  "
--> creation d'un nouveau catalogue avec un composant
"

${ROOT_BUILDDIR}/bin/runIDLparser \
		-Wbcatalog=my_catalog.xml \
		${SRCDIR}/tests/TrucComponent.idl

