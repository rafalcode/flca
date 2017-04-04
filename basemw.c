/* rdfasta.c: a multifasta summarizer iof mine repurposed to a read & store fasta idlines. merely a protype for pit2f.c's 1st arg
   Copyright (C) 2014  Ramon Fallon

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

n_sz_t *rdname(char *name)
{
    int i, c;
    n_sz_t *ns=malloc(sizeof(n_sz_t));;
    ns->sz=GBUF; /* we tell a lie in the beginning */
    ns->n=calloc(ns->sz, sizeof(char));

    i=0;
    while( (c=name[i]) != '\0') {
        CONDREALLOC(i, ns->sz, GBUF, ns->n, char);
        ns->n[i]=(char)c;
        i++;
    }
    ns->n[i++]='\0';
    ns->sz=i; /* will be size including zero */
    ns->n=realloc(ns->n, ns->sz*sizeof(char));
    return ns;
}

n_sza_t *rdmnams(char *namarr[]) /* Read multi nams */
{
    int i, j;
    // n_sz_t *c=NULL;
    n_sza_t *nsa=malloc(sizeof(n_sza_t));;
    nsa->sz=GBUF; /* we tell a lie in the beginning */
    nsa->ns=malloc(nsa->sz*sizeof(n_sz_t));

    i=0;
    // while( (c=rdname(namarr[i])) != NULL) {
    //     CONDREALLOC(i, nsa->sz, GBUF, nsa->ns, n_sz_t);
    //     nsa->ns[i].sz=c->sz;
    //     memcpy(nsa->ns[i].n, c->n, nsa->ns[i].sz*sizeof(char));
    //     i++;
	// 	free(c);
    // }
    while( (&(nsa->ns[i])=rdname(namarr[i])) != NULL) {
        CONDREALLOC(i, nsa->sz, GBUF, nsa->ns, n_sz_t);
        i++;
    }
    for(j=i; j<nsa->sz; ++j)
        free(nsa->ns[j].n);
    free(nsa->ns);
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
    n_sza_t *nsa=rdmnams(argv+1);

    for(j=0;j<nsa->sz;++j) {
		printf("Your word #%i needs %u char storage and ws read as: \"", j, nsa->ns[j].sz); 
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
