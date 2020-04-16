#ifndef PAGES_H
#define PAGES_H

#include <stdio.h>

void pages_init(const char* path, int create);
void pages_free();
void* pages_get_page(int pnum);
void*
find_page_bitmap();
void*
pages_get_page(int pnum);
void*
find_inode_bitmap();
int
alloc_page();
void
free_page(int pnum);
#endif
