CFLAGS = -g -Wall

all:
	gcc $(CFLAGS) -o program *.c -I./

clean:
	rm -f program
	rm -f log.txt
	rm -f *.zip
	rm -f test*.out
	clear

run:
	./program

.zip:
	zip red_black_tree.zip *.c *.h Makefile

debug: all
	valgrind --log-file=log.txt --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < 1.in > test1.out
	diff test1.out 1.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < 2.in > test2.out
	diff test2.out 2.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < 3.in > test3.out
	diff test3.out 3.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < 4.in > test4.out
	diff test4.out 4.out
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program < 5.in > test5.out
	diff test5.out 5.out

