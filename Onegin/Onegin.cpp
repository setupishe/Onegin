
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <sys\stat.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"


int main(){

	FILE* fp;
	int nlines = 0, nsymbols = 0;
	fp = fastopen("hamlet.txt");
	char *buffer = makebuffer(fp, &nlines, &nsymbols);
	lline *index = makeindex(buffer, nlines, nsymbols);

	printindex(index, nlines);
	return 0;
}


