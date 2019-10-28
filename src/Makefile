COMPILER=g++

GUIOBJECTS=view.o knobview.o button.o toggleview.o sliderview.o matsig.o linearlayout.o layout.o progressview.o imageview.o label.o
BACKENDOBJECTS=draw.o shaderprogram.o window.o application.o font.o texture.o bitmapfont.o paint.o
OBJECTS= ${PROGOBJECS} ${GUIOBJECTS}
CXXFLAGS=-g -std=c++11 -O0 `sdl2-config --cflags` -I"../include/" -I. -fPIC
CXXFLAGS+= -MMD -MF $*.d # For calculating dependencies
LIB=../matgui-sdl.a
SHARED=../libmatgui.so

LIBS= -lpthread `sdl2-config --libs` -lGL -lSDL2_ttf -lSDL2_image

all: $(LIB) ${SHARED}

${TARGET}: ${OBJECTS} $(LIB) 
	${COMPILER} ${FLAGS} -o ${TARGET} ${PROGOBJECS} $(LIB) ${LIBS}
	
../matgui.a: ${GUIOBJECTS} #without sdl-dependencies
	ar rs ../matgui.a ${GUIOBJECTS}
	
${LIB}: ${BACKENDOBJECTS} ${GUIOBJECTS}
	ar rs ../matgui-sdl.a ${BACKENDOBJECTS} ${GUIOBJECTS}

${SHARED}: ${BACKENDOBJECTS} ${GUIOBJECTS}
	${CXX} -shared -o  ${SHARED} ${BACKENDOBJECTS} ${GUIOBJECTS} ${LIBS}

-include *.d

clean:
	rm -f *.o
	rm -f *.d
	rm -f ${LIB} ${SHARED}

rebuild: clean ${TARGET}



.SECONDARY: ${BACKENDOBJECTS} ${GUIOBJECTS} # Prevent make from removing them
