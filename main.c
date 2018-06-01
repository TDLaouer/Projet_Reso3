/* 

  Author: Quentin Giorgi 
	
  This file is part of HTTP parser 

  This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

// this will declare internal type used by the parser
#include "httpparser.h" 

#include "api.h" 

int main(int argc,char *argv[])
{
	int res,fi,i=5000;
	char *p=NULL,*addr;
        struct stat st;

	if (argc < 2 ) { printf("Usage: httpparser <file> [search]\nAttention <search> is case sensitive\n\tDisplay tree with CRLF removed!!"); exit(1); }
	// mmap from file the request to parse
	if ((fi=open(argv[1],O_RDWR)) == -1) {
                perror("open");
                return -1;
        }
        if (fstat(fi, &st) == -1)           /* To obtain file size */
                return -1;
        if ((addr=mmap(NULL,st.st_size,PROT_WRITE,MAP_PRIVATE, fi, 0)) == MAP_FAILED)
                return -1;
	// search a specific token 
	if (argc == 3 ) { 
		p=argv[2]; 	
		printf("searching for %s\n",p); 
		while (*p) { 
			if (*p=='-') { *p='_'; }
			p++; 
		}
		p=argv[2]; 	
	}
	// call the parser with respect to api.h function declaration 
	if (res=parseur(addr,st.st_size)) {
		_Token *r,*tok; 
		
		// get the root of the tree this is no longer opaque since we know the internal type with httpparser.h 
		//void *root; 
		Lnode *root;
		root=getRootTree(); 
		if (p != NULL) {
			r=searchTree(root,p); 
			tok=r; 
			while (tok) {
				int l; 
				char *s; 
				// node is no longer opaque 
				Lnode *node;
				node=(Lnode *)tok->node; 
				printf("%s --> FOUND [%.*s]\n",node->name,node->len,node->value);
				tok=tok->next; 
			}
			purgeElement(&r); 
		} 		
		purgeTree(root); 
	} else {
		printf("NOK\n");
	}
	munmap(addr,st.st_size); 	
	close(fi);
	return(res); 
}
