#!/bin/sh

./build.sh

for i in examples/*.vtl
do
	if test -f "$i"
	then
		echo "$i"
		./vtl $i
	fi
done
