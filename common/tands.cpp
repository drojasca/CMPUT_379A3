#include "tands.h"
int TransSave = 0;

void Trans(int n)
{
    long i, j;

    // Use CPU cycles
    j = 0;
    for (i = 0; i < n * 100000; i++)
    {
        j += i ^ (i + 1) % (i + n);
    }
    TransSave += j;
    TransSave &= 0xff;
}

// Sleep simulates pauses between transactions
// Parameter given expresses wait time in hundreds of a nanosecond.

#include <stdio.h>
#include <time.h>

void Sleep(int n)
{
    struct timespec sleep;

    // Make sure pass a valid nanosecond time to nanosleep
    if (n <= 0 || n >= 100)
    {
        n = 1;
    }

    // Sleep for less than one second
    sleep.tv_sec = 0;
    sleep.tv_nsec = n * 10000000 + TransSave;
    if (nanosleep(&sleep, NULL) < 0)
    {
        perror("NanoSleep");
    }
}