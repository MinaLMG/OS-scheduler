#include "headers.h"

void clearResources(int);

int main(int argc, char * argv[])
{
    signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.
    FILE *fptr;
    fptr = fopen("process.txt","r");
    
    int num;

    if (fptr == NULL){
        printf("Error! opening file");

        // Program exits if the file pointer returns NULL.
        exit(1);
    }
    while(!feof(fptr)){
        struct Process p1;
        struct Process *p=&p1;
        fscanf(fptr,"%d", &p->id);

        printf("Value of id =%d \n", (p->id));

        fscanf(fptr,"%d",&p->run_time);

        printf("Value of run time =%d \n", (p->run_time));

        fscanf(fptr,"%d", &p->arrival_time);

        printf("Value of arrival time =%d \n", (p->arrival_time));

        fscanf(fptr,"%d", &p->priority);

        printf("Value of priority =%d \n", (p->priority));
    }

    fclose(fptr);
    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    // 3. Initiate and create the scheduler and clock processes.
    // 4. Use this function after creating the clock process to initialize clock
    initClk();
    // To get time use this
    int x = getClk();
    printf("current time is %d\n", x);
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    // 7. Clear clock resources
    destroyClk(true);
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
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