#include <stdio.h>
#include <stdlib.h>

void display(int* arr, int length) {
    for(int i=0;i<length;i++) {
        printf("%d ",arr[i]);
    }
    printf("\n");
}

int find_max(int* arr, int length) {
    int max = arr[0];
    for(int i=0;i<length;i++) {
        if(arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

int find_min(int* arr, int length) {
    int min = arr[0];
    for(int i=0;i<length;i++) {
        if(arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
}

int get(int* arr, int length, int index) {
    if(index >= 0 && index < length) {
        return arr[index];
    }
    else {
        return -1;
    }
}

void set(int* arr, int length, int index, int element) {
    if(index >= 0 && index < length) {
        arr[index] = element;
    }
}

int search(int* arr, int length, int element) {
    for(int i=0;i<length;i++) {
        if(arr[i] == element) {
            return i;
        }
    }
    return -1;
}

int delete(int* arr, int length, int pos) {
    for(int i=pos;i<length-1;i++) {
        arr[i] = arr[i+1];
    }
    return length-1;
}

void create(int* arr, int length) {
    for(int i=0;i<length;i++) {
        printf("enter arr[%d]: ",i);
        scanf("%d",&arr[i]);
    }
}

int append(int* arr, int length, int element) {
    arr[length] = element;
    return length+1;
}

int insert(int* arr, int length, int pos, int element) {
    int i;
    for(i=length;i>pos;i--) {
        arr[i] = arr[i-1];
    }
    arr[i] = element; 
    return length+1;
}

int* reverse(int* arr, int length) {
    int* temp;
    temp = (int*)malloc(length*sizeof(int));
    for(int i=0;i<length;i++) {
        temp[i] = arr[length-i-1];
    }
    return temp;
}

int main() {
    int size = 0;
    int length = 0;
    int* arr;

    printf("enter size: ");
    scanf("%d",&size);

    printf("enter length: ");
    scanf("%d",&length);

    arr = (int*)malloc(size*sizeof(int));

    create(arr, length);
    display(arr, length);

    length = insert(arr, length, 3, 77);
    display(arr, length);

    length = append(arr, length, 15);
    display(arr, length);

    length = delete(arr, length, 4);
    display(arr, length);

    printf("%d",find_max(arr,length));
    printf("\n%d\n",find_min(arr,length));

    set(arr, length, 3, 15);
    display(arr, length);
    
    printf("%d",get(arr, length, 3));
    printf("\n%d\n",search(arr, length, 2));

    int* reversed_array = reverse(arr, length);
    display(reversed_array, length);

    return 0;
}