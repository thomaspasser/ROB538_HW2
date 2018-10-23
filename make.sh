#!/bin/bash
if [ "$1" == "1" ];
then
	echo "Compiling problem 1"
	g++ -Wall problem1.cpp Domain.cpp Target.cpp Player.cpp -o problem1 -std=c++11 -O3 -I include/ -g
fi

if [ "$1" == "2" ];
then
        echo "Compiling problem 2"
        g++ -Wall problem2.cpp Domain.cpp Target.cpp Player.cpp -o problem2 -std=c++11 -O3 -I include/ -g
fi

if [ "$1" == "3" ];
then
        echo "Compiling problem 3"
        g++ -Wall problem3.cpp Domain.cpp Target.cpp Player.cpp -o problem3 -std=c++11 -O3 -I include/ -g
fi

if [ "$1" == "all" ];
then
        echo "Compiling problem 1"
        g++ -Wall problem1.cpp Domain.cpp Target.cpp Player.cpp -o problem1 -std=c++11 -O3 -I include/
        echo "Compiling problem 2"
        g++ -Wall problem2.cpp Domain.cpp Target.cpp Player.cpp -o problem2 -std=c++11 -O3 -I include/
        echo "Compiling problem 3"
        g++ -Wall problem3.cpp Domain.cpp Target.cpp Player.cpp -o problem3 -std=c++11 -O3 -I include/
fi

