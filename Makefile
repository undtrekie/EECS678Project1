project1: main.o
	gcc -g main.o -o project1
	editor README.md

main.o: main.c
	gcc -g -c main.c

clean:
	rm project1 *.o
