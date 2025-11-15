#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <syslog.h>
#include <sys/utsname.h>  

#define NUM_THREADS 12

typedef struct
{
    int threadIdx;
} threadParams_t;


// POSIX thread declarations and scheduling attributes
//
pthread_t threads[NUM_THREADS];
threadParams_t threadParams[NUM_THREADS];


void *counterThread(void *threadp)
{
    syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:1] Hello World from Thread!");

    int sum=0, i;
    threadParams_t *threadParams = (threadParams_t *)threadp;

    for(i=1; i < (threadParams->threadIdx)+1; i++)
        sum=sum+i;
 
    // Write to syslog instead of printf
    syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:1] Thread idx=%d, sum[0...%d]=%d",
           threadParams->threadIdx,
           threadParams->threadIdx,
           sum);
}


int main (int argc, char *argv[])
{
   int rc;
   int i;

    // Open syslog
    openlog("pthread", LOG_PID | LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:1] Hello World from Main!");

    // Print uname -a first
    struct utsname sysinfo;
    if (uname(&sysinfo) == 0) {
        char unameStr[512];
        snprintf(unameStr, sizeof(unameStr), "%s %s %s %s %s",
                 sysinfo.sysname,
                 sysinfo.nodename,
                 sysinfo.release,
                 sysinfo.version,
                 sysinfo.machine);

        syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:1] %s", unameStr);
    }

   for(i=0; i < NUM_THREADS; i++)
   {
       threadParams[i].threadIdx=i;

       pthread_create(&threads[i],   // pointer to thread descriptor
                      (void *)0,     // use default attributes
                      counterThread, // thread function entry point
                      (void *)&(threadParams[i]) // parameters to pass in
                     );

   }

   for(i=0;i<NUM_THREADS;i++)
       pthread_join(threads[i], NULL);

    syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:1] TEST COMPLETE");

    closelog(); // Close syslog

}
