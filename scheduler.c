
#include "headers.h"
#include "time.h"
key_t fromGenToSchAlg;
key_t fromGenToSchPro;
char owner = 'c';
void handler(int signum)
{
    printf("i'm in the handler of sigusr1 \n");
    owner = 'p';
    raise(SIGALRM);
}
void alarmHandler(int signum)
{
    printf("i'm in the handler \n");
}
int main(int argc, char *argv[])
{
    initClk();
    signal(SIGUSR1, handler);
    signal(SIGALRM, alarmHandler);
    int start_time_stat = -1;
    int finish_time_stat;
    int runnig_time_stat = 0;
    int count_stat = 0;
    float total_WTA_stat = 0;
    int total_waiting_stat = 0;
    float avg_WTA_stat;
    float avg_waiting_stat;
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
    struct Queue *Qserved = create();
    struct Process *currentProcess = (struct Process *)malloc(sizeof(struct Process));
    currentProcess->address = -1;
    // printf(" address = %d \n", (int)(currentProcess->address));
    int last_process = 0;
    int finish = 0;
    int finish_time = -1;
    while (1)
    {
        if (algorithm == 1)
        {

            if (finish == 1)
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
                    if (start_time_stat == -1)
                        start_time_stat = getClk();
                    runnig_time_stat += to_receive->run_time;
                    count_stat++;
                    printf("displaying scheduler queue before\n");
                    display_queue(Q);
                    display_queue(Qserved);
                    insert_by_priority(Q, to_receive, 'p');
                    // printf(" address = %d \n", currentProcess->address);
                    printf("displaying scheduler queue \n");
                    display_queue(Q);
                    // printf("displaying scheduler queue after\n");
                }
                // printf("following1\n");
                // display_queue(Q);
                to_receive = (struct Process *)malloc(sizeof(struct Process));
                // printf("following2\n");
                // display_queue(Q);
                rec_val = receiveProcessValueNoWait(fromGenToSchPro, 500, to_receive, &message);
                // printf("following3\n");
                // display_queue(Q);
                printf("rec_val %d \n", rec_val);
                // printf("following4\n");
                // display_queue(Q);
            }
            printf("passed nullity check at %d\n", getClk());
            // printProcess(to_receive);
            // printf(" address = %d \n", currentProcess->address);
            printProcess(currentProcess);
            // printf("passed nullity check \n");
            struct Process *temp;
            temp = dequeue(Q);
            printf("dequiuing scheduler queue \n");
            printProcess(temp);
            if (!temp)
            {
                if (last_process)
                {
                    finish = 1;
                    // finish_time=
                }
                continue;
            }
            else
                *currentProcess = *temp;

            finish_time = getClk() + currentProcess->run_time;
            currentProcess->finish_time = getClk() + currentProcess->run_time;
            currentProcess->WTA = (float)(currentProcess->finish_time - currentProcess->arrival_time) / (float)currentProcess->run_time;
            finish_time_stat = currentProcess->finish_time;
            fprintf(fptr, "at time %d process %d started arr %d total %d remain %d wait %d \n",
                    getClk(),
                    currentProcess->id,
                    currentProcess->arrival_time,
                    currentProcess->run_time,
                    currentProcess->remaining_time,
                    getClk() - currentProcess->arrival_time - currentProcess->run_time + currentProcess->remaining_time);
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
            printf("i'm sleeping at %d for %d \n", getClk(), currentProcess->run_time);
            // alarm(currentProcess->run_time);
            pause();
            printf("i'm awaken at %d\n", getClk());
            // int pid, status;
            // pid = wait(&status);
            // if (!(status & 0x00FF))
            //     printf("\nA child with pid %d terminated with exit code %d\n\n", pid, status >> 8);
            // printf("new process id = %d \n", currentProcess->address);
            key_t from_scheduler_to_process;
            struct msgIntBuff *int_message = (struct msgIntBuff *)malloc(sizeof(struct msgIntBuff));
            // printf("%d \n", currentProcess->remaining_time);
            sendIntMesssage(from_scheduler_to_process, currentProcess->address, getpid(), currentProcess->remaining_time, int_message);
            // printf("message to send : %d %d %d %d %d\n", from_scheduler_to_process, currentProcess->address, getpid(), currentProcess->remaining_time, int_message->val);
            // printProcess(currentProcess);
            printf("\nMessage sent from scheduler at time %d : %d \n", getClk(), int_message->val);
            currentProcess->remaining_time = 0;
            insert_by_priority(Qserved, currentProcess, 'w');
            currentProcess->waiting_time = currentProcess->finish_time - currentProcess->arrival_time - currentProcess->run_time + currentProcess->remaining_time;
            fprintf(fptr, "at time %d d process %d finished arr %d total %d remain %d wait %d TA %d WTA %.2f \n",
                    getClk(),
                    currentProcess->id,
                    currentProcess->arrival_time,
                    currentProcess->run_time,
                    currentProcess->remaining_time,
                    currentProcess->finish_time - currentProcess->arrival_time - currentProcess->run_time + currentProcess->remaining_time,
                    currentProcess->finish_time - currentProcess->arrival_time,
                    (float)(currentProcess->finish_time - currentProcess->arrival_time) / (float)currentProcess->run_time);
            if (currentProcess->remaining_time == 0)
            {
                if (currentProcess->last_process == 1)
                    last_process = 1;
                currentProcess = (struct Process *)malloc(sizeof(struct Process));
                currentProcess->address = -1;
            }
            // raise(SIGSTOP);

            // printf(" address = %d \n", currentProcess->address);
        }
        else
        {
            // printf("i'm here \n");
            if (finish == 1)
                break;
            struct Process *to_receive = (struct Process *)malloc(sizeof(struct Process));
            // printf("i'm here \n");
            printf(" address = %d \n", currentProcess->address);
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
                    if (start_time_stat == -1)
                        start_time_stat = getClk();
                    runnig_time_stat += to_receive->run_time;
                    count_stat++;
                    printf("displaying scheduler queue before\n");
                    display_queue(Q);
                    if (algorithm == 2)
                        insert_by_priority(Q, to_receive, 'r');
                    else if (algorithm == 3)
                        insert_by_priority(Q, to_receive, 'w');
                    // printf(" address = %d \n", currentProcess->address);
                    printf("displaying scheduler queue \n");
                    display_queue(Q);
                    // printf("displaying scheduler queue after\n");
                }
                // printf("following1\n");
                // display_queue(Q);
                to_receive = (struct Process *)malloc(sizeof(struct Process));
                // printf("following2\n");
                // display_queue(Q);
                rec_val = receiveProcessValueNoWait(fromGenToSchPro, 500, to_receive, &message);
                // printf("following3\n");
                // display_queue(Q);
                printf("rec_val %d \n", rec_val);
                // printf("following4\n");
                // display_queue(Q);
            }
            printf("passed nullity check at %d\n", getClk());
            // printProcess(to_receive);
            // printf(" address = %d \n", currentProcess->address);
            printProcess(currentProcess);
            // printf("expression = %.2f \n",
            // (float) clock() /(float) CLOCKS_PER_SEC);
            // struct Process *temp;
            // temp = dequeue(Q);
            // printf("dequiuing scheduler queue \n");
            // printProcess(temp);
            // if (!temp)
            // {
            //     if (last_process)
            //     {
            //         finish = 1;
            //     }
            //     continue;
            // }
            // else
            //     *currentProcess = *temp;

            // finish_time = getClk() + currentProcess->run_time;
            // currentProcess->finish_time = getClk() + currentProcess->run_time;
            if (currentProcess->address != -1)
            {
                if (algorithm == 2)
                    insert_by_priority(Q, currentProcess, 'r');
                else if (algorithm == 3)
                    insert_by_priority(Q, currentProcess, 'w');
            }
            struct Process *toCompare = dequeue(Q);
            printf("to compare at time %d :\n", getClk());
            printProcess(toCompare);
            if (!toCompare)
            {
                if (last_process)
                {
                    finish = 1;
                }
                // finish_time = getClk() + currentProcess->run_time;
                // currentProcess->finish_time = getClk() + currentProcess->run_time;
                continue;
            }
            if (toCompare != currentProcess)
            {
                printf("tocompare!=current\n");
                printf("process adderss=%d\n", currentProcess->address);
                if (currentProcess->address != -1)
                {
                    printf("sending sigstop to id %d with pid %d at time %d\n", currentProcess->id, currentProcess->address, getClk());
                    kill(currentProcess->address, SIGSTOP);
                    fprintf(fptr, "at time %d process %d stopped arr %d total %d remain %d wait %d \n",
                            getClk(),
                            currentProcess->id,
                            currentProcess->arrival_time,
                            currentProcess->run_time,
                            currentProcess->remaining_time,
                            getClk() - currentProcess->arrival_time - currentProcess->run_time + currentProcess->remaining_time);
                    // sleep(1);
                }
                currentProcess = toCompare;
                if (currentProcess->address == -1)
                {

                    int process_id = fork();
                    if (process_id == 0)
                    {
                        char running_time_buffer[20]; // more than big enough for a 32 bit integer
                        snprintf(running_time_buffer, 20, "%d", currentProcess->run_time);
                        char starting_time_buffer[20]; // more than big enough for a 32 bit integer
                        snprintf(starting_time_buffer, 20, "%d", getClk());
                        char *argv[] = {"./process.out", running_time_buffer, starting_time_buffer, NULL};
                        execv(argv[0], argv);
                    }
                    currentProcess->address = process_id;
                    printProcess(currentProcess);
                    key_t from_scheduler_to_process;
                    struct msgIntBuff *int_message = (struct msgIntBuff *)malloc(sizeof(struct msgIntBuff));
                    sendIntMesssage(from_scheduler_to_process, currentProcess->address, getpid(), currentProcess->remaining_time, int_message);
                    printf("\nMessage sent from scheduler at time %d : %d \n", getClk(), int_message->val);
                    fprintf(fptr, "at time %d process %d started arr %d total %d remain %d wait %d \n",
                            getClk(),
                            currentProcess->id,
                            currentProcess->arrival_time,
                            currentProcess->run_time,
                            currentProcess->remaining_time,
                            getClk() - currentProcess->arrival_time - currentProcess->run_time + currentProcess->remaining_time);
                    // for(int k=0;k<Q->rear->arrival_time;k++){
                    //     Q->pri_que[k]->waiting_time++;
                    // }
                    sleepDetrmine(algorithm, rr, currentProcess, &owner);
                    if (currentProcess->remaining_time != 0)
                    {
                        printf("sending sigstop to id %d with pid %d at time %d\n", currentProcess->id, currentProcess->address, getClk());
                        kill(currentProcess->address, SIGSTOP);
                    }
                    printf("i'm awaken at %d\n", getClk());
                    if (currentProcess->remaining_time == 0)
                    {
                        if (algorithm == 2)
                        {
                            int pid, status;
                            printf("waiting for status\n");
                            pid = wait(&status);
                            if (!(status & 0x00FF))
                                printf("\nA child with pid %d terminated with exit code %d\n\n", pid, status >> 8);
                        }
                        currentProcess->finish_time = getClk();
                        currentProcess->WTA = (float)(currentProcess->finish_time - currentProcess->arrival_time) / (float)currentProcess->run_time;
                        currentProcess->waiting_time = currentProcess->finish_time - currentProcess->arrival_time - currentProcess->run_time + currentProcess->remaining_time;
                        finish_time_stat = currentProcess->finish_time;
                        insert_by_priority(Qserved, currentProcess, 'w');
                        fprintf(fptr, "at time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %.2f\n",
                                getClk(),
                                currentProcess->id,
                                currentProcess->arrival_time,
                                currentProcess->run_time,
                                currentProcess->remaining_time,
                                currentProcess->finish_time - currentProcess->arrival_time - currentProcess->run_time + currentProcess->remaining_time,
                                currentProcess->finish_time - currentProcess->arrival_time,
                                (float)(currentProcess->finish_time - currentProcess->arrival_time) / (float)currentProcess->run_time);
                        currentProcess = (struct Process *)malloc(sizeof(struct Process));
                        currentProcess->address = -1;
                    }
                    if (currentProcess->last_process == 1)
                        last_process = 1;
                }
                else
                {
                    printf("does it enter here ?\n");
                    key_t from_scheduler_to_process;
                    struct msgIntBuff *int_message = (struct msgIntBuff *)malloc(sizeof(struct msgIntBuff));
                    sendIntMesssage(from_scheduler_to_process, currentProcess->address, getpid(), currentProcess->remaining_time, int_message);
                    printf("\nMessage sent from scheduler at time %d : %d \n", getClk(), int_message->val);
                    // printf("i'm sleeping at %d for %d \n", getClk(), 1);
                    printf("sending sigcont to id %d with pid %d at time %d\n", currentProcess->id, currentProcess->address, getClk());
                    kill(currentProcess->address, SIGCONT);
                    fprintf(fptr, "at time %d process %d resumed arr %d total %d remain %d wait %d \n",
                            getClk(),
                            currentProcess->id,
                            currentProcess->arrival_time,
                            currentProcess->run_time,
                            currentProcess->remaining_time,
                            getClk() - currentProcess->arrival_time - currentProcess->run_time + currentProcess->remaining_time);

                    sleepDetrmine(algorithm, rr, currentProcess, &owner);
                    printf("process address=%d\n", currentProcess->address);
                    if (currentProcess->remaining_time != 0)
                    {
                        printf("sending sigstop to id %d with pid %d at time %d\n", currentProcess->id, currentProcess->address, getClk());
                        kill(currentProcess->address, SIGSTOP);
                    }
                    printf("i'm awaken at %d\n", getClk());
                    if (currentProcess->remaining_time == 0)
                    {
                        if (algorithm == 2)
                        {
                            int pid, status;
                            printf("waiting for status\n");
                            pid = wait(&status);
                            if (!(status & 0x00FF))
                                printf("\nA child with pid %d terminated with exit code %d\n\n", pid, status >> 8);
                        }
                        currentProcess->finish_time = getClk();
                        currentProcess->WTA = (float)(currentProcess->finish_time - currentProcess->arrival_time) / (float)currentProcess->run_time;
                        currentProcess->waiting_time = currentProcess->finish_time - currentProcess->arrival_time - currentProcess->run_time + currentProcess->remaining_time;
                        finish_time_stat = currentProcess->finish_time;
                        insert_by_priority(Qserved, currentProcess, 'w');
                        fprintf(fptr, "at time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %.2f\n",
                                getClk(),
                                currentProcess->id,
                                currentProcess->arrival_time,
                                currentProcess->run_time,
                                currentProcess->remaining_time,
                                currentProcess->finish_time - currentProcess->arrival_time - currentProcess->run_time + currentProcess->remaining_time,
                                currentProcess->finish_time - currentProcess->arrival_time,
                                (float)(currentProcess->finish_time - currentProcess->arrival_time) / (float)currentProcess->run_time);
                        currentProcess = (struct Process *)malloc(sizeof(struct Process));
                        currentProcess->address = -1;
                    }
                }
            }
            else
            {
                key_t from_scheduler_to_process;
                struct msgIntBuff *int_message = (struct msgIntBuff *)malloc(sizeof(struct msgIntBuff));
                sendIntMesssage(from_scheduler_to_process, currentProcess->address, getpid(), currentProcess->remaining_time, int_message);
                printf("\nMessage sent from scheduler at time %d : %d \n", getClk(), int_message->val);
                // printf("i'm sleeping at %d for %d \n", getClk(), 1);
                printf("sending sigcont to id %d with pid %d at time %d\n", currentProcess->id, currentProcess->address, getClk());
                kill(currentProcess->address, SIGCONT);
                sleepDetrmine(algorithm, rr, currentProcess, &owner);
                if (currentProcess->remaining_time != 0)
                {
                    printf("sending sigstop to id %d with pid %d at time %d\n", currentProcess->id, currentProcess->address, getClk());
                    kill(currentProcess->address, SIGSTOP);
                }
                printf("i'm awaken at %d\n", getClk());
                if (currentProcess->remaining_time == 0)
                {
                    if (algorithm == 2)
                    {
                        int pid, status;
                        printf("waiting for status\n");
                        pid = wait(&status);
                        if (!(status & 0x00FF))
                            printf("\nA child with pid %d terminated with exit code %d\n\n", pid, status >> 8);
                    }
                    currentProcess->finish_time = getClk();
                    currentProcess->WTA = (float)(currentProcess->finish_time - currentProcess->arrival_time) / (float)currentProcess->run_time;
                    currentProcess->waiting_time = currentProcess->finish_time - currentProcess->arrival_time - currentProcess->run_time + currentProcess->remaining_time;
                    finish_time_stat = currentProcess->finish_time;
                    insert_by_priority(Qserved, currentProcess, 'w');
                    fprintf(fptr, "at time %d process %d finished arr %d total %d remain %d wait %d TA %d WTA %.2f\n",
                            getClk(),
                            currentProcess->id,
                            currentProcess->arrival_time,
                            currentProcess->run_time,
                            currentProcess->remaining_time,
                            currentProcess->finish_time - currentProcess->arrival_time - currentProcess->run_time + currentProcess->remaining_time,
                            currentProcess->finish_time - currentProcess->arrival_time,
                            (float)(currentProcess->finish_time - currentProcess->arrival_time) / (float)currentProcess->run_time);
                    currentProcess = (struct Process *)malloc(sizeof(struct Process));
                    currentProcess->address = -1;
                }
            }
        }
    }
    fclose(fptr);
    // printf("the served processes\n");
    // display_queue(Qserved);
    FILE *fptr2;
    fptr2 = fopen("stats.txt", "w");
    if (fptr2 == NULL)
    {
        printf("Error! opening file");

        // Program exits if the file pointer returns NULL.
        exit(1);
    }
    printf("file is opened successfully\n");
    printf("finish_time_stat %d ,start_time_stat %d ,runnig_time_stat %d\n", finish_time_stat, start_time_stat, runnig_time_stat);
    float cpu_utilization = (float)(finish_time_stat - start_time_stat) * 100.0 / (float)(runnig_time_stat);
    // printf("nouran \n");
    printf("cpu utiliztioin = %.2f\n", cpu_utilization);
    // TODO:add percentage
    fprintf(fptr2, "CPU utiliztaion = %.2f %c\n", cpu_utilization, '%');
    for (int k = 0; k < Qserved->rear->arrival_time + 1; k++)
    {
        // Qserved->pri_que[k]->
        total_WTA_stat += Qserved->pri_que[k]->WTA;
        total_waiting_stat += Qserved->pri_que[k]->waiting_time;
    }
    printf("count = %d , total_WTA_stat = %.2f ,total_waiting_stat = %d \n", count_stat, total_WTA_stat, total_waiting_stat);
    avg_WTA_stat = (float)(total_WTA_stat) / (float)(count_stat);
    fprintf(fptr2, "Avg WTA = %.2f \n", avg_WTA_stat);
    avg_waiting_stat = (float)(total_waiting_stat) / (float)(count_stat);
    fprintf(fptr2, "Avg Waiting = %.2f \n", avg_waiting_stat);
    int k;
    float data[Qserved->rear->arrival_time + 1];
    for (k = 0; k < Qserved->rear->arrival_time + 1; k++)
        data[k] = Qserved->pri_que[k]->WTA;
    fprintf(fptr2, "Std WTA = %.2f \n", calculateSD(data, count_stat));
    fclose(fptr2);
    // key_t from_scheduler_to_generator;
    // struct msgIntBuff *int_message = (struct msgIntBuff *)malloc(sizeof(struct msgIntBuff));
    // sendIntMesssage(from_scheduler_to_generator, getppid(), getpid(), 1, int_message);
    // printf("\nMessage sent from generator to scheduler at time %d : %d \n", getClk(),int_message->val);
    destroyClk(false);
    removeMessageQueue(fromGenToSchAlg);
    removeMessageQueue(fromGenToSchPro);
    return 0;
}