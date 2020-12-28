all: second

second: second.c
	gcc -Wall -Werror -fsanitize=address -g -o second second.c

clean: 
	rm -f second
