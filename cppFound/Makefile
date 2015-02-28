all: quash

quash: main.o commands.o
	g++ -g main.o commands.o -o quash

main.o: main.cpp commands.h
	g++ -c -g main.cpp

commands.o: commands.cpp commands.h
	g++ -c -g commands.cpp

test: all
	./quash

clean:
	rm -f *.o *~ quash
