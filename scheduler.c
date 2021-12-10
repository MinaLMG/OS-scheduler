#include "headers.h"
//*key_t queue_key,int get_value,int pid,int value_to_send,struct msgIntBuff* message)*/
void receiveIntValue(key_t queue_key, int get_value, int value_to_receive, struct msgIntBuff *message)
{
    queue_key = msgget(get_value, IPC_CREAT | 0644);
    // printf("queue key int value receiver :%d \n", queue_key);
    if (queue_key == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    int rec_val;
    rec_val = msgrcv(queue_key, message, sizeof(message->val), 0, !IPC_NOWAIT);
    // printf("has received %d with %d \n", (int)message->val, rec_val);
    if (rec_val == -1)
        perror("Error in receiving");
    else
    {
        value_to_receive = message->val;
        // printf("from function : %d \n", (int)message->val);
    }
}
void receiveProcessValue(key_t queue_key, int get_value, struct Process process_to_receive, struct msgProcessBuff *message)
{
    queue_key = msgget(get_value, IPC_CREAT | 0644);
    // printf("queue key value receiver :%d \n", queue_key);
    if (queue_key == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    int rec_val;
    rec_val = msgrcv(queue_key, message, sizeof(message->p), 0, !IPC_NOWAIT);
    // printf("has receivd\n");
    if (rec_val == -1)
        perror("Error in receiving");
    else
    {
        // process_to_receive = message->p;
        // printf("from function: \n");
        // printProcess(&process_to_receive);
    }
}
key_t fromGenToSchAlg;
key_t fromGenToSchPro;
int main(int argc, char *argv[])
{
    initClk();

    // TODO implement the scheduler :)
    // upon termination release the clock resources
    int algorithm;
    int rr = -1;
    struct msgProcessBuff message;
    struct msgIntBuff message2;
    /* receive the value of algorithm */
    receiveIntValue(fromGenToSchAlg, 550, algorithm, &message2);
    printf("\nAlgorithm received: %d\n", message2.val);
    if (algorithm == 3)
    {
        receiveIntValue(fromGenToSchAlg, 550, rr, &message2);
        printf("\n rr received: %d\n", message2.val);
    }
    struct Process to_receive;
    while (1)
    {
        // printf("i'm here \n");
        receiveProcessValue(fromGenToSchPro, 500, to_receive, &message);
        printf("Process received : \n");
        to_receive=message.p;
        // printProcess(&to_receive);
        printProcess(&message.p);
    }

    destroyClk(true);
}
