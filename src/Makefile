COMPILER=g++

GUIOBJECTS=view.o knobview.o button.o matsig.o linearlayout.o layout.o progressview.o imageview.o
BACKENDOBJECTS=draw.o shaderprogram.o window.o application.o font.o texture.o
OBJECTS= ${PROGOBJECS} ${GUIOBJECTS}
#LIBS= -lpthread `sdl2-config --libs` -lGL -lSDL2_ttf -lSDL2_image
CFLAGS=-g -std=c++11 -O0 `sdl2-config --cflags` -I"../include/matgui" -I.
LIB=../matgui-sdl.a

all: .depend $(LIB)

#Calculating dependincies
.depend: $(wildcard ./*.cpp ./*.h) Makefile
	$(CXX) $(CFLAGS) -MM *.cpp > ./.dependtmp
	cat ./.dependtmp | sed 's/h$$/h \n\t \$(CXX) -c $$(CFLAGS) $$< -o $$@/' > ./.depend
	rm ./.dependtmp

${TARGET}: ${OBJECTS} $(LIB) #cleancpp
	${COMPILER} ${FLAGS} -o ${TARGET} ${PROGOBJECS} $(LIB) ${LIBS}
	
../matgui.a: ${GUIOBJECTS} #without sdl-dependencies
	ar rvs ../matgui.a ${GUIOBJECTS}
	
$(LIB): ${BACKENDOBJECTS} ${GUIOBJECTS}
	ar rvs ../matgui-sdl.a ${BACKENDOBJECTS} ${GUIOBJECTS}

include .depend

clean:
	rm -f $(LIB)
	rm *.o
	rm .depend

rebuild: clean ${TARGET}
