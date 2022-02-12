COMPILER=g++

GUIOBJECTS=../matgui_views.o
BACKENDOBJECTS=../matgui_backend.o
OBJECTS= ${PROGOBJECS} ${GUIOBJECTS}
CXXFLAGS=-std=c++17 `sdl2-config --cflags` -I"../include/" -I. -fPIC
CXXFLAGS+= -MMD -MF $*.d # For calculating dependencies
LIB=../matgui-sdl.a
SHARED=../libmatgui.so

LIBS= -lpthread `sdl2-config --libs` -lGL -lSDL2_ttf -lSDL2_image

all: $(LIB) ${SHARED}

debug: CXXFLAGS+=-g -O0 -Wall
debug: all

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
