#!/bin/sh

copy_old="Copyright \(C\) 2024"
copy_new="Copyright (C) 2024-2025-2025"


#ATTENTION: the following code modifies the files
num_changes=$(find . -type f -exec grep -qsE "${copy_old}" {} \; -exec sed -E -i "s%${copy_old}%${copy_new}%g" {} \; -print | wc -l)

printf "\033[01;32m%16s\033[0m files modified...\n" ${num_changes}

