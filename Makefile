build:
	gcc *.c -o tema2

test: build
	valgrind ./tema2

clean:
	rm tema2 output*.out