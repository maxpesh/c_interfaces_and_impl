#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

int linenum;

int getword(FILE *fp, char *buf, size_t size);
void doubleword(char *, FILE *);

int main(int argc, char **argv)
{
	for (size_t i = 1; i < argc; ++i) {
		FILE *fp = fopen(argv[i], "r");
		if (fp == NULL) {
			fprintf(stderr, "%s: can't open '%s' (%s)\n",
			  argv[0], argv[i], strerror(errno));
			return EXIT_FAILURE;
		} else {
			doubleword(argv[i], fp);
			fclose(fp);
		}
	}
	if (argc == 1)
		doubleword(NULL, stdin);
	return EXIT_SUCCESS;
}

void doubleword(char *name, FILE *fp)
{
	char prev[128], word[128];

	linenum = 1;
	prev[0] = '\0';
	while (getword(fp, word, sizeof word)) {
		if (isalpha(word[0]) && strcmp(prev, word) == 0) {
			while (isalpha(word[0]) && strcmp(prev, word) == 0) {
				getword(fp, word, sizeof word);
			}
			if (name)
				printf("%s:", name);
				printf("%d: %s\n", linenum, prev);
		}
		strcpy(prev, word);
	}
}

int getword(FILE *fp, char *buf, size_t size)
{
	int c;

	c = getc(fp);
	for (; c != EOF && isspace(c); c = getc(fp)) {
		if (c == '\n')
			++linenum;
	}
	size_t i = 0;
	for (; c != EOF && !isspace(c); c = getc(fp)) {
		if (i < size - 1)
			buf[i++] = tolower(c);
	}
	if (i < size)
		buf[i] = '\0';
	if (c != EOF)
		ungetc(c, fp);
	return buf[0] != '\0';
}
