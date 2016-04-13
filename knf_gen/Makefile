CFLAGS = -Wall -g -Icryptominisat/include
CFLAGS += -DXOR_SUPPORT
CFLAGS += -DADDITIONAL_CLAUSES
LFLAGS = -Wl,-rpath=./cryptominisat/lib -Lcryptominisat/lib
LIBS = -lcryptominisat4 -lpthread
COMP = g++
#COMP = clang++-3.5

#SOLVERVERSION = 4.2.0
#SOLVERVERSION = 4.4.0
SOLVERVERSION = 4.5.3

COMMON = $(shell find common/*.cpp -type f | sed "s/.cpp/.o/g")
MODULE = $(shell find module/*.cpp -type f | sed "s/.cpp/.o/g")
PRINTER = $(shell find collector/*.cpp -type f | sed "s/.cpp/.o/g")
BINARYS = $(patsubst %.cpp,%.bin,$(wildcard *.cpp))

LOUTB = \033[1;32mLinking \033[0m
LOUTD = \033[1;32m ......\033[0m
LOUTF = \033[1;32m done.\033[0m

BOUTB = \033[1;34mBuilding \033[0m
BOUTD = \033[1;34m ......\033[0m
BOUTF = \033[1;34m done.\033[0m

all: cryptominisat/lib/libcryptominisat4.so $(BINARYS)

%.o: %.cpp
	@echo -n "$(BOUTB)"; echo -n "$@"; echo -n "$(BOUTD)"
	@$(COMP) -c $(CFLAGS) -o $@ $<
	@echo "$(BOUTF)"

%.bin: %.o $(COMMON) $(MODULE) $(PRINTER)
	@echo -n "$(LOUTB)"; echo -n "$@"; echo -n "$(LOUTD)"
	@$(COMP) $(LFLAGS) -o $@ $^ $(LIBS)
	@echo "$(LOUTF)"

test_module.bin: test_module.o $(COMMON) $(MODULE) $(PRINTER)
	@echo -n "$(LOUTB)"; echo -n "$@"; echo -n "$(LOUTD)"
	@$(COMP) $(LFLAGS) -lrt -lm -o $@ $^ $(LIBS)
	@echo "$(LOUTF)"
	rm -f test_module.o

clean:
	-rm -f $(BINARYS) *.o common/*.o module/*.o collector/*.o

clobber: clean
	-rm -rf cryptominisat
	-rm -rf cryptominisat4-$(SOLVERVERSION)
	-rm -f *.tt *.log

cryptominisat/lib/libcryptominisat4.so:
	tar xjf cryptominisat-$(SOLVERVERSION).tar.bz2
	mkdir cryptominisat
	cd cryptominisat;cmake -DSTATS=1 ../cryptominisat4-$(SOLVERVERSION)
	cd cryptominisat;make -j4

startsql:
	mysql -u root -p cmsat < cmsat_tablestructure.sql

depend:
	sudo apt-get install build-essential cmake valgrind libboost-program-options-dev zlib1g-dev mysql-server libmysqlclient-dev libsqlite3-dev libm4ri-dev libtbb-dev

.PHONY: all clean clobber depend startsql
