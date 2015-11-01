all: $(shell find *.eqn -type f | sed "s/.eqn/.cnf/g")

%.cnf: %.tt espresso/espresso
	./espresso/espresso -Dexact -epos $< > $@

%.tt: %.eqn
	eqntott -l $< > $@

espresso/espresso:
	tar xf espresso-modern.tar.gz
	cd espresso/source;./configure;make
	cp espresso/source/src/espresso espresso/espresso

clean:
	-rm -f *.cnf

clobber: clean
	-rm -rf espresso

.PHONY: all clean clobber
