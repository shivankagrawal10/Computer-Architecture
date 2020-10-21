all: first

first: first.c
	gcc -g -Wall -Werror -fsanitize=address -o first first.c

clean: 
	rm -f first
