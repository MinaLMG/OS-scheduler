#include "headers.h"

key_t fromGenToSchAlg;
key_t fromGenToSchPro;
int main(int argc, char *argv[])
{
    initClk();

    // TODO implement the scheduler :)
    // upon termination release the clock resources
    FILE *fptr;
    fptr = fopen("output.txt", "w");
    if (fptr == NULL)
    {
        printf("Error! opening file");

        // Program exits if the file pointer returns NULL.
        exit(1);
    }
    fprintf(fptr, "at time x process y state arr w total z remain y wait k \n");
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
    int finish = 0;
    int finish_time = -1;
    while (1)
    {
        if (finish == 1 && finish_time == getClk())
            break;
        struct Process *to_receive = (struct Process *)malloc(sizeof(struct Process));
        // printf("i'm here \n");
        // printf(" address = %d \n", currentProcess->address);
        receiveProcessValue(fromGenToSchPro, 500, to_receive, &message);
        int rec_val = 1;
        while (rec_val != -1)
        {
            printf("Process received : at %d \n", getClk());
            *to_receive = message.p;
            printProcess(to_receive);
            printf("at %d finish = %d , finish_time =%d \n", getClk(), finish, finish_time);
            if (to_receive->last_process == 1)
                last_process = 1;
            // printf(" address = %d \n", currentProcess->address);
            // printProcess(&message.p);
            if (to_receive->null != 100)
            {
                // printf("entered after receiving vallid process \n");
                if (algorithm == 1)
                {
                    printf("displaying scheduler queue before\n");
                    display_queue(Q);
                    insert_by_priority(Q, to_receive, 'p');
                    // printf(" address = %d \n", currentProcess->address);
                    printf("displaying scheduler queue \n");
                    display_queue(Q);
                    // printf("displaying scheduler queue after\n");
                }
                else if (algorithm == 2)
                {
                }
                else if (algorithm == 3)
                {
                }
            }
            printf("following1\n");
            display_queue(Q);
            to_receive = (struct Process *)malloc(sizeof(struct Process));
            printf("following2\n");
            display_queue(Q);
            rec_val = receiveProcessValueNoWait(fromGenToSchPro, 500, to_receive, &message);
            printf("following3\n");
            display_queue(Q);
            printf("rec_val %d \n", rec_val);
            printf("following4\n");
            display_queue(Q);
        }
        printf("passed nullity check at %d\n", getClk());
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
            {
                if (last_process)
                {
                    finish = 1;
                }
                continue;
            }
            else
                *currentProcess = *temp;

            finish_time = getClk() + currentProcess->run_time;
            currentProcess->finish_time = getClk() + currentProcess->run_time;
            fprintf(fptr, "at time %d process %d started arr %d total %d remain %d wait 0 TA %d WTA %.2f \n",
                    getClk(),
                    currentProcess->id,
                    currentProcess->arrival_time,
                    currentProcess->run_time,
                    currentProcess->remaining_time,
                    currentProcess->finish_time - currentProcess->arrival_time,
                    (float)(currentProcess->finish_time - currentProcess->arrival_time) / (float)currentProcess->run_time);
            int process_id = fork();
            if (process_id == 0)
            {
                /*int number, reverse,k;
                char string[20];
                number = currentProcess->run_time;
                int index = 0;
                printf("number %d index %d \n",number,index);
                if (number > 0 || number == 0)
                {
                    do
                    {
                        reverse = number % 10;
                        printf("reverse %d\n",reverse);
                        string[index++] = reverse;
                        number /= 10;
                    } while (number != 0);
                    printf("do while terminated \n");
                }
                printf("sending \n");
                for (k = index; k >= 0; k--)
                    printf("%c \n", string[k]);*/
                char running_time_buffer[20]; // more than big enough for a 32 bit integer
                snprintf(running_time_buffer, 20, "%d", currentProcess->run_time);
                char starting_time_buffer[20]; // more than big enough for a 32 bit integer
                snprintf(starting_time_buffer, 20, "%d", getClk());
                char *argv[] = {"./process.out", running_time_buffer, starting_time_buffer, NULL};
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
                if (currentProcess->last_process == 1)
                    last_process = 1;
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
                // if (currentProcess->last_process == 1)
                // {
                //     printf("before breaking /n");
                //     printProcess(currentProcess);
                //     break;
                // }
                currentProcess = (struct Process *)malloc(sizeof(struct Process));
                currentProcess->address = -1;
            }
        }
        // printf(" address = %d \n", currentProcess->address);
    }
    fclose(fptr);
    // key_t from_scheduler_to_generator;
    // struct msgIntBuff *int_message = (struct msgIntBuff *)malloc(sizeof(struct msgIntBuff));
    // sendIntMesssage(from_scheduler_to_generator, getppid(), getpid(), 1, int_message);
    // printf("\nMessage sent from generator to scheduler at time %d : %d \n", getClk(),int_message->val);
    destroyClk(true);
}
