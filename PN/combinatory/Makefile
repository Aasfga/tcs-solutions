ASM := $(wildcard *.asm)
OBJ := $(patsubst %.asm,%.o,${ASM})
EXE := test.x

${EXE} : ${OBJ} mallocx.o
	g++ -static $^ -o $@

${OBJ} : %.o : %.asm
	nasm $< -O0 -felf64 -g -o $@

mallocx.o : mallocx.cpp
	g++ -std=c++0x -c $^ -O2 -ggdb -o $@

clean :
	rm -f *.x *.o

.PHONY: clean
