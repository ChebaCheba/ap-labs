#include <stdio.h>
#include <stdlib.h>
 
/* month_name:  return name of n-th month */
char *month_name(int n) {

   static char *name[] = {
       "Illegal month",
       "Jan", "Feb", "Mar",
       "Apr", "May", "Jun",
       "Jul", "Aug", "Sep",
       "Oct", "Nov", "Dec"
   };

   return (n < 1 || n > 12) ? name[0] : name[n];
}

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday){

    static int daytab[2][13] = {
        {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
    };
    int i, leap;
    leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
    i = 1;
    
    while (yearday>daytab[leap][i]){
        yearday -= daytab[leap][i];
        i++;
    }
    *pmonth = i;
    *pday = yearday;
}

int main(int argc, char *argv[]) {

    int year, yearday, *pmonth, *pday, i, j;
    i = 0;
    j = 0;
    if (argc < 3) {
        perror("error, not enough arguments\n");
        exit(EXIT_FAILURE);
    }

    long conv1 = strtol(argv[1], NULL, 10);
    long conv2 = strtol(argv[2], NULL, 10);

    year = conv1;
    yearday = conv2;

    pmonth = &i;
    pday = &j;
    
    if (yearday<1||yearday>365){
        perror("error, yearday out of range\n");
        exit(EXIT_FAILURE);
    } else if (year < 0){
        perror("error, year out of range\n");
        exit(EXIT_FAILURE);
    }

    month_day(year, yearday, pmonth, pday);
    

    if(*pday>9){
	printf("%s %d, %d\n", month_name(*pmonth), *pday, year);
    } else {
	printf("%s 0%d, %d\n", month_name(*pmonth), *pday, year);
    }

    return 0;
}


