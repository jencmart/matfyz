
all: meth.pbm
	./pbm-compose.pl

clean:
	rm meth*pbm

%.pbm : %.rle
	./rle2pbm.pl $< > $@

.PHONY: clean all
