#include <stdlib.h>

int mystrlen(char *str){
    int len = 0;
    for(int i=0;str[i]!='\0';i++){
        len++;
    }
    return len;
}

char *mystradd(char *origin, char *addition){
    char* concat;
    int lenO = mystrlen(origin);
    int lenA = mystrlen(addition);
    int len = lenO + lenA;
    concat = malloc(sizeof(char)*(len+1));
    lenA = 0;
    for(int i=0;i<len;i++){
        if (lenO>0){
            concat[i]=origin[i];
            lenO--;
        } else {
            concat[i]=addition[lenA];
            lenA++;
        }
    }
    concat[len] = '\0';
    return concat;
}

int mystrfind(char *origin, char *substr){
    int len = mystrlen(origin);
    int sub_len = mystrlen(substr);
    int found = 0;
    if (len<sub_len || sub_len==0){
        return 0;
    }
    for(int i=0;i<(len+1-sub_len);i++){
        if (origin[i]==substr[0]){
            if(sub_len==1){
                return 1;
            }
            found = 1;
            for(int j=1;j<sub_len;j++){
                if(origin[i+j]!=substr[j]){
                    found = 0;
                    break;
                }
            }
            if (found==1){
                return 1;
            }
        }
    }
    return 0;
}

