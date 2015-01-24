COMPILER=g++
RUNSTRING=./${TARGET}

GUIOBJECTS=view.o knobview.o button.o signal.o linearlayout.o layout.o progressview.o imageview.o
BACKENDOBJECTS=draw.o shaderprogram.o window.o application.o font.o texture.o
OBJECTS= ${PROGOBJECS} ${GUIOBJECTS}
#LIBS= -lpthread -lsndfile `sdl2-config --libs` -lGL -lSDL2_ttf -lSDL2_image
CFLAGS=-g -std=c++11 -O0 `sdl2-config --cflags` -I"../include"

all: .depend ../matgui.a ../matgui-sdl.a

#Calculating dependincies
.depend: $(wildcard ./*.cpp ./*.h) Makefile
	$(CXX) $(CFLAGS) -MM *.cpp > ./.dependtmp
	cat ./.dependtmp | sed 's/h$$/h \n\t \$(CXX) -c $$(CFLAGS) $$< -o $$@/' > ./.depend
	rm ./.dependtmp

${TARGET}: ${OBJECTS} matgui-sdl.a #cleancpp
	${COMPILER} ${FLAGS} -o ${TARGET} ${PROGOBJECS} matgui-sdl.a ${LIBS}
	
../matgui.a: ${GUIOBJECTS} #without sdl-dependencies
	ar rvs ../matgui.a ${GUIOBJECTS}
	
../matgui-sdl.a: ${BACKENDOBJECTS} ${GUIOBJECTS}
	ar rvs ../matgui-sdl.a ${BACKENDOBJECTS} ${GUIOBJECTS}

include .depend

#För att kunna köra filen direkt
run: ${TARGET}
	${RUNSTRING}

clean:
	rm *.a
	rm *.o
	rm .depend

rebuild: clean ${TARGET}
