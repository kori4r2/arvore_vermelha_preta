CFLAGS = -g -Wall

all:
	gcc $(CFLAGS) -o program *.c -I./

clean:
	rm -f program
	clear

run:
	./program

.zip:
	zip red_black_tree.zip *.c *.h Makefile

debug: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < case.in > test.out
	#diff test.out case.out

