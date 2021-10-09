#pragma once

typedef struct l_line {
	char* ptr;
	int length;
} lline;

//Opens small files without buffering
FILE* fastopen(const char *name);

//Makes char buffer from file, inserts numbers of lines and symbols
char* makebuffer(FILE* fp, int* n_lines, int* n_symbols);

//Makes index of lines and lengths
lline* makeindex(char* buffer, int nlines, int nsymbols);

//Prints text stored in index
void printindex(lline* index, int nlines);