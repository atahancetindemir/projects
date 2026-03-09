#include <stdio.h>
#include <stdlib.h>

int main() {
    int* arr;
    int pos, val, i, size = 10;

    // Open 8*10 byte space in memory (sizeof(arr) = 8 byte, size = 10 byte)
        arr = (int*)malloc(sizeof(arr)*size);

    // Assign numbers into the memory we opened
    for(i=0;i<size;i++) {
        *(arr+i) = i+1;
    }

    // Loop until pos value becomes -1
    while(1) {
        
        printf("pos:");
        scanf("%d",&pos);
        
        if(pos == -1) {
            break;
        }

        printf("value:");
        scanf("%d",&val);

        // Increase the memory to add 1 more element
        // Since the initial size of arr is 8 bytes,
        // it is not (8*10)/8 = 10 elements but (8*10+8)/8 = 11 elements
        arr = (int*)realloc(arr,sizeof(arr)*size+sizeof(arr));
    
        // Start from the largest index of the array and move the elements
        // to the next index until we reach the index we want
        for(i=size;i>=pos;i--) {
            *(arr+i+1) = *(arr+i);
        }

        // Entering the val into the desired index
        *(arr+pos) = val;

        // Correcting the size value for the next iteration
        size++;

        for(i=0;i<size;i++) {
            printf("%d\n",*(arr+i));
        }
    }

    return 0;
}
