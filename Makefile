COMPILER=g++
RUNSTRING=./${TARGET}

OBJECTS=main.o draw.o shaderprogram.o view.o knobview.o button.o signal.o
LIBS= -lpthread -lsndfile `sdl2-config --libs` -lGL
FLAGS=-g -std=c++11 -Ofast `sdl2-config --cflags`

TARGET=project

all: .depend ${TARGET}

#Calculating dependincies
.depend: $(wildcard ./*.cpp ./*.h) Makefile
	$(CXX) $(CFLAGS) -MM *.cpp > ./.dependtmp ${FLAGS}
	cat ./.dependtmp | sed 's/h$$/h \n\t \$(CXX) -c $(FLAGS) $$< -o $$@/' > ./.depend
	rm ./.dependtmp

${TARGET}: ${OBJECTS} #cleancpp
	${COMPILER} ${FLAGS} -o ${TARGET} ${OBJECTS} ${LIBS}

include .depend

#För att kunna köra filen direkt
run: ${TARGET}
	${RUNSTRING}

clean:
	rm *.o
	rm .depend

rebuild: clean ${TARGET}
