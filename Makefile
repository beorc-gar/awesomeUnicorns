C_FILES = src/awesomeUnicorns.c src/gameFunctions.c src/graphics.c src/minion.c src/point.c src/tower.c src/fileParsing.c
O_FILES = awesomeUnicorns.o gameFunctions.o graphics.o minion.o point.o tower.o fileParsing.o
H_FILES = include/awesomeUnicorns.h include/graphics.h include/minion.h include/point.h include/point.h include/structs.h include/tower.h include/fileParsing.h
FLAGS = -std=c99 -Wall -pedantic -Iinclude

game: $(O_FILES)
	gcc $(O_FILES) $(FLAGS) -lncurses -o bin/awesomeUnicorns

cleanBuild: $(O_FILES)
	rm *.o
	rm bin/awesomeUnicorns
	gcc $(O_FILES) $(FLAGS) -lncurses -o bin/awesomeUnicorns
awesomeUnicorns.o: src/awesomeUnicorns.c src/gameFunctions.c gameFunctions.o $(H_FILES)
	gcc src/awesomeUnicorns.c $(FLAGS) -c

gameFunctions.o: $(C_FILES) $(H_FILES)
	gcc src/gameFunctions.c $(FLAGS) -c

graphics.o: src/graphics.c src/point.c include/graphics.h include/point.h include/structs.h
	gcc src/graphics.c $(FLAGS) -c

minion.o: src/minion.c include/minion.h include/structs.h
	gcc src/minion.c $(FLAGS) -c

point.o: src/point.c include/point.h
	gcc src/point.c $(FLAGS) -c

tower.o: src/tower.c src/point.c include/tower.h include/point.h include/structs.h
	gcc src/tower.c $(FLAGS) -c

fileParsing.o: src/fileParsing.c src/tower.c include/structs.h include/fileParsing.h include/tower.h
	gcc src/fileParsing.c $(FLAGS) -c

cleanO:
	rm *.o

clean:
	rm *.o
	rm bin/awesomeUnicorns
