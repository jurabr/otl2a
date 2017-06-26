DEBUG=-Wall -ansi -pedantic -O0 -g
DEBUG=-Wall -ansi -pedantic -O3

all: otl2a

otl2a: otl2a.c
	$(CC) $(CFLAGS) $(DEBUG) otl2a.c -o $(@)


clean:
	rm -f otl2a
