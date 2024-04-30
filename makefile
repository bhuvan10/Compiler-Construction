CFLAGS =  -w -O3
CC = gcc
all: driver

driver: lexer.o parser.o driver.o
	$(CC) $(CFLAGS) lexer.o parser.o driver.o -o driver

driver.o: driver.c lexer.o parser.o
	$(CC) $(CFLAGS) -c driver.c -o driver.o 

parser.o: parser.c parserDef.h lexer.o
	$(CC) $(CFLAGS) -c parser.c -o parser.o 

lexer.o: lexer.c lexer.h
	$(CC)  $(CFLAGS) -c lexer.c -o lexer.o  

run: driver
	./driver
clean:
	echo "Cleaning up intermediate files..."
	rm parser.o lexer.o driver.o
