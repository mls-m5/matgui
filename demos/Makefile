CXX=g++
RUNSTRING=./${TARGET}

SRC_FILES=$(wildcard *-demo.cpp)
ALL_FILES=${wildcard *.cpp *.h}
OBJECTS=$(SRC_FILES:.cpp=.o);
LIBS= -lpthread `sdl2-config --libs` -lGL -lSDL2_ttf -lSDL2_image
CXXFLAGS=-g -std=c++11 -O0 `sdl2-config --cflags` -I"../include/matgui" -I $(LIBS)
DEMOS=$(SRC_FILES:.cpp= )

TARGET=simple
LINKFLAGS=${FLAGS} ../matgui-sdl.a ${LIBS} 

all: ${DEMOS}

#Calculating dependincies
.depend: ${ALL_FILES} Makefile
	@echo calculationg dependencies...
	$(CXX) $(CXXFLAGS) -MM *.cpp > .depend

%-demo: %-demo.o ../matgui-sdl.a
	${CXX} -o $@ ${CXXFLAGS} $< ${LINKFLAGS}
	
-include .depend

#För att kunna köra filen direkt
run: ${TARGET}
	${RUNSTRING}

clean:
	rm -f *.o
	rm -f ${DEMOS}
	rm -f .depend

rebuild: clean ${TARGET}
