CFLAGS = -Wall -g -Icryptominisat/include -DXOR_SUPPORT
LFLAGS = -Wl,-rpath=./cryptominisat/lib -Lcryptominisat/lib
LIBS = -lcryptominisat4
COMP = g++
#COMP = clang++-3.5

MODULE = $(shell find module/*.cpp -type f | sed "s/.cpp/.o/g")
PRINTER = $(shell find printer/*.cpp -type f | sed "s/.cpp/.o/g")

all: cryptominisat/lib/libcryptominisat4.so knf_gen knf_gen_test

#%.o: %.c
#	gcc -c $(CFLAGS) -o $@ $<

%.o: %.cpp
	$(COMP) -c $(CFLAGS) -o $@ $<

knf_gen: knf_gen.o $(MODULE) $(PRINTER)
	$(COMP) $(LFLAGS) -o $@ $^ $(LIBS)

knf_gen_test: knf_gen_test.o $(MODULE) $(PRINTER)
	$(COMP) $(LFLAGS) -o $@ $^ $(LIBS)

clean:
	-rm -f knf_gen knf_gen_test *.o module/*.o printer/*.o

clobber: clean
	-rm -rf cryptominisat
	-rm -rf cryptominisat4-4.2.0
	-rm -f *.tt *.dimacs *.log

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
