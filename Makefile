CFLAGS = -Wall -pedantic -std=c++11 -O2
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf

OBJS = build/main.o build/game.o build/video.o build/figures.o build/random.o
TARGET = main

${TARGET}: ${OBJS}
	g++ -o $@ $+ ${LIBS}
 
build/%.o: src/%.cpp
	g++ -g -c ${CFLAGS} -o $@ $+ 


.PHONY:
	clean
	
clean:
	rm ${OBJS} ${TARGET}

