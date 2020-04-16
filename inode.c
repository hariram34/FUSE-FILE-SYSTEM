
#include <stdint.h>

#include "pages.h"
#include "inode.h"
#include "util.h"
#include "bitmap.h"

inode*
get_inode(int inum)
{
    uint8_t* base = (uint8_t*) pages_get_page(0);
    inode* nodes = (inode*)(base);
    return &(nodes[inum]);
}

int
alloc_inode()
{
    printf("entering alloc inode\n");
    int inum;
    for (int ii = 1; ii < 128; ++ii) {
        
         if (!bitmap_get(find_inode_bitmap(), ii)) {
             
            bitmap_put(find_inode_bitmap(), ii, 1);
            inum = ii;
            break;
        }
    }
    printf("inum from ii is %d\n",inum);
    inode* node = get_inode(inum);
    node->count = 0;
    node->size = 0;
    node->mode = 0;
    node->ptrs[0] = alloc_page();
    
       // inode* node = get_inode(ii);
       // if (node->mode == 0) {
       //     memset(node, 0, sizeof(inode));
       //     node->mode = 010644;
       //     printf("+ alloc_inode() -> %d\n", ii);
       //     return ii;
       // }
    

    return inum;
}

void
free_inode(int inum)
{
    printf("+ free_inode(%d)\n", inum);
    inode* node = get_inode(inum);
    void* bmp = find_inode_bitmap(); 
    shrink_inode(node, 0);
    free_page(node->ptrs[0]);
    bitmap_put(bmp, inum, 0);
}

void
print_inode(inode* node)
{
    if (node) {
        printf("node{mode: %04o, size: %d, pinum: %d, count: %d}\n",
               node->mode, node->size,node->pinum,node->count);
    }
    else {
        printf("node{null}\n");
    }
}


// grows the inode, if size gets too big, it allocates a new page if possible
int grow_inode(inode* node, int size) {
    for (int i = (node->size / 4096) + 1; i <= size / 4096; i ++) {
        if (i < 2) { //we can use direct ptrs
            node->ptrs[i] = alloc_page(); //alloc a page
        } else { //need to use indirect
            if (node->iptr == 0) { //get a page if we don't have one
                node->iptr = alloc_page();
            }
            int* iptrs = pages_get_page(node->iptr); //retrieve memory loc.
            iptrs[i - 2] = alloc_page(); //add another page
        }
    }
    node->size = size;
    return 0;
}

// shrinks an inode size and deallocates pages if we've freed them up
int shrink_inode(inode* node, int size) {
    for (int i = (node->size / 4096); i > size / 4096; i --) {
        if (i < 2) { //we're in direct ptrs
            free_page(node->ptrs[i]); //free the page
            node->ptrs[i] = 0;
        } else { //need to use indirect
            int* iptrs = pages_get_page(node->iptr); //retrieve memory loc.
            free_page(iptrs[i - 2]); //free the single page
            iptrs[i-2] = 0;

            if (i == 2) { //if that was the last thing on the page
                free_page(node->iptr); //we don't need it anymore
                node->iptr = 0;
            }
        }
    } 
    node->size = size;
    return 0;  
}

// gets the page number for the inode
int inode_get_pnum(inode* node, int fpn) {
    int blocknum = fpn / 4096;
    if (blocknum < 2) {
        return node->ptrs[blocknum];
    } else {
        int* iptrs = pages_get_page(node->iptr);
        return iptrs[blocknum-2];
    }
}

//void decrease_refs(int inum)
//{
//    inode* node = get_inode(inum);
//    node->refs = node->refs - 1;
//    if (node->refs < 1) {
//        free_inode(inum);
//    }
//}
//