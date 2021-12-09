#include "headers.h"

key_t fromGenToSchAlg;
key_t fromGenToSchPro;
int main(int argc, char *argv[])
{
    initClk();

    // TODO implement the scheduler :)
    // upon termination release the clock resources
    pid_t pid;
    fromGenToSchPro = msgget(500, IPC_CREAT | 0644);
    if (fromGenToSchPro == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    // printf("fromGenToSchPro = %d\n", fromGenToSchPro);

    fromGenToSchAlg = msgget(550, IPC_CREAT | 0644);
    if (fromGenToSchAlg == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    // printf("fromGenToSchAlg = %d\n", fromGenToSchAlg);

    int rec_val;
    int algorithm;
    int rr = -1;
    struct msgProcessBuff message;
    struct msgAlgorithmBuff message2;
    /* receive the value of algorithm */
    rec_val = msgrcv(fromGenToSchAlg, &message2, sizeof(message2.val), 0, !IPC_NOWAIT);
    // sleep(2);
    if (rec_val == -1)
        perror("Error in receiving");
    else
    {
        // msgctl(fromGenToSchPro, IPC_RMID, (struct msqid_ds *) 0);
        printf("\nAlgorithm received: %d\n", message2.val);
        algorithm = message2.val;
    }
    if (algorithm == 3)
    {
        rec_val = msgrcv(fromGenToSchAlg, &message2, sizeof(message2.val), 0, !IPC_NOWAIT);
        // sleep(2);
        if (rec_val == -1)
            perror("Error in receiving");
        else
        {
            // msgctl(fromGenToSchPro, IPC_RMID, (struct msqid_ds *) 0);
            printf("\n rr received: %d\n", message2.val);
            rr = message2.val;
        }
    }
    while (1)
    {
        /* receive all types of messages */
        rec_val = msgrcv(fromGenToSchPro, &message, sizeof(message.p), 0, !IPC_NOWAIT);
        // sleep(2);
        if (rec_val == -1)
            perror("Error in receiving");
        else
        {
            // msgctl(fromGenToSchPro, IPC_RMID, (struct msqid_ds *) 0);
            printf("\nMessage received: \n");

            printProcess(&message.p);
        }
    }

    destroyClk(true);
}
