#include "headers.h"

/* Modify this file as needed*/
int remainingtime;

int main(int agrc, char * argv[])
{
    initClk();
    printf("fucken project \n");
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
