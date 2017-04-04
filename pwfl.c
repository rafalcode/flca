/* basemw.c: academic really, takes strings from command line and puts them into a struct of an array for structs.
   Copyright (C) 2017  Ramon Fallon

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
   */
/* What does this serve?
 * 1. show how the CONDREALLOC macro is good for structs too
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GBUF 2

/* quick macro for conditionally enlarging a char pointer, space always available for final null char */
#define CONDREALLOC(x, b, c, a, t); \
	if((x)==((b)-1)) { \
		(b) += (c); \
		(a)=realloc((a), (b)*sizeof(t)); \
	}

typedef unsigned char boole;

typedef struct  /* opts_true: a structure for the options reflecting their true types */
{
    boole s; /* just the pairwise ops array sanity check */
    boole q; /* print out nothing. just give any errors, if any */
} opts_t;

typedef struct /* n_sz, name and size type*/
{
	char *n; /* name */
	unsigned sz; /* name size, also buffer */
} n_sz_t; /* sequence index and number of symbols */

typedef struct /* n_sza, name and size type*/
{
	n_sz_t *ns; /* NameS ... an array of n_sz_t's */
	unsigned sz; /* size of array */
} n_sza_t; /* sequence index and number of symbols */


int catchopts(opts_t *opts, int oargc, char **oargv)
{
	int c;
	opterr = 0;

	while ((c = getopt (oargc, oargv, "zs")) != -1)
		switch (c) {
			case 'q':
				opts->q = 1;
				break;
			case 's':
				opts->s = 1;
				break;
			case '?':
				/* general error state? */
				if(isprint (optopt))
					fprintf (stderr, "`-%c' is not a valid option for this program.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				return 1;
			default:
				abort();
		}
	return 0;
}

void snckpwoa(n_sza_t *nsa) /* sanity check pairwise ops array */
{
    int i, j;
	int npwc=nsa->sz*(nsa->sz-1)/2; // well known, from the maths.
	printf("Info: %u filenames to be compared, %i pairwise comparisons\n", nsa->sz, npwc);
	int nc=nsa->sz-1; /* max number of columns of pairwise array */
	int nr=nc; /* number of rows, this is not needed but it helps clarity */

	int mj, mi=0;
    printf("Here is how an array of the pairwise operations would look.\n"); 
	for(i=0;i<nr;++i) {
		mj=nc-i; // gradually decreasing extent of the column run
        printf("Name \"%s\" against:\n", nsa->ns[i].n);
		for(j=0;j<mj;++j)
			printf("@ pos %i: %s | ", mi+j, nsa->ns[i+j+1].n);
        printf("\n");
		mi+=nsa->sz-i-1; // cumulative start position for the column run.
	}
    return;
}

int rdname(FILE *fin, n_sz_t *ns)
{
	int i, c, ret;
	char oldc='\n'; /* this mechanism used to take care of file which end with a \n and then a EOF */
	ns->sz=GBUF; /* we tell a lie in the beginning */
	ns->n=calloc(ns->sz, sizeof(char));

	i=0;
	for(;;) {
		c = fgetc(fin);
		if( c== '\n') {
			ret=0;
			break;
		} else if (c==EOF) {
			/* we try to catch out situation of line being only an EOF! */
			if(oldc=='\n')
				ret=2;
			else
				ret=1;
			break;
		}
		CONDREALLOC(i, ns->sz, GBUF, ns->n, char);
		ns->n[i]=(char)c;
		i++;
		oldc=(char)c;
	}
	if(ret==2) {
		free(ns->n);
		return ret;
	}

	ns->n[i++]='\0';
	printf("%i|%u\n", i, ns->sz); 
	ns->sz=i; /* will be size including zero */
	ns->n=realloc(ns->n, ns->sz*sizeof(char));
	return ret;
}

n_sza_t *rdmnams(char *fname)
{
	FILE *fin=fopen(fname, "r");
	int i=0;
	n_sza_t *nsa=malloc(sizeof(n_sza_t));;
	nsa->sz=GBUF; /* we tell a lie in the beginning */
	nsa->ns=malloc(nsa->sz*sizeof(n_sz_t));
	int ret;

	while( !(ret=(rdname(fin, nsa->ns+i))) ) {
		CONDREALLOC(i, nsa->sz, GBUF, nsa->ns, n_sz_t);
		i++;
	}

	/* normalize the array of structs inside nsa */
	/* note that it will be one bigger */
	nsa->ns=realloc(nsa->ns, i*sizeof(n_sz_t));
	/* note how the nsa->ns[i].n do not need freeing, rdnam does its own normalizing */
	nsa->sz=i;

	fclose(fin);
	return nsa;
}

int main(int argc, char *argv[])
{
	/* argument accounting: remember argc, the number of arguments, _includes_ the executable */
	if(argc!=2) {
		printf("Usage: This program should be fed a file listing.\n");
		exit(EXIT_FAILURE);
	}

	int j;
	n_sza_t *nsa=rdmnams(argv[1]);
    snckpwoa(nsa);

	/* Clean up */
	for(j=0; j<nsa->sz; ++j) {
		free(nsa->ns[j].n);
	}
	free(nsa->ns);
	free(nsa);
	return 0;
}
