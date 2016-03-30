CFLAGS = -Wall -g
COMP = g++
#COMP = clang++-3.5

all: satcoin sha256 satcoin.dimacs sha256.dimacs

satcoin: satcoin.c
	$(COMP) $(CFLAGS) -o $@ $<

sha256: sha256.c
	$(COMP) $(CFLAGS) -o $@ $<

%.dimacs: %.c cbmc/cbmc
	cbmc/cbmc --slice-formula --32 --dimacs --outfile $@ -DCBMC $<

proof: sha256.c cbmc/cbmc
	cbmc --slice-formula --32 -DCBMC $<

clean:
	-rm -f satcoin satcoin.dimacs sha256 sha256.dimacs

clobber: clean
	-rm -rf cbmc

cbmc/cbmc:
	mkdir cbmc
	tar xf cbmc-5-0-linux-64.tgz -C ./cbmc

.PHONY: all clean clobber
