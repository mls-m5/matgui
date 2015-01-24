
all:
	make -C src/
	make -C demos/

lib:
	make -C src/

demos:
	make -C demos/
	
clean:
	make -C src/ clean
	make -C demos/ clean