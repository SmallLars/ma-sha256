all: $(shell find *.eqn -type f | sed "s/.eqn/.cnf/g")

%.cnf: %.eqn
	eqntott -l $< | ./../../espresso/espresso/source/src/espresso -Dexact -epos > $@

clean:
	-rm -f *.cnf

.PHONY: all clean
