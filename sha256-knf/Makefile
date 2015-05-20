CFLAGS = -Wall -g -Wl,-rpath,/home/lars/Dokumente/Masterarbeit/sha256-knf/cryptominisat/lib -I/home/lars/Dokumente/Masterarbeit/sha256-knf/cryptominisat/include -L/home/lars/Dokumente/Masterarbeit/sha256-knf/cryptominisat/lib -lcryptominisat4

all: cryptominisat/lib/libcryptominisat4.so sha256

%.o: %.c
	gcc -c $(CFLAGS) -o $@ $<

%.o: %.cpp
	g++ -c $(CFLAGS) -o $@ $<

sha256: sha256.o cryptominisat/lib/libcryptominisat4.so
	g++ $(CFLAGS) -o $@ $^

clean:
	-rm sha256 *.o

clobber: clean
	-rm -r cryptominisat
	-rm -r cryptominisat4-4.2.0

cryptominisat/lib/libcryptominisat4.so:
	tar xjf cryptominisat-4.2.0.tar.bz2
	mkdir cryptominisat
	cd cryptominisat;cmake ../cryptominisat4-4.2.0
	cd cryptominisat;make

.PHONY: all clean clobber
