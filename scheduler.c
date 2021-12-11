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
    struct Queue *Q = create();
    struct Process *currentProcess = (struct Process *)malloc(sizeof(struct Process));
    currentProcess->address = -1;
    // printf(" address = %d \n", (int)(currentProcess->address));
    int last_process = 0;
    while (1)
    {
        struct Process *to_receive = (struct Process *)malloc(sizeof(struct Process));
        // printf("i'm here \n");
        // printf(" address = %d \n", currentProcess->address);
        receiveProcessValue(fromGenToSchPro, 500, to_receive, &message);
        printf("Process received : \n");
        *to_receive = message.p;
        printProcess(to_receive);
        if (to_receive->last_process == 1)
            last_process = 1;
        // printf(" address = %d \n", currentProcess->address);
        // printProcess(&message.p);
        if (to_receive->null != 100)
        {
            // printf("entered after receiving vallid process \n");
            if (algorithm == 1)
            {
                // printf("displaying scheduler queue before\n");
                insert_by_priority(Q, to_receive, 'p');
                // printf(" address = %d \n", currentProcess->address);
                // printf("displaying scheduler queue \n");
                // display_queue(Q);
                // printf("displaying scheduler queue after\n");
            }
            else if (algorithm == 2)
            {
            }
            else if (algorithm == 3)
            {
            }
        }
        printf("passed nullity check \n");
        // printProcess(to_receive);
        // printf(" address = %d \n", currentProcess->address);
        printProcess(currentProcess);
        // printf("passed nullity check \n");
        if (currentProcess->address == -1)
        {
            struct Process *temp;
            temp = dequeue(Q);
            printf("dequiuing scheduler queue \n");
            printProcess(temp);
            if (!temp)
                continue;
            else
                *currentProcess = *temp;
            int process_id = fork();
            if (process_id == 0)
            {
                char *argv[] = {"./process.out", NULL};
                execv(argv[0], argv);
            }
            currentProcess->address = process_id;
            printProcess(currentProcess);
            // printf("new process id = %d \n", currentProcess->address);
            key_t from_scheduler_to_process;
            struct msgIntBuff *int_message = (struct msgIntBuff *)malloc(sizeof(struct msgIntBuff));
            // printf("%d \n", currentProcess->remaining_time);
            sendIntMesssage(from_scheduler_to_process, currentProcess->address, getpid(), currentProcess->remaining_time, int_message);
            // printf("message to send : %d %d %d %d %d\n", from_scheduler_to_process, currentProcess->address, getpid(), currentProcess->remaining_time, int_message->val);
            // printProcess(currentProcess);
            printf("\nMessage sent from scheduler at time %d : %d \n", getClk(), int_message->val);
            currentProcess->remaining_time--;
            if (currentProcess->remaining_time == 0)
            {
                currentProcess = (struct Process *)malloc(sizeof(struct Process));
                currentProcess->address = -1;
            }
            // raise(SIGSTOP);
        }
        else
        {
            // printf("entering else condition \n");
            // kill(currentProcess->address, SIGCONT);
            key_t from_scheduler_to_process;
            struct msgIntBuff *int_message = (struct msgIntBuff *)malloc(sizeof(struct msgIntBuff));
            sendIntMesssage(from_scheduler_to_process, currentProcess->address, getpid(), currentProcess->remaining_time, int_message);
            // printf("message to send : %d %d %d %d %d\n", from_scheduler_to_process, currentProcess->address, getpid(), currentProcess->remaining_time, int_message->val);
            // printProcess(currentProcess);
            // printf("%d \n",currentProcess->remaining_time);
            printf("\nMessage sent from scheduler at time %d : %d \n", getClk(), (currentProcess->remaining_time));
            currentProcess->remaining_time--;
            if (currentProcess->remaining_time == 0)
            {
                currentProcess = (struct Process *)malloc(sizeof(struct Process));
                currentProcess->address = -1;
                if (currentProcess->last_process == 1)
                    break;
            }
        }
        // printf(" address = %d \n", currentProcess->address);
    }
    // key_t from_scheduler_to_generator;
    // struct msgIntBuff *int_message = (struct msgIntBuff *)malloc(sizeof(struct msgIntBuff));
    // sendIntMesssage(from_scheduler_to_generator, getppid(), getpid(), 1, int_message);
    // printf("\nMessage sent from generator to scheduler at time %d : %d \n", getClk(),int_message->val);
    destroyClk(true);
}
