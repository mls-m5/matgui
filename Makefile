.PHONY: all lib demos tests clean

all: lib demos tests
	
debug: target=debug
debug: all

lib:
	make -C src/ ${target}

demos: lib
	make -C demos/
	
tests: lib
	make -C tests/
	
clean:
	make -C src/ clean
	make -C demos/ clean
	make -C tests/ clean
