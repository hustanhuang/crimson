all:
	cd src && make
	mv src/server .

.PHONY: clean

clean:
	cd src && make clean
	rm -f server *~
