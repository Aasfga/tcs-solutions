si.out : si.x si.in
	./si.x < si.in | tee $@

si.x : si.o main.o
	g++ $^ -o $@

si.o : si.asm
	nasm $^ -felf64 -o $@

main.o : main.cpp
	g++ -std=c++0x -c $^ -O2 -ggdb -o $@
