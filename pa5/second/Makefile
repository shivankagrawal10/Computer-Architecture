all: second

second: second.c
	gcc -g -Wall -Werror -fsanitize=address -o second second.c -lm

clean:
	rm -f second
