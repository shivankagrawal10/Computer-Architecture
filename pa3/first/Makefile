all: first

first: first.c
	gcc -Wall -Werror -fsanitize=address -g -o first first.c

clean: 
	rm -f first
