#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) 
{  
    char *list;
    int list_min, list_max, list_size;
    int prime, i, i_start;
    int count;
    char filename[100];
    FILE *f;

    list_min = 2;
    if (argc == 2)
        list_max = atol(argv[1]);
    if (argc != 2 || list_max == 0 || list_max < list_min)
    {
        fprintf(stderr, "Bad arguments!\nUsage:\n\tsoe_basic <max>\n\n");
        return 1;        
    }
    
    // reserve memory
    list_size = list_max-list_min+1;
    list = (char *)calloc(list_size, sizeof(char));

    // sieving
    prime = 2;
    while (prime*prime <= list_max)
    {
        // tag multiplies
        i_start = prime*prime-list_min;
        for (i = i_start; i < list_size; i += prime)
            list[i] = 1;
        // find next prime
        while (list[++prime-2]);
    }
    
    // write primes to file
    sprintf(filename, "primes_basic_%d_%d.txt", list_min, list_max);
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

    return 0;
}
