#include <stdio.h>
#include "logger.h"

int main() {
    infof("INFO Message %d\n", 1);
    warnf("WARN Message %d\n", 2);
    errorf("ERROR Message %d\n", 2);
    panicf("Big Chale %s %d\n", "moment", 3);
    printf("This should not be printed");
    return 0;
}

