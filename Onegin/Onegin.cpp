
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


	FILE* fp = NULL;
	int nlines = 0, nsymbols = 0;

	if (!(fp = fastopen("hamlet.txt"))){
		return 1;
	}

	if ((nsymbols = getsize("hamlet.txt")) == 0) {
		printf("Cannot check file size");
			return 1;
	}

	char* buffer = NULL;
	if (!(buffer = makebuffer(fp, &nlines, &nsymbols))) {
		return 1;
	}

	lline* index = NULL;
	if (!(index = makeindex(buffer, nlines, nsymbols))) {
		return 1;
	}

	FILE* fop = NULL;
	if (!(fop = fopen("hamlet sorted.txt", "w"))) {
		return 1;
	}

	//sorting alphabetically
	qsort((void*)index, nlines, sizeof(lline), (int(*) (const void *, const void *))lstrcomp);
	fprintf(fop, "left sorted:\n");
	fprintindex(fop, index, nlines);
	fputc('\n', fop);

	//sorting reversed-alphabetically
	bubblesort(index, nlines, rstrcomp);
	fprintf(fop, "right sorted:\n");	
	fprintindex(fop, index, nlines);
	fputc('\n', fop);

	//printing original
	fprintf(fop, "original:\n");
	fprintbuffer(fop, buffer, nsymbols);

	//clear memory
	fclose(fp);
	fclose(fop);
	free(buffer);
	free(index);

	return 0;
}


