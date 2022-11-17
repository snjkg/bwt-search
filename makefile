bwtsearch: bwtsearch.c
	gcc bwtsearch.c -o bwtsearch -O2 -ftree-parallelize-loops=4 -floop-parallelize-all
