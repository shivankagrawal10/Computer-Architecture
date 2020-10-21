all: third

third: third.c
	gcc -Wall -Werror -fsanitize=address -g -o third third.c

clean: 
	rm -f third
