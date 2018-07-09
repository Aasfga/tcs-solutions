ASM := $(wildcard *.asm)
OBJ := $(patsubst %.asm,%.o,${ASM})
EXE := $(patsubst %.asm,%,${ASM})

all : ${EXE}

${EXE} : % : %.o
	ld $^ -s -o $@

${OBJ} : %.o : %.asm
	nasm $< -felf64 -o $@

clean :
	rm -f ${EXE} ${OBJ}

.PHONY: all clean
