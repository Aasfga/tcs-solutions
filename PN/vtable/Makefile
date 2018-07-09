CPP := $(wildcard test*.cpp)
EXE := $(patsubst %.cpp,%,${CPP})

all : ${EXE}

${EXE} : % : %.cpp inject.h
	g++ -std=c++11 $< -fno-devirtualize -O2 -fdump-class-hierarchy -o $@
	mv $*.cpp*.class $*.dump

clean :
	rm -f ${EXE} *.dump

.PHONY : all clean
