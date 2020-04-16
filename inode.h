#ifndef INODE_H
#define INODE_H

#include "pages.h"
//const int INODE_COUNT = 128;
///const int INODE_COUNT = 128;
typedef struct inode {
    int mode; // permission & type; zero for unused
    int size; // bytes
    int count;
    int pinum;
    int ptrs[2]; // direct pointers
    int iptr; // single indirect 
              // inode #x always uses data page #x
} inode;

void print_inode(inode* node);
inode* get_inode(int inum);
int alloc_inode();
void free_inode();
int inode_get_pnum(inode* node, int fpn);
int grow_inode(inode* node, int size);
int shrink_inode(inode* node, int size);
int inode_get_pnum(inode* node, int fpn);
#endif
