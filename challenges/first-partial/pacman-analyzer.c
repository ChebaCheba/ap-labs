#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "link.h"
#define TABLE_SIZE 1024
#define REPORT_FILE "packages_report.txt"

unsigned int hash(char* key){
    unsigned long int value = 0;
    unsigned int key_len = strlen(key);

    for(int i=0;i<key_len;i++){
        value = value*33+key[i];
    }
    value = value%TABLE_SIZE;

    return value;
}

char* get_line(char* buffer, int start, int end){
    char* line;
    int size = end - start;
    line = malloc(sizeof(char)*size);
    for (int i=0;i<size;i++){
        line[i] = buffer[start+i];
    }
    return line;
}

int check_status(char* line){
    if (strstr(line, "[ALPM] installed") != NULL){
        return 1;
    } else if (strstr(line, "[ALPM] upgraded") != NULL){
        return 2;
    } else if (strstr(line, "[ALPM] removed") != NULL){
        return 3;
    } else {
        return -1;
    }
}

char* get_package_name(char* line, int offset, int *nlen){
    char* name;
    int name_size = 0;
    while(line[offset+name_size]!=' '){
        name_size++;
    }
    name = malloc(sizeof(char)*name_size);
    for(int i=0;i<name_size;i++){
        name[i] = line[offset+i];
    }
    *nlen = name_size;
    return name;
}

char* get_date(char* line){
    char* date;
    date = malloc(sizeof(char)*16);
    for(int i=0;i<16;i++){
        date[i] = line[i+1];
    }
    return date;
}

void generate_data(struct Hash_Table* ht, char* buffer, off_t fs){
    char *line, *name, *date;
    int start, end, status;
    int *nlen;
    int name_size = 0;
    struct Package* curr_package;
    nlen = &name_size;
    start = 0;
    for(end=0;end<fs;end++){
        if(buffer[end]=='\n'){
            line = get_line(buffer, start, end);
            status = check_status(line);
            if (status>0){
                date = get_date(line);
                switch(status){
                    case 1:
                        name = get_package_name(line,36, nlen);
                        insert_p(ht,name,date);
                        break;
                    case 2:
                        name = get_package_name(line,35, nlen);
                        curr_package = search_p(ht,name);
                        update_p(ht,curr_package,date);
                        break;
                    case 3:
                        name = get_package_name(line,34, nlen);
                        curr_package = search_p(ht,name);
                        remove_p(ht,curr_package,date);
                        break;
                }
            }
            memset(line,0,end-start);
            start = end+1;
        }
    }
}

struct Package* package(char* name, char* date){
    struct Package* newPackage = (struct Package*)malloc(sizeof(struct Package));
    newPackage->packageName = name;
    newPackage->installDate = date;
    newPackage->isUpgraded = 0;
    newPackage->upgradeNumber = 0;
    newPackage->isRemoved = 0;
    return newPackage;
}

struct Hash_Table* hash_table(){
    struct Hash_Table* ht = (struct Hash_Table*)malloc(sizeof(struct Hash_Table)+sizeof(struct Package*)*TABLE_SIZE);
    ht->installed = 0;
    ht->removed = 0;
    ht->upgraded = 0;
    ht->current = 0;
    return ht;   
}

void insert_p(struct Hash_Table* ht, char* name, char* date){
    struct Package* newPackage = package(name, date);
    unsigned int hash_value = hash(newPackage->packageName);
    while(ht->table[hash_value]!=NULL){
        if(strcmp(ht->table[hash_value]->packageName,name)==0){
            ht->table[hash_value]->installDate=date;
            return;
        }
        hash_value++;
    }
    ht->table[hash_value] = newPackage;
    ht->installed++;
    ht->current++;
}

struct Package* search_p(struct Hash_Table* ht,char* name){
    unsigned int hash_value = hash(name);
    struct Package* temp = ht->table[hash_value];
    while(temp!=NULL){
        if(strcmp(temp->packageName,name)==0){
            return temp;
        } else {
            hash_value++;
            temp = ht->table[hash_value];
        }
    }
    return 0;
}

void update_p(struct Hash_Table* ht, struct Package* curr_package, char* date){
    if(curr_package->isUpgraded==0){
        ht->upgraded++;
        curr_package->isUpgraded++;
    }
    curr_package->upgradeNumber++;
    curr_package->lastUpdate=date;
}

void remove_p(struct Hash_Table* ht, struct Package* curr_package, char* date){
    if(curr_package->isRemoved==0){
        ht->removed++;
        ht->current--;
        curr_package->isRemoved++;
    }
    curr_package->removalDate=date;
}

void generate_report(struct Hash_Table* ht){
    FILE *report;
    report = fopen(REPORT_FILE, "w");
    fprintf(report,"Pacman Packages Report\n----------------------\n- Installed packages : %d\n- Removed packages   : %d\n- Upgraded packages  : %d\n- Current installed  : %d\n", ht->installed,ht->removed,ht->upgraded,ht->current);
    fprintf(report, "\nList of packages\n----------------\n");
    for(int i=0;i<TABLE_SIZE;i++){
        if(ht->table[i]!=NULL){
            struct Package* temp = ht->table[i];
            fprintf(report,"- Package Name       : %s\n", temp->packageName);
            fprintf(report,"  - Install date     : %s\n",temp->installDate);
            if(temp->isUpgraded==1){
                fprintf(report,"  - Last update date : %s\n",temp->lastUpdate);
            } else {
                fprintf(report,"  - Last update date : -\n");
            }
            fprintf(report,"  - How many updates : %d\n", temp->upgradeNumber);
            if(temp->isRemoved==1){
                fprintf(report,"  - Removal date     : %s\n",temp->removalDate);
            } else {
                fprintf(report,"  - Removal date     : -\n");
            }
        }
    }
    fclose(report);
}

void analizeLog(char *logFile, char *report);

int main(int argc, char **argv) {

    if (argc < 2) {
	printf("Usage:./pacman-analizer.o pacman.log\n");
	return 1;
    }

    analizeLog(argv[1], REPORT_FILE);

    return 0;
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    int fd;
    struct Hash_Table* packages = hash_table();
    fd = open(logFile, O_RDONLY);
    /*off_t fs = lseek(fd,(size_t)0,SEEK_END);
    lseek(fd,(size_t)0,SEEK_SET);
    char buff[fs+1];
    read(fd, buff, fs*sizeof(char));
    buff[fs*sizeof(char)] = '\0';
    close(fd);*/
    if (fd==-1){
        perror("Something went wrong while opening the file");
        exit(-1);
    }
    off_t fs = lseek(fd,(size_t)0,SEEK_END);
    if (fs == -1){
        perror("Something went wrong while checking the size of the file");
        exit(-1);
    }
    lseek(fd,(size_t)0,SEEK_SET);
    char buff[fs+1];
    if (read(fd, buff, fs*sizeof(char))==-1){
        perror("Something went wrong while reading the file");
        exit(-1);
    }
    buff[fs*sizeof(char)] = '\0';
    if (close(fd)==-1) {
        perror("Something went wrong while closing the file");
        exit(-1);
    }
    generate_data(packages, buff, fs);
    generate_report(packages);

    printf("Report is generated at: [%s]\n", report);
}

