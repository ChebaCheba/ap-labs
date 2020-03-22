#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void mergesort(void *lineptr, int l, int r, size_t size, int (*comp)(void *, void *));
void merge(void* lineptr, int l, int m, int r, size_t size, int(*comp)(void *, void *));
int numcmp(void* x, void* y);
int chrcmp(void* x, void* y);


void mergesort(void *lineptr, int l, int r, size_t size, int (*comp)(void *, void *)){
    if (l >= r) { /* do nothing if array contains */
        return;   /* fewer than two elements */
    }
    int m = l+(r-l)/2;
    mergesort(lineptr, l, m, size, comp);
    mergesort(lineptr, m+1, r, size, comp);
    merge(lineptr, l, m, r, size, comp);
}

void merge(void* lineptr, int l, int m, int r, size_t size, int(*comp)(void *, void *)){
    int i, j, k;
    int s1 = m - l + 1;
    int s2 = r -m;
    void* L = malloc(size*s1);
    void* R = malloc(size*s2);

    for(i=0; i < s1; i++){
        memcpy(L+size*i, lineptr+size*(l+i), size);
    }
    for(j=0; j < s2; j++){
        memcpy(R+size*j, lineptr+size*(m+1+j), size);
    }

    i = 0;
    j = 0;
    k = l;

    while(i<s1 && j < s2){
        if(comp(L+size*i, R+size*j)){
            memcpy(lineptr + size*k, L+size*i, size);
            i++;
        } else {
            memcpy(lineptr + size*k, R+size*j, size);
            j++;
        }
        k++;
    }
    while(i < s1) {
        memcpy(lineptr+size*k, L+size*i, size);
        i++;
        k++;
    }
    while(j < s2){
        memcpy(lineptr+size*k, R+size*j, size);
        j++;
        k++;
    }
    free(L);
    free(R);
}

/* numcmp: compare x and y numerically */
int numcmp(void* x, void* y){
    int *a = (int*)x;
    int *b = (int*)y;
    return *a <= *b ? 1 : 0;
}

int chrcmp(void* x, void* y){
    char *a = (char*)x;
    char *b = (char*)y;
    return *a <= *b ? 1 : 0;
}

int* str_to_int(char* str, int length){
    int *iarr =(int*)malloc(sizeof(int)*length);
    for(int i=0;i<length;i++){
        iarr[i]=str[i]-48;
    }
    return iarr;
}

/* sort input lines */
int main(int argc, char **argv) {
    int numeric = 0;
    if (argc < 2) {
        perror("No arguments detected");
        exit(EXIT_FAILURE);
    }
    if (strcmp(argv[1], "-n")==0){
        if (argc < 3){
            perror("Error: format must be ./generic_merge_sort \"-n\" <numbers>");
            exit(EXIT_FAILURE);
        }
        numeric = 1;
    }
    if (numeric == 1) {
        int length = strlen(argv[2]);
        if (length < 1) {
            perror("Error: format must be ./generic_merge_sort \"-n\" <numbers>");
            exit(EXIT_FAILURE);
        }
        int* a = str_to_int(argv[2], length);
        mergesort(a, 0, length-1, sizeof(int), numcmp);
        for(int i = 0; i<length;i++){
            printf("%d ", a[i]);
        }
        printf("\n");
    } else {
        int length = strlen(argv[1]);
        if (length < 1) {
            perror("Error: format must be ./generic_merge_sort <string>");
            exit(EXIT_FAILURE);
        }
        char* a = argv[1];
        mergesort(a, 0, length-1, sizeof(char), chrcmp);
        for(int i = 0; i<length;i++){
            printf("%c ", a[i]);
        }
        printf("\n");
    }

    return 0;
}
