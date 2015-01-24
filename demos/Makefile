COMPILER=g++
RUNSTRING=./${TARGET}

OBJECTS=simple.o
LIBS= -lpthread -lsndfile `sdl2-config --libs` -lGL -lSDL2_ttf -lSDL2_image
CFLAGS=-g -std=c++11 -O0 `sdl2-config --cflags` -I"../include"

TARGET=simple

all: ${TARGET}

#Calculating dependincies
.depend: $(wildcard ./*.cpp ./*.h) Makefile
	$(CXX) $(CFLAGS) -MM *.cpp > ./.dependtmp
	cat ./.dependtmp | sed 's/h$$/h \n\t \$(CXX) -c $$(CFLAGS) $$< -o $$@/' > ./.depend
	rm ./.dependtmp

${TARGET}: ${OBJECTS} ../matgui-sdl.a
	${COMPILER} ${FLAGS} -o ${TARGET} ${OBJECTS} ../matgui-sdl.a ${LIBS}
	
include .depend

#För att kunna köra filen direkt
run: ${TARGET}
	${RUNSTRING}

clean:
	rm *.a
	rm *.o
	rm .depend

rebuild: clean ${TARGET}
