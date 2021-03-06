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

void rdname2(char *name, n_sz_t *ns)
{
    int i, c;
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
    return;
}

int main(int argc, char *argv[])
{
    /* argument accounting: remember argc, the number of arguments, _includes_ the executable */
    if(argc!=2) {
        printf("Usage this very basic program only accepts one argument, a name (probably a file name)");
        exit(EXIT_FAILURE);
    }

    int i;
    n_sz_t *ns=malloc(sizeof(n_sz_t));
    rdname2(argv[1], ns);
    printf("Your word needs %u char storage and ws read as: \"", ns->sz); 

    for(i=0;i<ns->sz;++i) 
        putchar(ns->n[i]);

    printf("\"\n"); 

    free(ns->n);
    free(ns);
    return 0;
}
