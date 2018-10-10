COMPILER=g++

GUIOBJECTS=view.o knobview.o button.o toggleview.o sliderview.o matsig.o linearlayout.o layout.o progressview.o imageview.o label.o
BACKENDOBJECTS=draw.o shaderprogram.o window.o application.o font.o texture.o bitmapfont.o paint.o
OBJECTS= ${PROGOBJECS} ${GUIOBJECTS}
#LIBS= -lpthread `sdl2-config --libs` -lGL -lSDL2_ttf -lSDL2_image
CXXFLAGS=-g -std=c++11 -O0 `sdl2-config --cflags` -I"../include/matgui" -I. -fPIC
LIB=../matgui-sdl.a

all: .depend $(LIB)

#Calculating dependincies
.depend: $(wildcard ./*.cpp ./*.h) Makefile
	$(CXX) $(CXXFLAGS) -MM *.cpp > ./.dependtmp
	cat ./.dependtmp | sed 's/h$$/h \n\t \$(CXX) -c $$(CXXFLAGS) $$< -o $$@/' > ./.depend
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
	rm -f *.o
	rm -f .depend

rebuild: clean ${TARGET}
