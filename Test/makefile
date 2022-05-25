CC = gcc 
CFLAGS = -g -Wall -Wextra

CII_LIST := hanson/list.o
CII_MEM  := hanson/mem.o
CII_EXC  := hanson/except.o
CII_ASSE := hanson/assert.o


wordcount: lex.yy.o
	${CC} ${CFLAGS} -o wordcount lex.yy.o $(CII_LIST) $(CII_MEM) $(CII_EXC) $(CII_ASSE)

lex.yy.o: lex.yy.c
	${CC} ${CFLAGS} -c lex.yy.c

lex.yy.c:
	flex wordcount.l
	
clean:
	rm -f *.o wordcount lex.yy.c