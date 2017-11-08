
pathInc = -I/usr/include/directfb/direct -I/usr/include/directfb -I./Source/
Libs = `sdl2-config --cflags`
DLibs = `sdl2-config --libs` -L/usr/local/lib  -lSDL2_mixer

CC = clang++ $(CXXFLAGS) -c -Wall -std=c++14 -ferror-limit=100 $(pathInc) $(Libs)
LD = clang++ $(LDFLAGS) obj/*.o $(DLibs)

all : fodder

fodder: 	main
		mv *.o obj/
		$(LD) -o Run/OpenFodder

main:
		$(CC) Source/*.cpp Source/PC/*.cpp Source/Amiga/*.cpp
		mkdir -p obj

clean:
		rm obj/*.o


