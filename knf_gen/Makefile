CFLAGS = -Wall -g -Icryptominisat/include -DXOR_SUPPORT
LFLAGS = -Wl,-rpath=./cryptominisat/lib -Lcryptominisat/lib
LIBS = -lcryptominisat4 -lpthread
COMP = g++
#COMP = clang++-3.5

SOLVERVERSION = 4.2.0

COMMON = $(shell find common/*.cpp -type f | sed "s/.cpp/.o/g")
MODULE = $(shell find module/*.cpp -type f | sed "s/.cpp/.o/g")
PRINTER = $(shell find printer/*.cpp -type f | sed "s/.cpp/.o/g")

all: cryptominisat/lib/libcryptominisat4.so cc_miner test_module sha256_1 sha256_2 sha256_3 sha256_4

#%.o: %.c
#	gcc -c $(CFLAGS) -o $@ $<

%.o: %.cpp
	$(COMP) -c $(CFLAGS) -o $@ $<

cc_miner: cc_miner.o $(COMMON) $(MODULE) $(PRINTER)
	$(COMP) $(LFLAGS) -o $@ $^ $(LIBS)

test_module: test_module.o $(COMMON) $(MODULE) $(PRINTER)
	$(COMP) $(LFLAGS) -lrt -lm -o $@ $^ $(LIBS)

sha256_1: sha256_1.o $(COMMON) $(MODULE) $(PRINTER)
	$(COMP) $(LFLAGS) -o $@ $^ $(LIBS)

sha256_2: sha256_2.o $(COMMON) $(MODULE) $(PRINTER)
	$(COMP) $(LFLAGS) -o $@ $^ $(LIBS)

sha256_3: sha256_3.o $(COMMON) $(MODULE) $(PRINTER)
	$(COMP) $(LFLAGS) -o $@ $^ $(LIBS)

sha256_4: sha256_4.o $(COMMON) $(MODULE) $(PRINTER)
	$(COMP) $(LFLAGS) -o $@ $^ $(LIBS)

clean:
	-rm -f cc_miner test_module sha256_1 sha256_2 sha256_3 sha256_4 *.o common/*.o module/*.o printer/*.o

clobber: clean
	-rm -rf cryptominisat
	-rm -rf cryptominisat4-$(SOLVERVERSION)
	-rm -f *.tt *.dimacs *.log

cryptominisat/lib/libcryptominisat4.so:
	tar xjf cryptominisat-$(SOLVERVERSION).tar.bz2
	mkdir cryptominisat
	cd cryptominisat;cmake ../cryptominisat4-$(SOLVERVERSION)
	cd cryptominisat;make

startsql:
	mysql -u root -p cmsat < cmsat_tablestructure.sql

depend:
	sudo apt-get install build-essential cmake valgrind libboost-program-options-dev zlib1g-dev mysql-server libmysqlclient-dev libsqlite3-dev libm4ri-dev libtbb-dev

.PHONY: all clean clobber depend startsql
