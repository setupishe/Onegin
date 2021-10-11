
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#define BOTH(a,b,c) ((a == c && a == b) || (a != c && b != c))


#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <sys\stat.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "core.h"



FILE* fastopen(const char *name) {
	assert(name != NULL);

	FILE* fp = NULL;
	int dfp = 0;
	if ((dfp = open(name, O_RDONLY)) == -1) {
		printf("Cannot open file: descriptor creation failed\n");
		return NULL;
	}
	if (!(fp = fdopen(dfp, "r"))) { 
		printf("Cannot open file: file pointer creation failed\n");
		return NULL;
	}

	return fp;
}

int getsize(const char *name) {
	struct stat statfp = {};
	if (stat(name, &statfp) == -1) {
		printf("Cannot check file size");
		return 0;
	}
	return statfp.st_size;
}

char *makebuffer(FILE* fp, int *n_lines, int *n_symbols) {
	assert(fp != NULL);
	assert(n_lines != NULL);
	assert(n_symbols != NULL);

	char c = 0;
	int nlines = 0, nsymbols = 0;


	//printf("size = %d\n", statfp.st_size);
	if ((nsymbols = getsize("hamlet.txt")) == 0) {
		printf("Cannot check file size");
		return NULL;
	}

	char* buffer = (char *)calloc(nsymbols, sizeof(char));
	if (!buffer) {
		printf("Cannot create buffer");
		return NULL;
	}

	c = getc(fp); //fill the buffer
	for (int i = 0; c != EOF; c = getc(fp)) {
		if (c != '\r') {
			if (c == '\n') {
				buffer[i] = '\0';
				nlines++;
			}
			else {
				buffer[i] = c;
			}
		}
		i++;
	}
	buffer[nsymbols - 1] = '\0';
	if (nlines != 0) {
		nlines++;
	}

	*n_lines = nlines;
	*n_symbols = nsymbols;
	return buffer;
}


lline* makeindex(char* buffer, int nlines, int nsymbols) {

	assert(buffer != NULL);
	assert(nlines > 0);
	assert(nsymbols > 0);

	lline *index = (lline*)calloc(nlines, sizeof(lline)); 

	if (!index) {
		printf("Cannot create index");
		return NULL;
	}

	int k = 0;
	if (buffer[0] != '\0') { //check if the first element start the line 
		index[k].ptr = buffer;
		index[k].length = strlen(index[k].ptr);
		k++;
	}
	for (int i = index[0].length; i < nsymbols - 1; ) {
		if (buffer[i + 1] != '\0') {
			index[k].ptr = buffer + i + 1;
			index[k].length = strlen(index[k].ptr);
			i += index[k].length;
			k++;
		}
		else {
			i++;
		}
	}

	return index;
}

void printindex(lline* index, int nlines) {
	assert(index != NULL);
	assert(nlines != 0);

	for (int i = 0; i < nlines; i++) {
		printf("%s\n", index[i].ptr);
	}
}

void fprintindex(FILE* fp, lline* index, int nlines ) {
	assert(index != NULL);
	assert(fp != NULL);
	assert(nlines != 0);

	for (int i = 0; i < nlines; i++) {
		fprintf(fp, "%s\n", index[i].ptr);
	}
}

void fprintbuffer(FILE* fp, char* buffer, int nsymbols) {
	assert(buffer != NULL);
	assert(fp != NULL);
	assert(nsymbols != 0);

	for (int i = 0; i < nsymbols; i++) {
		if (buffer[i] == '\0') {
			buffer[i] = '\n';
		}
		fputc(buffer[i], fp);
	}
}


int lstrcomp(lline *left, lline *right) {

	for (int i = 0, k = 0; left->ptr[i] != '\0' && right->ptr[k] != '\0'; i++, k++) {
		if ((isalpha(left->ptr[i])) && (isalpha(right->ptr[k]))) {
			if ((tolower(left->ptr[i])) > tolower(right->ptr[k])) {
				return 1;
			}
			else if ((tolower(left->ptr[i])) < tolower(right->ptr[k])) {
				return -1;
			}
		}
		else if (isalpha(left->ptr[i])) {
			i--;
			continue;
		}
		else {
			k--;
			continue;
		}
	}
	
	return 0;
}

int rstrcomp(lline* left, lline* right) {

	for (int i = 0, k = 0; left->ptr[left->length - 1 - i] != '\0' && right->ptr[right->length -1 - k] != '\0'; i++, k++) {
		if ((isalpha(left->ptr[left->length - 1 - i])) && (isalpha(right->ptr[right->length - 1 - k]))) {
			if ((tolower(left->ptr[left->length - 1 - i])) > tolower(right->ptr[right->length - 1 - k])) {
				return 1;
			}
			else if ((tolower(left->ptr[left->length - 1 - i])) < tolower(right->ptr[right->length - 1 - k])) {
				return -1;
			}
		}
		else if (isalpha(left->ptr[left->length - 1 - i])) {
			i--;
			continue;
		}
		else {
			k--;
			continue;
		}
	}

	return 0;
}

void swap(lline* a, lline* b) {
	lline t = *b;
	*b = *a;
	*a = t;
}

void bubblesort(lline* base, size_t num, int (*compare) (lline *, lline *)) {
	for (int i = 1; i < num; i++) {
		for (int k = num - 1; k > 0; k--) {
			if ((compare(&base[k - 1], &base[k])) > 0) {
				swap(&base[k - 1], &base[k]);
			}
		}
	}
}