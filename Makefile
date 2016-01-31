.PHONY: all lib demos tests clean

all:
	make -C src/
	make -C demos/
	make -C tests/

lib:
	make -C src/

demos:
	make -C demos/
	
tests:
	make -C tests/
	
clean:
	make -C src/ clean
	make -C demos/ clean
	make -C tests/ clean