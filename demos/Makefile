CXX=g++
RUNSTRING=./${TARGET}

SRC_FILES=$(wildcard *-demo.cpp)
OBJECTS=$(SRC_FILES:.cpp=.o);
LIBS= -lpthread `sdl2-config --libs` -lGL -lSDL2_ttf -lSDL2_image
CXXFLAGS=-g -std=c++11 -O0 `sdl2-config --cflags` -I"../include/matgui" -I $(LIBS)
DEMOS=$(SRC_FILES:.cpp= );

TARGET=simple
LINKFLAGS=${FLAGS} ../matgui-sdl.a ${LIBS} 

all: ${DEMOS}

#Calculating dependincies
.depend: $(wildcard ./*.cpp ./*.h) Makefile
	$(CXX) $(CFLAGS) -MM *.cpp > ./.dependtmp
	cat ./.dependtmp | sed 's/h$$/h \n\t \$(CXX) -c $$(CFLAGS) $$< -o $$@/' > ./.depend
	rm ./.dependtmp

%-demo: %-demo.o ../matgui-sdl.a
	${CXX} -o $@ $< ${LINKFLAGS}
	
#include .depend

#För att kunna köra filen direkt
run: ${TARGET}
	${RUNSTRING}

clean:
	rm -f *.o
	rm -f ${DEMOS}
	rm -f .depend

rebuild: clean ${TARGET}
