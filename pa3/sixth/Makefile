all: sixth

sixth: sixth.c
	gcc -Wall -Werror -fsanitize=address -g -o sixth sixth.c -lm	

clean: 
	rm -f sixth
