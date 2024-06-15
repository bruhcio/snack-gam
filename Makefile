all:
	g++ -o snakegame main.cpp Score.h Score.cpp Map.h Map.cpp Snake.h Snake.cpp -lncursesw
