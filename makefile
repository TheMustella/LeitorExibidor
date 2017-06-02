CFLAGS=-Wall -Wextra
ifdef RELEASE
	CFLAGS += -O2
else
	#CFLAGS += -lm -ggdb3 -fsanitize=address
	CFLAGS += -lm -ggdb -std=c99 -pthread
endif

all: leitor

leitor: leitor_exibidor.o
	gcc $(CFLAGS) leitor_exibidor.o -o ltex

leitor_exibidor.o: leitor_exibidor.c
	gcc $(CFLAGS) -c leitor_exibidor.c

clean:
	find . -name '*.o' -exec rm '{}' \;
	find . -name 'servidord' -exec rm '{}' \;

#prog:
#@gcc -Wall -std=c99 experiencias.c -o exe

#@rm -f exe
