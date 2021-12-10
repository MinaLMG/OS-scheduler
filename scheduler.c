#include "headers.h"

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
    receiveIntValue(fromGenToSchAlg, 550, &algorithm, &message2);
    printf("\nAlgorithm received: %d\n", message2.val);
    // algorithm=(int)message2.val;
    printf("algorithm = %d \n", algorithm);
    if (algorithm == 3)
    {
        receiveIntValue(fromGenToSchAlg, 550, &rr, &message2);
        printf("\n rr received: %d\n", message2.val);
        rr = message2.val;
    }
    struct Process to_receive;
    while (1)
    {
        // printf("i'm here \n");
        receiveProcessValue(fromGenToSchPro, 500, &to_receive, &message);
        printf("Process received : \n");
        to_receive = message.p;
        printProcess(&to_receive);
        // printProcess(&message.p);
    }

    destroyClk(true);
}
