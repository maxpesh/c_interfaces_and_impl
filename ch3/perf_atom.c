#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "atom.h"

int main(void) {
	char *line = NULL;
	size_t linecap = 0;
	ssize_t linelen;

	clock_t begin = clock();
	while ((linelen = getline(&line, &linecap, stdin)) > 0) {
		Atom_string(line);
		line  = NULL;
	}
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("time spent: %g\n", time_spent);
	return EXIT_SUCCESS;
}
