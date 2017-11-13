all:
	g++ game.cpp maze.cpp directions.cpp point.cpp utils.cpp particle.cpp -o game -lglut -lGLU -lGL -lm -std=c++11
clean:
	rm game
exec:
	./game 25 51