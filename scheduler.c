#include "headers.h"

key_t fromGenToSch;
key_t fromSchToGen;
int main(int argc, char * argv[])
{
    initClk();
    
    //TODO implement the scheduler :)
    //upon termination release the clock resources
    pid_t pid;
    fromGenToSch=msgget(500, IPC_CREAT | 0644);
    // fromGenToSch = msgget(IPC_PRIVATE, 0644); // or msgget(12613, IPC_CREAT | 0644)
    if(fromGenToSch == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    printf("fromGenToSch = %d\n", fromGenToSch);

    fromSchToGen=msgget(550, IPC_CREAT | 0644);
    // fromGenToSch = msgget(IPC_PRIVATE, 0644); // or msgget(12613, IPC_CREAT | 0644)
    if(fromSchToGen == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    printf("fromSchToGen = %d\n", fromSchToGen);

    int rec_val;

    struct msgbuff message;

    while(1){
        /* receive all types of messages */
        rec_val = msgrcv(fromGenToSch, &message, sizeof(message.mtext), 0, !IPC_NOWAIT);
        // sleep(2);
        if(rec_val == -1)
            perror("Error in receiving");
        else
        {   
            // msgctl(fromGenToSch, IPC_RMID, (struct msqid_ds *) 0);
            printf("\nMessage received: %s\n", message.mtext);
            char str[256]="";
            strcpy(str,message.mtext);
            int send_val;
            struct msgbuff message2;
            message2.mtype = message.mtype;     	/* arbitrary value */
            strcpy(message2.mtext, str);
            printf("\nMessage to reply with : %s\n", message2.mtext);
            send_val = msgsnd(fromSchToGen, &message2, sizeof(message2.mtext), !IPC_NOWAIT);
        }

    }


    destroyClk(true);
}
