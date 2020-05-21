CXX=g++
RUNSTRING=./${TARGET}

SRC_FILES=$(wildcard *-demo.cpp)
ALL_FILES=${wildcard *.cpp *.h}
OBJECTS=$(SRC_FILES:.cpp=.o);
LIBS= -lpthread `sdl2-config --libs` -lGL -lSDL2_ttf -lSDL2_image
CXXFLAGS=-g -std=c++14 -O0 `sdl2-config --cflags` -I"../include/" -I $(LIBS)
CXXFLAGS+= -MMD -MF $*.d # For calculating dependencies

DEMOS=$(SRC_FILES:.cpp= )

TARGET=simple
LINKFLAGS=${FLAGS} ../matgui-sdl.a ${LIBS} 

all: ${DEMOS}

%-demo: %-demo.o ../matgui-sdl.a
	${CXX} -o $@ ${CXXFLAGS} $< ${LINKFLAGS}
	
-include *.d

#För att kunna köra filen direkt
run: ${TARGET}
	${RUNSTRING}

clean:
	rm -f *.o
	rm -f *.d
	rm -f ${DEMOS}

rebuild: clean ${TARGET}


.SECONDARY: ${OBJECTS} # Prevent make from removing them
