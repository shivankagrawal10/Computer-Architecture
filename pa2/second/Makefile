all: second

second: second.c
	gcc -g -Wall -Werror -fsanitize=address -o second second.c

clean: 
	rm -f second
