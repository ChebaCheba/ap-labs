#include <stdio.h>
#include "strlib.c"

int main(int argc, char *argv[]) {
    if(argc != 4){
        perror("Error: format must be ./main <original string> <string to add> <sub-string to find>");
        return -1;
    }
    char* originalStr = argv[1];
    char* addStr = argv[2];
    char* sub = argv[3];

    char* newStr = mystradd(originalStr, addStr);

    printf("Initial Lenght      : %d\n", mystrlen(originalStr));
    printf("New String          : %s\n", newStr);
    if (mystrfind(newStr, sub)==1){
        printf("SubString was found : yes\n");
    } else {
        printf("SubString was found : no\n");
    }

    free(newStr);
    
    return 0;
}

