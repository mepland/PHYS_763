# the program(s) to be built: main

###
# components for each program and the flags
# g++ is the compiler, `root-config ...etc` are ROOT flags, 
#  -lTreePlayer is a linked library
###

main: main.o
	g++ ./bin/main.o -o ./main `root-config --cflags --libs` -lTreePlayer -lHistPainter -g3 -Iinclude

###
# compile the object files from the source files
###
main.o: ./src/main.C
	mkdir -p ./bin
	g++ ./src/main.C -c -Wall -o ./bin/main.o `root-config --cflags` -g3 -Iinclude
# -Wno-unused-variable

###
# in the command line type 'make clean' to remove all compiled
#  pieces of this program
###
clean:
	rm -rf ./bin/main.o ./main
