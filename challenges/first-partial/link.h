struct Package {
    char* packageName;
    char* lastUpdate;
    char* installDate;
    char* removalDate;
    int isUpgraded, upgradeNumber, isRemoved;
};

struct Hash_Table {
    int installed, removed, upgraded, current;
    struct Package* table[];
};

struct Package* package(char*, char*);
struct Hash_Table* hash_table();

void insert_p(struct Hash_Table*, char*, char*);
struct Package* search_p(struct Hash_Table*, char* name);
void update_p(struct Hash_Table*, struct Package*, char*);
void remove_p(struct Hash_Table*, struct Package*, char*);
