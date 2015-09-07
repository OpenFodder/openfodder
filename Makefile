pathInc = -I/usr/include/directfb/direct -I/usr/include/directfb -I./Source/
Libs = `sdl2-config --cflags`
DLibs = `sdl2-config --libs` -L/usr/local/lib
 
CC = clang++ -c -Wall -std=c++0x $(pathInc) $(Libs)
LD = clang++ obj/*.o $(DLibs)

all : fodder 


fodder : main
	mv *.o obj/
	$(LD) -o run/creep

main:
	$(CC) Source/*.cpp Source/PC/*.cpp Source/Amiga/*.cpp


clean :
	rm obj/*.o
	
