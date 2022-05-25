CC = gcc 
CFLAGS = -g -Wall -Wextra

maxargs-interp: warmup.o A_grammar.o util.o
	${CC} ${CFLAGS} -o $@ $^

warmup.o: warmup.c 
A_grammar.o: A_grammar.c
util.o: util.c

clean-o:
	rm -f *.o
	
clean:
	rm -f *.o maxargs-interp