CXX=g++
RUNSTRING=./${TARGET}

OBJECTS=simple.o game.o
LIBS= -lpthread `sdl2-config --libs` -lGL -lSDL2_ttf -lSDL2_image
CFLAGS=-g -std=c++11 -O0 `sdl2-config --cflags` -I"../include" -I
DEMOS=simple game

TARGET=simple
LINKFLAGS=${FLAGS} ../matgui-sdl.a ${LIBS} 

all: ${DEMOS}

#Calculating dependincies
.depend: $(wildcard ./*.cpp ./*.h) Makefile
	$(CXX) $(CFLAGS) -MM *.cpp > ./.dependtmp
	cat ./.dependtmp | sed 's/h$$/h \n\t \$(CXX) -c $$(CFLAGS) $$< -o $$@/' > ./.depend
	rm ./.dependtmp

${TARGET}: simple.o ../matgui-sdl.a
	${CXX} -o ${TARGET} simple.o ${LINKFLAGS}
	
game: game.o ../matgui-sdl.a
	${CXX} -o game game.o ${LINKFLAGS}
	
#include .depend

#För att kunna köra filen direkt
run: ${TARGET}
	${RUNSTRING}

clean:
	rm *.o
	rm ${DEMOS}
	rm .depend

rebuild: clean ${TARGET}
