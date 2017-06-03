CFLAGS=-Wall -Wextra
ifdef RELEASE
	CFLAGS += -O2
else
	#CFLAGS += -lm -ggdb3 -fsanitize=address
	CFLAGS += -lm -ggdb -std=c99
endif

all: leitor

leitor: main.o exibidor.o util.o leitor_exibidor.o fileio.o
	gcc $(CFLAGS) main.o exibidor.o util.o leitor_exibidor.o fileio.o -o leitorexibidor

main.o: main.c
	gcc $(CFLAGS) -c main.c

leitor_exibidor.o: leitor_exibidor.c
	gcc $(CFLAGS) -c leitor_exibidor.c

exibidor.o: exibidor.c
	gcc $(CFLAGS) -c exibidor.c

util.o: util.c
	gcc $(CFLAGS) -c util.c

fileio.o: fileio.c
	gcc $(CFLAGS) -c fileio.c


clean:
	find . -name '*.o' -exec rm '{}' \;
	find . -name 'leitorexibidor' -exec rm '{}' \;