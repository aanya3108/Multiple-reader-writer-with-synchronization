all:
	gcc readerwriter.c -o readerwriter -lpthread

clean:
	rm readerwriter