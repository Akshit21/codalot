#Makefile

all:
	gcc -o main src/main.c
	
.PHONY
clean:
	rm -rf main.o