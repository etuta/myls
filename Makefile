ls-file: ls-file.c
	gcc -Wall -pedantic -o ls-file ls-file.c

.PHONY: clean
clean:
	rm -f ls-file
