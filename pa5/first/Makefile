all: first

first: first.c
	gcc -g -Wall -Werror -fsanitize=address -o first first.c -lm

clean:
	rm -f first
