#!/bin/bash

echo   "test4:

        creation d'un catalogue avec tous les idl de Salome 
	(au 29/3/2002)
        "

\rm -f my_catalog.xml*

for i in ${ROOT_SRCDIR}/idl/*.idl
do
	echo
	echo "            processing " `basename $i`
	echo
	${ROOT_BUILDDIR}/bin/runIDLparser \
		-Wbcatalog=my_catalog.xml $i
done
