#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "CurrentID.h"

void* chird(void* arg)
{
    pid_t id = getCtid();
    printf("Child id = %d\n", id);
}

int main(int argc, char **argv)
{
    pid_t id = getCtid();
    printf("main id = %d\n", id);
    pthread_t pID;
    if( 0 != pthread_create(&pID, NULL, chird, NULL))
    {
        printf("create pthread error\n");
        exit(0);
    }
    printf("pthread id = %ld\n", pID);
    pthread_join(pID, NULL);
    return 0;
}