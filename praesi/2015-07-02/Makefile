TNAME = praesi

all: $(TNAME).pdf

$(TNAME).pdf: $(TNAME).tex
	pdflatex $(TNAME).tex
	pdflatex $(TNAME).tex

clean:
	rm -f $(TNAME).tex.backup $(TNAME).aux $(TNAME).log $(TNAME).nav $(TNAME).out $(TNAME).snm $(TNAME).toc

clobber: clean
	rm -f $(TNAME).pdf

.PHONY: all clean
