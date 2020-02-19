#include <stdio.h>
#include <stdlib.h>

/* print Fahrenheit-Celsius table */

int main(int argc, char **argv)
{
    int fahr;


    int start = strtol(argv[1], NULL, 10);
    int end = 0;
    int increment = 1;
    if(argc > 2){
    	end  = strtol(argv[2], NULL, 10);
    	increment = strtol(argv[3], NULL, 10);
    } 

    for (fahr = start; fahr <= end; fahr = fahr + increment)
	printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));

    return 0;
}
