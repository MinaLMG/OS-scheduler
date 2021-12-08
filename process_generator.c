#include "headers.h"

void clearResources(int);

int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.
    FILE *fptr;
    fptr = fopen("process.txt", "r");

    int num;

    if (fptr == NULL)
    {
        printf("Error! opening file");

        // Program exits if the file pointer returns NULL.
        exit(1);
    }
    struct Queue *q = create();
    while (!feof(fptr))
    {
        // person * myperson = (person *) malloc(sizeof(person));
        // struct Process p1;
        struct Process *p = (struct Process *)malloc(sizeof(struct Process));
        char firstString[256];
        fscanf(fptr, "%s", firstString);
        if (firstString[0] == '#')
        {
            char ignore[1024];
            fgets(ignore, sizeof(ignore), fptr);
            continue;
        }
        else
        {
            /*printf("doesn't start with #");*/
            p->id = strtol(firstString, NULL, 10);
        }

        // printf("Value of id =%d \n", (p->id));

        fscanf(fptr, "%d", &p->run_time);

        // printf("Value of run time =%d \n", (p->run_time));

        fscanf(fptr, "%d", &p->arrival_time);

        // printf("Value of arrival time =%d \n", (p->arrival_time));

        fscanf(fptr, "%d", &p->priority);

        // printf("Value of priority =%d \n", (p->priority));

        p->remaining_time = p->run_time;
        p->finish_time = -1;
        // printProcess(p);

        insert_by_priority_arrival_time(q, p);
        // printProcess( pri_que[0]);
        // printf("after a loop in file:\n");
        // display_pqueue();
    }
    // printProcess( pri_que[1]);
    display_queue(q);
    /*for (int i = 0; i < 6; i++)
    {
        struct Process *temp = dequeue(q);
        printProcess(temp);
        display_queue(q);
    }*/
    fclose(fptr);
    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    int algorithm;
    printf("enter the hoped algorithm number\n1 for non-preemptive HPF\n2 for SRTN\n3 for round-robin :\n ");
    scanf("%d", &algorithm);
    while (algorithm > 3 || algorithm < 1)
    {
        printf("you entered invalid number \n1 for non-preemptive HPF\n2 for SRTN\n3 for round-robin :\n ");
        scanf("%d", &algorithm);
    }
    int rr = -1;
    if (algorithm == 3)
    {
        printf("enter the quantum value\n ");
        scanf("%d", &rr);
        while (rr < 0)
        {
            printf("enter positive value for quantum\n ");
            scanf("%d", &rr);
        }
    }
    printf("Value of algorithm =%d \n", algorithm);

    // 3. Initiate and create the scheduler and clock processes.

    /*** initializing clock***/
    int clockId = fork();
    // printf("clokid= %d \n ",clockId);
    if (clockId == -1)
        printf("error in fork ");
    else if (clockId == 0)
    {
        char *argv[] = {"./clk.out", NULL};
        execv(argv[0], argv);
    }
    else
    {
        // 4. Use this function after creating the clock process to initialize clock
        initClk();

        // while(1){
        // sleep(2);

        int x = getClk();
        printf("current time is %d\n", x);

        // }
        // TODO Generation Main Loop
        // 5. Create a data structure for processes and provide it with its parameters.

        /*** implemented in header ****/

        // 6. Send the information to the scheduler at the appropriate time.
        int schedulerId = fork();
        if (schedulerId == -1)
        {
            printf("error in forking\n");
        }
        else if (schedulerId == 0)
        {
            char *argv[] = {"./schedular.out", NULL};
            execv(argv[0], argv);
        }
        else
        {
            // 7. Clear clock resources
            destroyClk(true);
        }
    }
}

void clearResources(int signum)
{
    // TODO Clears all resources in case of interruption
}

/* Driver program to test above function */
/*int main()
{
    struct Node *start = NULL;
    // Create and print an int linked list
    unsigned int_size = sizeof(int);
    int arr[] = {10, 20, 30, 40, 50}, i;
    for (i=4; i>=0; i--)
       push(&start, &arr[i], int_size);
    printf("Created integer linked list is \n");
    printList(start, printInt);
    // Create and print a float linked list
    unsigned float_size = sizeof(float);
    start = NULL;
    float arr2[] = {10.1, 20.2, 30.3, 40.4, 50.5};
    for (i=4; i>=0; i--)
       push(&start, &arr2[i], float_size);
    printf("\n\nCreated float linked list is \n");
    printList(start, printFloat);
    return 0;
}*/