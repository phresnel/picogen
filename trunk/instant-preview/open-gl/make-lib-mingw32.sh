g++ -c -Wall -Wextra src/main.cc  -I./include -L/usr/lib -lSDL -lopengl32 -lglu32 
ar -rs -o build/libinstantpreview.a main.o
