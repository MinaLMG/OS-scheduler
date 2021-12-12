#include "headers.h"

/* Modify this file as needed*/
int remainingtime;

int main(int agrc, char * argv[])
{
    char* a = argv[1];
    // printf("%s",a);
    int running_time =atoi(a);
    char* b = argv[2];
    // printf("%s",b);
    int start_time =atoi(b);
    printf("received in the process run %d start %d\n",running_time,start_time);
    initClk();
    key_t from_scheduler_to_process;
    struct msgIntBuff * message;
    int remainingtime;
    receiveIntValue(from_scheduler_to_process,getpid(),&remainingtime,message);
    //TODO it needs to get the remaining time from somewhere
    //remainingtime = ??;
    while (remainingtime > 0)
    {
        printf("I'm inside the process , remaining time = %d \n",remainingtime);
        receiveIntValue(from_scheduler_to_process,getpid(),&remainingtime,message);
        // remainingtime = ??;
    }
    
    destroyClk(false);
    
    return 0;
}
