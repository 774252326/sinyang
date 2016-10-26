#include "stdafx.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

void nrerror(char error_text[])
	/* Numerical Recipes standard error handler */
{
	fprintf(stderr,"Numerical Recipes run-time error...\n");
	fprintf(stderr,"%s\n",error_text);
	fprintf(stderr,"...now exiting to system...\n");
	exit(1);
}