
pathInc = -I/usr/include/directfb/direct -I/usr/include/directfb -I./Source/
Libs = `sdl2-config --cflags`
DLibs = `sdl2-config --libs` -L/usr/local/lib  -lSDL2_mixer

CC = clang++ $(CXXFLAGS) -c -Wall -std=c++14 -ferror-limit=100 $(pathInc) $(Libs)
LD = clang++ $(LDFLAGS) obj/*.o -lpthread $(DLibs) 

all : fodder

fodder: 	main
		mv *.o obj/
		$(LD) -o Run/OpenFodder

main:
		git log -n 1 --pretty="const char* gitversion=\"%h\";" > ./Source/gitver.hpp
		$(CC) Source/*.cpp Source/PC/*.cpp Source/Amiga/*.cpp Source/Structures/*.cpp Source/Utils/*.cpp Source/Map/*.cpp
		mkdir -p obj

clean:
		rm obj/*.o


