#include <stdio.h>
#include <stdlib.h>

void reverse(int length, int arr[]) {

    int i, tmp;

    for (i = 0;  i < length/2; i++) {
        tmp = arr[i];
        arr[i] = arr[length - i - 1];
        arr[length - i - 1] = tmp;
    }
}

int main(){
    // Place your magic here
    int length = 0;
    int size = 100;
    int *arr;
    char c;

    arr = malloc(sizeof(int)*size);

    while((c = getchar()) != EOF){
        if (length>=size){
            int *temp;
            size = size*2;
            temp = malloc(sizeof(int)*size);
            for (int i=0;i<length;i++){
                temp[i] = arr[i];
            }
            free(arr);
            arr = temp;
            temp = NULL;
        }
        if (c == '\n'){
            reverse(length, arr);
            for(int i=0;i<length;i++){
                printf("%c",arr[i]);
            }
            printf("\n");
            length = 0;
	    continue;
        } else {
            arr[length]=c;
        }
        length++;
    }
    free(arr);
    return 0;
}

