# daniisakov@gmail.com

all: build

build: main.cpp game.o player.o governor.o baron.o general.o judge.o merchant.o spy.o
	g++ -std=c++17 -g -O0 -o Main main.cpp game.o player.o governor.o baron.o general.o judge.o merchant.o spy.o -I/opt/homebrew/Cellar/sfml@2/2.6.2_1/include -L/opt/homebrew/Cellar/sfml@2/2.6.2_1/lib -lsfml-graphics -lsfml-window -lsfml-system

Main: build
	./Main

game.o: game.cpp
	g++ -std=c++17 -g -O0 -c game.cpp -o game.o

player.o: player.cpp
	g++ -std=c++17 -g -O0 -c player.cpp -o player.o

governor.o: governor.cpp
	g++ -std=c++17 -g -O0 -c governor.cpp -o governor.o

baron.o: baron.cpp
	g++ -std=c++17 -g -O0 -c baron.cpp -o baron.o

general.o: general.cpp
	g++ -std=c++17 -g -O0 -c general.cpp -o general.o

judge.o: judge.cpp
	g++ -std=c++17 -g -O0 -c judge.cpp -o judge.o

merchant.o: merchant.cpp
	g++ -std=c++17 -g -O0 -c merchant.cpp -o merchant.o

spy.o: spy.cpp
	g++ -std=c++17 -g -O0 -c spy.cpp -o spy.o

test: test_coup.cpp game.o player.o governor.o baron.o general.o judge.o merchant.o spy.o
	g++ -std=c++17 -o test test_coup.cpp game.o player.o governor.o baron.o general.o judge.o merchant.o spy.o

valgrind: test
	valgrind --leak-check=full --suppressions=nvidia_suppression.supp ./test

valgrind-detailed: Main
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=nvidia_suppression.supp ./Main

valgrind-no-suppress: Main
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./Main

clean:
	rm -f *.o Main test main 
