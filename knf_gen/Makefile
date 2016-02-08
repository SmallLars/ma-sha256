CFLAGS = -Wall -g -Icryptominisat/include
CFLAGS += -DXOR_SUPPORT
#CFLAGS += -DADDITIONAL_CLAUSES
LFLAGS = -Wl,-rpath=./cryptominisat/lib -Lcryptominisat/lib
LIBS = -lcryptominisat4 -lpthread
COMP = g++
#COMP = clang++-3.5

#SOLVERVERSION = 4.2.0
#SOLVERVERSION = 4.4.0
SOLVERVERSION = 4.5.3

COMMON = $(shell find common/*.cpp -type f | sed "s/.cpp/.o/g")
MODULE = $(shell find module/*.cpp -type f | sed "s/.cpp/.o/g")
PRINTER = $(shell find printer/*.cpp -type f | sed "s/.cpp/.o/g")

all: cryptominisat/lib/libcryptominisat4.so $(patsubst %.cpp,%,$(wildcard *.cpp))

%.o: %.cpp
	$(COMP) -c $(CFLAGS) -o $@ $<

cc_miner: cc_miner.o $(COMMON) $(MODULE) $(PRINTER)
	$(COMP) $(LFLAGS) -o $@ $^ $(LIBS)

cc_miner_2: cc_miner_2.o $(COMMON) $(MODULE) $(PRINTER)
	$(COMP) $(LFLAGS) -o $@ $^ $(LIBS)

sha256_%: sha256_%.o $(COMMON) $(MODULE) $(PRINTER)
	$(COMP) $(LFLAGS) -o $@ $^ $(LIBS)

clausecollector: clausecollector.o $(COMMON) $(MODULE) $(PRINTER)
	$(COMP) $(LFLAGS) -o $@ $^ $(LIBS)

clausesorter: clausesorter.o $(COMMON) $(MODULE) $(PRINTER)
	$(COMP) $(LFLAGS) -o $@ $^ $(LIBS)

clausechecker: clausechecker.o $(COMMON) $(MODULE) $(PRINTER)
	$(COMP) $(LFLAGS) -o $@ $^ $(LIBS)

dimacsprinter: dimacsprinter.o $(COMMON) $(MODULE) $(PRINTER)
	$(COMP) $(LFLAGS) -o $@ $^ $(LIBS)

modulchecker: modulchecker.o $(COMMON) $(MODULE) $(PRINTER)
	$(COMP) $(LFLAGS) -o $@ $^ $(LIBS)

graphprinter: graphprinter.o $(COMMON) $(MODULE) $(PRINTER)
	$(COMP) $(LFLAGS) -o $@ $^ $(LIBS)

distancechecker: distancechecker.o $(COMMON) $(MODULE) $(PRINTER)
	$(COMP) $(LFLAGS) -o $@ $^ $(LIBS)

bitcoin: bitcoin.o $(COMMON) $(MODULE) $(PRINTER)
	$(COMP) $(LFLAGS) -o $@ $^ $(LIBS)

benchmark: benchmark.o $(COMMON) $(MODULE) $(PRINTER)
	$(COMP) $(LFLAGS) -o $@ $^ $(LIBS)

test_module: test_module.o $(COMMON) $(MODULE) $(PRINTER)
	$(COMP) $(LFLAGS) -lrt -lm -o $@ $^ $(LIBS)

clean:
	-rm -f $(patsubst %.cpp,%,$(wildcard *.cpp)) *.o common/*.o module/*.o printer/*.o

clobber: clean
	-rm -rf cryptominisat
	-rm -rf cryptominisat4-$(SOLVERVERSION)
	-rm -f *.tt *.log

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
