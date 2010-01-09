g++ -c -Wall -Wextra -I./include -L/usr/lib -lSDL -lGL -lGLU src/main.cc 
mkdir -p build
ar -rs -o build/libinstantpreview.a main.o
rm main.o