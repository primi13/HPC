#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[]) 
{  
    char *list;
    char *listsupport;
    int list_size, listsupport_size;
    int list_min, list_max;
    int chunk_min, chunk_max, chunk_size;
    int prime, i, i_start;
    int count;
    char filename[100];
    FILE *f;

    if (argc == 4)
    {
        list_min = atol(argv[1]);
        list_max = atol(argv[2]);
        chunk_size = atol(argv[3]);
    }
    if (argc != 4 || list_min == 0 || list_max == 0 || list_max < list_min || chunk_size == 0)
    {
        fprintf(stderr, "Bad arguments!\nUsage:\n\tsoe <min> <max> <chunk>\n\n");
        return 1;        
    }
    if (list_min < 2)
        list_min = 2;

    // reserve memory
    list_size = list_max-list_min+1;
    list = (char *)calloc(list_size, sizeof(char));

    listsupport_size = (int)sqrt(list_max);
    listsupport = (char *)calloc(listsupport_size, sizeof(char));
    
    // chunks
    chunk_min = list_min;
    while(chunk_min <= list_max)
    {
        chunk_max = chunk_min + chunk_size - 1;
        if (chunk_max > list_max)
        {
            chunk_max = list_max;
            chunk_size = chunk_max - chunk_min + 1;
        }

        //sieving
        for (i = 0; i < listsupport_size; i++)
            listsupport[i] = 0;
        prime = 2;
        while (prime*prime <= chunk_max)
        {
            // tag multiplies
            // listsupport
            for (i = prime*prime-2; i < listsupport_size; i += prime)
                listsupport[i] = 1;
            // list
            if (prime*prime > chunk_min)
                i_start = prime*prime - chunk_min;
            else if (chunk_min % prime == 0)
                i_start = 0;     
            else
                i_start = prime - (chunk_min % prime);
            i_start += chunk_min - list_min;
            for (i = i_start; i < chunk_size + chunk_min - list_min; i += prime)
                list[i] = 1;                
            // find next prime
            while (listsupport[++prime-2]);
        }

        chunk_min += chunk_size;
    }

    // write primes to file
    sprintf(filename, "primes_chunks_%d_%d.txt", list_min, list_max);
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
