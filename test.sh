#!/bin/sh

force_recompile=false

#Parse command line args
while [ $# -gt 0 ]
do
key="$1"

case $key in
	-debug)
		compile_args="$compile_args -debug"
	;;
	-f|--force)
		force_recompile=true
	;;
	*)
		echo "Invalid argument $1"
		exit 1
	;;
esac
shift

done

#Compile the compiler (lol) if needed or asked for
if [ ! -f vtl -o "$force_recompile" = true ]
then
	./build.sh
fi

#Compile examples
for i in examples/*.vtl
do
	if test -f "$i"
	then
		echo "Compiling $i..."
		./vtl $i $compile_args
	fi
done
