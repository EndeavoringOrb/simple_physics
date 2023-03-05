all: compile link

compile:
	g++ -c balls.cpp -IC:\Users\aaron\CODING\SFML-2.5.1\include -DSFML_STATIC

link:
	g++ balls.o -o balls -LC:\Users\aaron\CODING\SFML-2.5.1\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows -lsfml-main

clean:
	rm -f balls *.o