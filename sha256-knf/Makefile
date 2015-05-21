CFLAGS = -Wall -g -Icryptominisat/include
LFLAGS = -Wl,-rpath=./cryptominisat/lib -Lcryptominisat/lib
LIBS = -lcryptominisat4

all: cryptominisat/lib/libcryptominisat4.so sha256

%.o: %.c
	gcc -c $(CFLAGS) -o $@ $<

%.o: %.cpp
	g++ -c $(CFLAGS) -o $@ $<

sha256: sha256.o
	g++ $(LFLAGS) -o $@ $^ $(LIBS)

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

startsql:
	mysql -u root -p cmsat < cmsat_tablestructure.sql

depend:
	sudo apt-get install cmake libboost-program-options-dev zlib1g-dev mysql-server libmysql++-dev libm4ri-dev

.PHONY: all clean clobber depend startsql
