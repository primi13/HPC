#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[]) 
{  
    char *list;
    char *listsupport;
    int list_size, listsupport_size;
    int list_min, list_max;
    int prime, i, i_start;
    int count;
    char filename[100];
    FILE *f;

    if (argc == 3)
    {
        list_min = atol(argv[1]);
        list_max = atol(argv[2]);
    }
    if (argc != 3 || list_min == 0 || list_max == 0 || list_max < list_min)
    {
        fprintf(stderr, "Bad arguments!\nUsage:\n\tsoe <min> <max>\n\n");
        return 1;        
    }
    if (list_min < 2)
        list_min = 2;

    // reserve memory
    list_size = list_max-list_min+1;
    list = (char *)calloc(list_size, sizeof(char));

    listsupport_size = (int)sqrt(list_max);
    listsupport = (char *)calloc(listsupport_size, sizeof(char));
    
    // sieving
    prime = 2;
    while (prime*prime <= list_max)
    {
        // tag multiplies
        // listsupport
        i_start = prime*prime-2;
        for (i = i_start; i < listsupport_size; i += prime)
            listsupport[i] = 1;
        // list
        if (prime*prime > list_min)
            i_start = prime*prime-list_min;
        else if (list_min % prime == 0)
            i_start = 0;     
        else
            i_start = prime - (list_min % prime);
        for (i = i_start; i < list_size; i += prime)
            list[i] = 1;
        // find next prime
        while (listsupport[++prime-2]);
    }

    // write primes to file
    sprintf(filename, "primes_range_%d_%d.txt", list_min, list_max);
    if( (f = fopen(filename, "w")) != NULL )
    {
        count = 0;
        for(i = 0; i < list_size; i++)
            if(list[i] == 0)
            {
//                fprintf(f, "%ld\n", i+list_min);
                count++;
            }
        fprintf(f, "----------\ninterval[%ld, %ld]\n%ld\n", list_min, list_max, count);
        fclose(f);
    }

    // free memory
    free(list);
    free(listsupport);

    return 0;
}
