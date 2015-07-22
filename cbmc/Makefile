CFLAGS = -Wall -g
COMP = g++
#COMP = clang++-3.5

all: sha256 dimacs

sha256: sha256.c
	$(COMP) $(CFLAGS) -o $@ $<

dimacs: sha256.c cbmc/cbmc
	cbmc/cbmc --slice-formula --32 --dimacs --outfile sha256.dimacs -DCBMC $<

proof: sha256.c cbmc/cbmc
	cbmc --slice-formula --32 -DCBMC $<

clean:
	-rm -f sha256 sha256.dimacs

clobber: clean
	-rm -rf cbmc

cbmc/cbmc:
	mkdir cbmc
	tar xf cbmc-5-0-linux-64.tgz -C ./cbmc

.PHONY: all clean clobber
