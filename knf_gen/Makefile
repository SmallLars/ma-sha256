CFLAGS = -Wall -g -Icryptominisat/include
LFLAGS = -Wl,-rpath=./cryptominisat/lib -Lcryptominisat/lib
LIBS = -lcryptominisat4

all: cryptominisat/lib/libcryptominisat4.so knf_gen

%.o: %.c
	gcc -c $(CFLAGS) -o $@ $<

%.o: %.cpp
	g++ -c $(CFLAGS) -o $@ $<

knf_gen: knf_gen.o
	g++ $(LFLAGS) -o $@ $^ $(LIBS)

clean:
	-rm knf_gen *.o

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
	sudo apt-get install cmake libboost-program-options-dev zlib1g-dev mysql-server libmysql++-dev libm4ri-dev libtbb-dev

.PHONY: all clean clobber depend startsql
