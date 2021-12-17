#include "headers.h"
#include "time.h"
/* Modify this file as needed*/
int remainingtime;
// void SIGSTOPhandler();
// void SIGCONThandler();
// void SIGSTOPhandler()
// {
//     signal(SIGCONT, SIGCONThandler);
//     printf("has received a sigstop signal at time %d\n", getClk());
//     signal(SIGSTOP, SIG_DFL);
//     raise(SIGSTOP);
// }

// void SIGCONThandler()
// {
//     signal(SIGSTOP, SIGSTOPhandler);
//     printf("has received a sigcont signal at time %d\n", getClk());
//     signal(SIGCONT, SIG_DFL);
//     raise(SIGCONT);
// }
int main(int agrc, char *argv[])
{
    // signal(SIGSTOP, SIGSTOPhandler);
    // signal(SIGCONT, SIGCONThandler);
    char *a = argv[1];
    // printf("%s",a);
    int running_time = atoi(a);
    char *b = argv[2];
    // printf("%s",b);
    int start_time = atoi(b);
    printf("received in the process run %d start %d\n", running_time, start_time);
    initClk();
    key_t from_scheduler_to_process;
    struct msgIntBuff *message;
    int remainingtime;
    // receiveIntValue(from_scheduler_to_process, getpid(), &remainingtime, message);
    // TODO it needs to get the remaining time from somewhere
    // remainingtime = ??;
    while (clock() < running_time * CLOCKS_PER_SEC)
    {
        // sleep(0.5);
        // printf("I'm inside the process , remaining time = %d \n", remainingtime);
        // printf("I'm inside the process , with clock %d \n", getClk());
        // receiveIntValue(from_scheduler_to_process, getpid(), &remainingtime, message);
        // remainingtime = ??;
    }
    printf("the process is termenating at %d \n", getClk());
    destroyClk(false);

    return 0;
}
