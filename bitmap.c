
#include <stdio.h>
#include "bitmap.h"


int bitmap_get(void* bm, int ii) {

    return ((char*)bm)[ii / 8] >> (ii % 8) & 1;
}

// puts a value at the specified bit in the bitmap
void bitmap_put(void* bm, int ii, int vv) {
    //bitmap_print(bm, ii);
    int bit = (vv) ? 1 : 0;
    char* bitmap = (char*)bm;
    int actual = bitmap_get(bm, ii);
    //printf("\nbit val : %d \nstate of the %d th bit : %d\n",bit,ii,actual);
    if ( actual == bit) {
        //printf("returning from bitmamp put \n");
        return;
    }
    else {
        bitmap[(ii / 8)] ^= 1 << ii % 8; 
    }
    //bitmap_print(bm, ii);
}

// debug statement
void bitmap_print(void* bm, int size) {
    for (int ii = 0; ii < size * 4; ++ii) {
        printf("%d", bitmap_get(bm, ii));
        
    }
    //printf("\n");
}
