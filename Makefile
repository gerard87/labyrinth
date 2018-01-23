all:
	g++ game.cpp maze.cpp directions.cpp point.cpp utils.cpp particle.cpp bullet.cpp minimax.cpp -o game -lglut -lGLU -lGL -lm -ljpeg -std=c++11
clean:
	rm game
exec:
	./game 25 51
debug:
	./game 25 51 -d