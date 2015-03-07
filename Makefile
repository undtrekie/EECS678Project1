project1: main.o
	gcc -g main.o -o quash
	vi README.md

main.o: main.c
	gcc -g -c main.c

clean:
	rm quash *.o
