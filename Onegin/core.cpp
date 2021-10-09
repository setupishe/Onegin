
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

typedef struct l_line {
	char* ptr;
	int length;
} lline;

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <sys\stat.h>
#include <stdlib.h>
#include <string.h>

FILE* fastopen(const char *name) {

	FILE* fp;
	int dfp;

	dfp = open((const char *)name, O_RDONLY); 
	fp = fdopen(dfp, "r");

	return fp;
}

//
char *makebuffer(FILE* fp, int *n_lines, int *n_symbols) {
	char c;
	struct stat statfp;
	int nlines = 0, nsymbols = 0;
	stat("hamlet.txt", &statfp); //получаем размер файла
	nsymbols = statfp.st_size;
	//printf("size = %d\n", statfp.st_size);


	char* buffer = (char*)calloc(nsymbols, sizeof(char));

	c = getc(fp); //заполняем буфер
	if (c != '\r') {
		if (c == '\n') {
			buffer[0] = '\0';
			nlines++;
		}
		else {
			buffer[0] = c;
		}
	}

	//printf("%c", buffer[0]);
	for (int i = 1; c != EOF; i++) {
		c = getc(fp);
		if (c != '\r') {
			if (c == '\n') {
				buffer[i] = '\0';
				nlines++;
			}
			else {
				buffer[i] = c;
			}
		}
		//printf("%c", buffer[i]);
	}
	buffer[nsymbols] = '\0';

	if (nlines != 0) {
		nlines++;
	}

	*n_lines = nlines;
	*n_symbols = nsymbols;
	//printf("nlines = %d", nlines);



	return buffer;
}


lline* makeindex(char* buffer, int nlines, int nsymbols) {

	lline *index = (lline*)calloc(nlines, sizeof(lline)); //запрашиваем память для массива


	int k = 0;
	if (buffer[0] != '\0' && buffer[1] != '\0') { //проверяем начало буфера -- не строка ли
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

	for (int i = 0; i < nlines; i++) {
		printf("index number %d, string \"%s\", length %d \n", i + 1, index[i].ptr, index[i].length);
	}
}