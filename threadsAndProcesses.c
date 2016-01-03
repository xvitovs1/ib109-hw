#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <wait.h>
#include <pthread.h>
#include <unistd.h>
#define COUNT 250

void *doSomething()
{
    pthread_exit(NULL);
}

void measureThreads()
{
    struct timeval start, end;

    gettimeofday(&start, NULL); //start the timer

    //create threads
    pthread_t threads[COUNT];
    for(int i = 0; i < COUNT; i++)
    {
        pthread_create(&threads[i], NULL, doSomething, NULL);
    }

    for(int i = 0; i < COUNT; i++)
    {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL); //stop the timer

    printf("Time spent creating %d threads: %f\n", COUNT,
                ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6);
}

void measureProcesses()
{
    struct timeval start, end;
    gettimeofday(&start, NULL); //start the timer

    //create processes
    int processes[COUNT];
    int status = 0;
    for(int i = 0; i < COUNT; i++)
    {
        processes[i] = fork();
        if (processes[i] == 0)
        {
            _exit(0);
        }
        else if(processes[i] == -1)
        {
            perror("fork - fail\n");
	        return;
        }

        for (int i = 0; i < COUNT; i++)
        {
	      waitpid(processes[i], &status, 0);
        }
    }
    gettimeofday(&end, NULL); //stop the timer

    printf("Time spent creating %d processes: %f\n", COUNT,
                ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6);
}

int main()
{
    printf("409920, creating threads and processes\n");

    measureThreads();
    measureProcesses();

    return 0;
}
