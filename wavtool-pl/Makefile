CC = gcc
wavtool-pl:wavtool-pl.o wfd.o wfh.o
	$(CC) $(LDFLAGS) wavtool-pl.o wfd.o wfh.o -static -o wavtool-pl -lsndfile
clean:
	rm -f wavtool-pl.o wfd.o wfh.o wavtool-pl
