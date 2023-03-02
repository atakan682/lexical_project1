!bin/bash

#Author information
#Author names: Mustafa Atakan Tan (mtan17@csu.fullerton.edu)
#	  	         Joshua Germing (Joshuagerming@gmail.com)
#		           Valentyna Shyyan (ValentynaShyyan@gmail.com)  
#Course: 	CPSC323
#Project #:	RAT23S
#File name:	project.sh

echo script file for Project3 begin
rm *.o
rm *.out

g++ -g -c -std=c++11 main.cpp Lex.h Lex.cpp
g++ *.o -o a.out
rm *.o
echo finished.
