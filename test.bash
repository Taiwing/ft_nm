#!/bin/env bash

MY_NM="./ft_nm"
OG_NM="$(which nm)"
OPTS=""

for OPTS in "" "-a" "-g" "-u" "-ag" "-au" "-gu" "-agu" "-agurp"; do
	echo
	echo "Testing with options: $OPTS"
	for file in $(ls bin/*); do
	    echo "Testing $file"
	    $MY_NM $OPTS $file > my.out
	    $OG_NM $OPTS $file > og.out
	    diff my.out og.out
	done
done
