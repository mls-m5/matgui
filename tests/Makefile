CXX=g++
RUNSTRING=./${TARGET}

OBJECTS=signal-test.o font-test.o
LIBS= -lpthread `sdl2-config --libs` -lGL -lSDL2_ttf -lSDL2_image
CFLAGS=-g -std=c++11 -O0 `sdl2-config --cflags` -I"../include/matgui" -Imls-unit-test
TESTS=${OBJECTS:.o= }

LINKFLAGS=${FLAGS} ../matgui-sdl.a ${LIBS} 

all: ${TESTS}
	@echo all tests = ${TESTS}

#Calculating dependincies
.depend: $(wildcard ./*.cpp ./*.h) Makefile
	$(CXX) $(CFLAGS) -MM *.cpp > ./.dependtmp
	cat ./.dependtmp | sed 's/h$$/h \n\t \$(CXX) -c $$(CFLAGS) $$< -o $$@/' > ./.depend
	rm ./.dependtmp

%-test: %-test.o ../matgui-sdl.a
	@echo "test" $<
	${CXX} -o $@ $< ${LINKFLAGS}
	
include .depend

#För att kunna köra filen direkt
run: ${TARGET}
	${RUNSTRING}

clean:
	rm *.o
	rm ${TESTS}
	rm .depend

rebuild: clean ${TARGET}
