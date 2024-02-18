windows:
	g++ -I src/include -L src/lib -o main_windows main.cpp -lmingw32 -lSDL2main -lSDL2

linux:
	g++ main.cpp -g -pedantic -o main_linux -lSDL2main -lSDL2
