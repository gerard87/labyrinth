all:
	g++ game.cpp maze.cpp -o game -lglut -lGLU -lGL -lm
clean:
	rm game
exec:
	./game 25 51