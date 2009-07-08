g++ -c -Wall -Wextra -I./include -L/usr/lib -lSDL -lGL -lGLU src/main.cc 
ar -rs -o build/libinstantpreview.a main.o
