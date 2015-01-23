COMPILER=g++
RUNSTRING=./${TARGET}

GUIOBJECTS=view.o knobview.o button.o signal.o linearlayout.o layout.o
BACKENDOBJECTS=draw.o shaderprogram.o
PROGOBJECS=main.o
OBJECTS= ${PROGOBJECS} ${GUIOBJECTS}
LIBS= -lpthread -lsndfile `sdl2-config --libs` -lGL
FLAGS=-g -std=c++11 -Ofast `sdl2-config --cflags`

TARGET=project

all: .depend matgui.a matgui-sdl.a ${TARGET}

#Calculating dependincies
.depend: $(wildcard ./*.cpp ./*.h) Makefile
	$(CXX) $(CFLAGS) -MM *.cpp > ./.dependtmp ${FLAGS}
	cat ./.dependtmp | sed 's/h$$/h \n\t \$(CXX) -c $(FLAGS) $$< -o $$@/' > ./.depend
	rm ./.dependtmp

${TARGET}: ${OBJECTS} #cleancpp
	${COMPILER} ${FLAGS} -o ${TARGET} ${PROGOBJECS} matgui.a matgui-sdl.a ${LIBS}
	
matgui.a: ${GUIOBJECTS}
	ar rvs matgui.a ${GUIOBJECTS}
	
matgui-sdl.a: ${BACKENDOBJECTS}
	ar rvs matgui-sdl.a ${BACKENDOBJECTS}

include .depend

#För att kunna köra filen direkt
run: ${TARGET}
	${RUNSTRING}

clean:
	rm *.a
	rm *.o
	rm .depend

rebuild: clean ${TARGET}
