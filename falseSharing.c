#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#define COUNT 10

long array[513] = {0};

void *writeToFirstCell()
{
    //thread writes to the first cell of array
    for (int i = 0; i < 9999999; i++)
    {
        array[0]++;
    }
    pthread_exit(NULL);
}

void *writeToOtherCell(void *position)
{
    //thread writes to the cell on given position in array
    for (int i = 0; i < 9999999; i++)
    {
        array[*((int*) &position)]++;
    }
    pthread_exit(NULL);
}

void withFalseSharing()
{
    struct timeval start, end;

    gettimeofday(&start, NULL); //start the timer

    pthread_t threads[COUNT];
    pthread_t threads2[COUNT];

    for(int i = 0; i < COUNT; i++) //create more threads so the difference is more obvious
    {
        pthread_create(&threads[i], NULL, writeToFirstCell, NULL);
        pthread_create(&threads2[i], NULL, writeToOtherCell, (void *)1);
    }

    for(int i = 0; i < COUNT; i++)
    {
        pthread_join(threads[i], NULL);
        pthread_join(threads2[i], NULL);
    }

    gettimeofday(&end, NULL); //stop the timer

    printf("Time spent with false sharing: %f\n",
                ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6);
}

void withoutFalseSharing()
{
    struct timeval start, end;

    gettimeofday(&start, NULL); //start the timer

    pthread_t threads[COUNT];
    pthread_t threads2[COUNT];

    for(int i = 0; i < COUNT; i++)
    {
        pthread_create(&threads[i], NULL, writeToFirstCell, NULL);
        pthread_create(&threads2[i], NULL, writeToOtherCell, (void *)500);
    }

    for(int i = 0; i < COUNT; i++)
    {
        pthread_join(threads[i], NULL);
        pthread_join(threads2[i], NULL);
    }


    gettimeofday(&end, NULL); //stop the timer

    printf("Time spent without false sharing: %f\n",
            ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6);
}

int main()
{
    printf("409920, false sharing\n");

    withoutFalseSharing();
    withFalseSharing();

    return 0;
}
