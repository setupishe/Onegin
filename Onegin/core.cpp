
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE



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

	FILE* fp;
	int dfp;

	if (dfp = open((const char*)name, O_RDONLY) == -1) {
		printf("Cannot open file: descriptor creation failed\n");
		exit(1);
	}
	if (!(fp = fdopen(dfp, "r"))) {
		printf("Cannot open file: file pointer creation failed\n");
		exit(1);
	}

	return fp;
}

//
char *makebuffer(FILE* fp, int *n_lines, int *n_symbols) {
	assert(fp != NULL);
	assert(*n_lines != 0);
	assert(*n_symbols != 0);

	char c;
	struct stat statfp;
	int nlines = 0, nsymbols = 0;
	if (stat("hamlet.txt", &statfp) == -1) { //check file size
		printf("Cannot check file size");
		exit(1);
	}
	nsymbols = statfp.st_size;
	//printf("size = %d\n", statfp.st_size);


	char* buffer = (char*)calloc(nsymbols, sizeof(char));
	if (!buffer) {
		printf("Cannot create buffer");
		exit(1);
	}

	c = getc(fp); //fill the buffer
	if (c != '\r') {
		if (c == '\n') {
			buffer[0] = '\0';
			nlines++;
		}
		else {
			buffer[0] = c;
		}
	}

	for (int i = 1; c != EOF; c = getc(fp)) {
		i++;
		if (c != '\r') {
			if (c == '\n') {
				buffer[i] = '\0';
				nlines++;
			}
			else {
				buffer[i] = c;
			}
		}

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
	assert(nlines != 0);
	assert(nsymbols != 0);

	lline *index = (lline*)calloc(nlines, sizeof(lline)); 

	if (!index) {
		printf("Cannot create index");
		exit(1);
	}

	int k = 0;
	if (buffer[0] != '\0' && buffer[1] != '\0') { //check if the first element start the line
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
		/*if (buffer[i] == -1)
		{
			break;
		}*/
		fputc(buffer[i], fp);
	}
}


int lstrcomp(lline *left, lline *right) {

	if (!((left->ptr[0] <= RIGHTCAP && left->ptr[0] >= LEFTCAP) || (left->ptr[0] <= RIGHTLOW && left->ptr[0] >= LEFTLOW))) {
		return 2;
	}
	else if (!((right->ptr[0] <= RIGHTCAP && right->ptr[0] >= LEFTCAP) || (right->ptr[0] <= RIGHTLOW && right->ptr[0] >= LEFTLOW))) {
		return -2;
	}
	else {
		int n = (left->length > right->length) ? left->length : right->length;
		for (int i = 0; i < n; i++) {
			if ((tolower(left->ptr[i])) > tolower(right->ptr[i])) {
				return 1;
			}
			else if ((tolower(left->ptr[i])) < tolower(right->ptr[i])) {
				return -1;
			}
		}

		return 0;
		
	}
}

int rstrcomp(lline* left, lline* right) {

	if (!((left->ptr[left->length - 1] <= RIGHTCAP && left->ptr[left->length - 1] >= LEFTCAP) || (left->ptr[left->length - 1] <= RIGHTLOW && left->ptr[left->length - 1] >= LEFTLOW))) {
		return 2;
	}
	else if (!((right->ptr[right->length - 1] <= RIGHTCAP && right->ptr[right->length - 1] >= LEFTCAP) || (right->ptr[right->length - 1] <= RIGHTLOW && right->ptr[right->length - 1] >= LEFTLOW))) {
		return -2;
	}
	else {
		int n = (left->length > right->length) ? left->length : right->length;
		for (int i = 0; i < n; i++) {
			if ((tolower(left->ptr[left->length - 1 - i])) > tolower(right->ptr[right->length - 1 - i])) {
				return 1;
			}
			else if ((tolower(left->ptr[left->length - 1 - i])) < tolower(right->ptr[right->length - 1 - i])) {
				return -1;
			}
		}
		return 0;
	}
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