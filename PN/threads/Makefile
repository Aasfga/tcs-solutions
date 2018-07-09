CFLAGS := -O0

SC := $(wildcard *.c)
SX := $(wildcard *.cpp)
SA := $(wildcard *.asm)
OC := $(patsubst %,%.o,${SC})
OX := $(patsubst %,%.o,${SX})
OA := $(patsubst %,%.o,${SA})

solution.x : ${OC} ${OX} ${OA}
	g++ -static $^ -o $@

${OA} : %.o : %
	nasm $< -felf64 -o $@

${OC} : %.o : %
	gcc -c $< ${CFLAGS} -o $@

${OX} : %.o : %
	g++ -std=c++0x -c $< ${CFLAGS} -o $@

clean :
	rm -f *.o *.x

.PHONY : clean
