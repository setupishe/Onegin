#pragma once

typedef struct l_line {
	char* ptr;
	int length;
} lline;

const char LEFTCAP = 'A';
const char RIGHTCAP = 'Z';
const char LEFTLOW = 'a';
const char RIGHTLOW = 'z';

//Opens small files without buffering
FILE* fastopen(const char *name);

//Makes char buffer from file, inserts numbers of lines and symbols
char* makebuffer(FILE* fp, int* n_lines, int* n_symbols);

//Makes index of lines and lengths
lline* makeindex(char* buffer, int nlines, int nsymbols);

//Prints text stored in index to stdout
void printindex(lline* index, int nlines);

//Prints text stored in index to file
void fprintindex(FILE* fp, lline* index, int nlines);

//Prints text stored in buffer to file
void fprintbuffer(FILE* fp, char* buffer, int nsymbols);

//Compares two llines alphabetically, lines started with non-letter go to the end
int lstrcomp(lline* left, lline* right);

//Compares two llines reversed-alphabetically, lines started with non-letter go to the end
int rstrcomp(lline* left, lline* right);

//sorts llines* array using bubblesort
void bubblesort(lline* base, size_t num, int (*compare) (lline*, lline*));

//Swaps two llines
void swap(lline* a, lline* b);
