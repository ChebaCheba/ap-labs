#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ftw.h>
#include "logger.h"

#define BUF_LEN (10 * (sizeof(struct inotify_event) + 1))

int fd;

void getType(struct inotify_event *event, char* type){
    if(event->mask & IN_ISDIR){
        strcpy(type, "Directory");
    } else{
        strcpy(type, "File");
    }
}

int refreshWatch(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf){
    int wd;
    wd = inotify_add_watch(fd, fpath, IN_CREATE | IN_DELETE | IN_MODIFY | IN_ISDIR);
    if (wd == -1){
        errorf("Error: inotify_add_watch failed\n");
        return 0;
    }
    return 0;
}

void nftwLoad(char* path){
    int flags = 0;
     if (nftw(path, refreshWatch, 1000, flags) == -1){
        errorf("Error: nftw failed");
    }
}

void displayInotifyEvent(struct inotify_event *event, char* path){
    char* type;
    type = malloc(sizeof(char)*10);
    type[0] = '\0';
    getType(event, type);
    if (event->mask & IN_CREATE){        
        infof("%s %s created\n", type, event->name);
        nftwLoad(path);
    } else if (event->mask & IN_DELETE){        
        warnf("%s %s deleted\n", type, event->name);
        nftwLoad(path);
    }  else if (event->mask & IN_MODIFY){          
        infof("Modification in %s\n", event->name);
        nftwLoad(path);
    }
    free(type);
}

int main(int argc, char *argv[]){
    int rd;
    char*p;
    struct inotify_event *event;
    if((fd = inotify_init()) == -1){
        panicf("Error: cannot initialize inotify\n");
        return 0;
    }
    char *buffer = (char *)malloc(BUF_LEN);
    char *path = (char*)malloc(sizeof(char)*strlen(argv[1])*100);
    strcpy(path, argv[1]);
    nftwLoad(path);
    infof("Monitoring path %s\n", argv[1]);
    while(1){
        rd = read(fd, buffer, BUF_LEN);
        if (rd == 0){
            errorf("read() from inotify fd returned 0!\n");
        }
        for(p = buffer; p<buffer+rd;){
            event = (struct inotify_event*)p;
            displayInotifyEvent(event, path);
            p += sizeof(struct inotify_event) + event->len;
        }
    }
    close(fd);
    free(buffer);
    return 0;
}

