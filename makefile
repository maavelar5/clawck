all : main.cc
	g++ -w main.cc -lSDL2 -lSDL2_image -o clawck.exe
