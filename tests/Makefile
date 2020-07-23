CXX=g++
RUNSTRING=./${TARGET}

OBJECTS=signal-test.o font-test.o shadertranslate-test.o
LIBS= -lpthread `sdl2-config --libs` -lGL -lSDL2_ttf -lSDL2_image
CXXFLAGS=-g -std=c++14 -O0 `sdl2-config --cflags` -I"../include/" -Imls-unit-test
CXXFLAGS+= -MMD -MF $*.d # For calculating dependencies
TESTS=${OBJECTS:.o= }

LINKFLAGS=${FLAGS} ../matgui-sdl.a ${LIBS} 

all: ${TESTS}

%-test: %-test.o ../matgui-sdl.a
	@echo "test" $<
	${CXX} -o $@ $< ${LINKFLAGS}
	
-include *.d

#För att kunna köra filen direkt
run: ${TARGET}
	${RUNSTRING}

clean:
	rm -f *.o
	rm -f *.d
	rm -f ${TESTS}

rebuild: clean ${TARGET}

.SECONDARY: ${OBJECTS} # Prevent make from removing them
