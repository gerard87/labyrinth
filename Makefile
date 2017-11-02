maze: maze.c game.c
	gcc game.c maze.c -o game -lglut -lGLU -lGL -lm
clean:
	rm game
