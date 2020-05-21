#!/bin/bash

cd ClientSide/
echo "compiling ClientSide"
g++ -std=c++11 -c *.cpp && g++ -o Client-Side *.o
echo "removing *.o"
rm *.o
echo "success"

cd ../ServerSide/
echo "compiling ServerSide"
g++ -std=c++11 -c *.cpp && g++ -o Server-Side *.o
echo "removing *.o"
rm *.o
echo "success"

echo "run Server-Side first"
