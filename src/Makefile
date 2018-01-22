
cparser: parser.o lexer.o ast.o decltable.o astprint.o asttraverse.o main.o util.o stack.o table.o scopestack.o symboltable.o -lfl
	cc -ggdb $^ -o $@; \
	make clean;

%.o: %.c %.h
	cc -ggdb -Wall -Wextra -c $<

parser.c parser.h: parser.y
	bison -d -v -r all -t -o parser.c $<

tokens.h: parser.h
	mv $< tokens.h

lexer.c: lexer.l tokens.h
	flex -t $< > $@

.PHONY: clean install
clean:
	rm *.o lexer.c parser.c parser.output
all:
	make
