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
 * 2. create the struct in the parent function, pass as ptr argument to a void child function: this is the best
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GBUF 4

/* quick macro for conditionally enlarging a char pointer, space always available for final null char */
#define CONDREALLOC(x, b, c, a, t); \
    if((x)==((b)-1)) { \
        (b) += (c); \
        (a)=realloc((a), (b)*sizeof(t)); \
    }

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

void rdname(char *name, n_sz_t *ns)
{
    int i, c;
	unsigned char LOWERCASE;
    ns->sz=GBUF; /* we tell a lie in the beginning */
    ns->n=calloc(ns->sz, sizeof(char));

    i=0;
    while( (c=name[i]) != '\0') {
		CONDREALLOC(i, ns->sz, GBUF, ns->n, char);
		ns->n[i]=(char)c;
		if((c>=97) & (c<=122)) {
			LOWERCASE=1;
		} else {
			LOWERCASE=0;
		}
		i++;
	}
	ns->n[i++]='\0';
	ns->sz=i; /* will be size including zero */
	ns->n=realloc(ns->n, ns->sz*sizeof(char));
	return;
}

n_sza_t *rdmnams(char **namarr, int quan) /* Read multi nams */
{
	int i;
	n_sza_t *nsa=malloc(sizeof(n_sza_t));;
	nsa->sz=GBUF; /* we tell a lie in the beginning */
	nsa->ns=malloc(nsa->sz*sizeof(n_sz_t));

	for(i=0;i<quan;++i) {
		CONDREALLOC(i, nsa->sz, GBUF, nsa->ns, n_sz_t);
		rdname(namarr[i], nsa->ns+i);
	}

	/* normalize the array of structs inside nsa */
	nsa->ns=realloc(nsa->ns, i*sizeof(n_sz_t));
	/* note how the nsa->ns[i].n do not need freeing, rdnam does its own normalizing */
	nsa->sz=i;

	return nsa;
}

int main(int argc, char *argv[])
{
	/* argument accounting: remember argc, the number of arguments, _includes_ the executable */
	if(argc==1) {
		printf("Usage: This program should be fed any number of string arguments\n");
		exit(EXIT_FAILURE);
	}

	int i, j;
	n_sza_t *nsa=rdmnams(argv+1, argc-1);

	for(j=0;j<nsa->sz;++j) {
		printf("Your word #%i needs %u char storage and is read as: \"", j, nsa->ns[j].sz); 
		for(i=0;i<nsa->ns[j].sz;++i) 
			putchar(nsa->ns[j].n[i]);
		printf("\"\n"); 
	}

	for(j=0; j<nsa->sz; ++j) {
		free(nsa->ns[j].n);
	}
	free(nsa->ns);
	free(nsa);
	return 0;
}
