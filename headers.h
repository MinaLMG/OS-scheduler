#include <stdio.h> //if you don't use scanf/printf change this include
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


typedef short bool;
#define true 1
#define false 0

#define SHKEY 300

///==============================
// don't mess with this variable//
int *shmaddr; //
//===============================

int getClk()
{
    return *shmaddr;
}

/*
 * All process call this function at the beginning to establish communication between them and the clock module.
 * Again, remember that the clock is only emulation!
 */
void initClk()
{
    int shmid = shmget(SHKEY, 4, 0444);
    while ((int)shmid == -1)
    {
        // Make sure that the clock exists
        printf("Wait! The clock not initialized yet!\n");
        sleep(1);
        shmid = shmget(SHKEY, 4, 0444);
    }
    shmaddr = (int *)shmat(shmid, (void *)0, 0);
}

/*
 * All process call this function at the end to release the communication
 * resources between them and the clock module.
 * Again, Remember that the clock is only emulation!
 * Input: terminateAll: a flag to indicate whether that this is the end of simulation.
 *                      It terminates the whole system and releases resources.
 */

void destroyClk(bool terminateAll)
{
    shmdt(shmaddr);
    if (terminateAll)
    {
        killpg(getpgrp(), SIGINT);
    }
}

// C program for generic linked list
#include <stdio.h>
#include <stdlib.h>

/* A linked list node */
struct Node
{
    // Any data type can be stored in this node
    void *data;

    struct Node *next;
};

/* Function to add a node at the beginning of Linked List.
   This function expects a pointer to the data to be added
   and size of the data type */
void push(struct Node **head_ref, void *new_data, size_t data_size)
{
    // Allocate memory for node
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));

    new_node->data = malloc(data_size);
    new_node->next = (*head_ref);

    // Copy contents of new_data to newly allocated memory.
    // Assumption: char takes 1 byte.
    int i;
    for (i = 0; i < data_size; i++)
        *(char *)(new_node->data + i) = *(char *)(new_data + i);

    // Change head pointer as new node is added at the beginning
    (*head_ref) = new_node;
}

/* Function to print nodes in a given linked list. fpitr is used
   to access the function to be used for printing current node data.
   Note that different data types need different specifier in printf() */
void printList(struct Node *node, void (*fptr)(void *))
{
    while (node != NULL)
    {
        (*fptr)(node->data);
        node = node->next;
    }
}

// Function to print an integer
void printInt(void *n)
{
    printf(" %d", *(int *)n);
}

// Function to print a float
void printFloat(void *f)
{
    printf(" %f", *(float *)f);
}

/**** process structure *****/
struct Process
{
    // Any data type can be stored in this node
    int id;
    int run_time;
    signed int arrival_time;
    int priority;
    int remaining_time;
    int finish_time;
};
void printProcess(struct Process *p)
{
    if (p != NULL)
    {
        printf("the process has ... id = %d \n run_time = %d \n arrival_time = %d \n priority = %d \n remaining_time = %d \n finish_time= %d \n ",
               p->id,
               p->run_time,
               p->arrival_time,
               p->priority,
               p->remaining_time,
               p->finish_time);
    }
    else
    {
        printf("Null Process!!\n");
    }
}
/*

     * C Program to Implement Priority Queue to Add and Delete Elements

     */

#include <stdio.h>

#include <stdlib.h>

#define MAX 500

struct Queue
{
    struct Process *rear;
    struct Process *front;
    struct Process *pri_que[MAX];
};

void insert_by_priority(struct Queue *, struct Process *, char);

void delete_by_priority(struct Queue *, struct Process *);
struct Process *dequeue(struct Queue *);

struct Queue *create();

void check(struct Queue *, struct Process *,char);

void display_queue(struct Queue *);

/*void main()

{

    int n, ch;

    printf("\n1 - Insert an element into queue");

    printf("\n2 - Delete an element from queue");

    printf("\n3 - Display queue elements");

    printf("\n4 - Exit");

    create();

    while (1)

    {

        printf("\nEnter your choice : ");

        scanf("%d", &ch);

        switch (ch)

        {

        case 1:

            printf("\nEnter value to be inserted : ");

            scanf("%d", &n);

            // insert_by_priority(n);

            break;

        case 2:

            printf("\nEnter value to delete : ");

            scanf("%d", &n);

            // delete_by_priority(n);

            break;

        case 3:

            display_pqueue();

            break;

        case 4:

            exit(0);

        default:

            printf("\nChoice is incorrect, Enter a correct choice");
        }
    }
}*/

/* Function to create an empty priority queue */
struct Process p1;
struct Process *pptr1 = &p1;
struct Process p2;
struct Process *pptr2 = &p2;

struct Queue *create()
{
    struct Queue *q = (struct Queue *)malloc(sizeof(struct Queue));
    pptr1->arrival_time = -1;
    q->front = pptr1;
    pptr2->arrival_time = -1;
    q->rear = pptr2;
    return q;
    // printf("rearin create =%d\n ", rear->arrival_time);
}
//----------------------------------dequeue
struct Process *dequeue(struct Queue *q)
{
    if (q->front->arrival_time == -1 && q->rear->arrival_time == -1)
    {
        printf("The queue is empty!\n");
        return NULL;
    }
    else if (q->rear->arrival_time == 0)
    {
        q->rear->arrival_time = -1;
        q->front->arrival_time = -1;
        struct Process *temp = q->pri_que[0];
        q->pri_que[0] = NULL;
        return temp;
    }
    else
    {
        q->rear->arrival_time--;
        struct Process *temp = q->pri_que[0];
        for (int i = 0; i <= q->rear->arrival_time; i++)
        {
            q->pri_que[i] = q->pri_que[i + 1];
        }
        q->pri_que[q->rear->arrival_time + 1] = NULL;
        return temp;
    }
}
//----------------------------------------
/* Function to insert value into priority queue */

void insert_by_priority(struct Queue *q, struct Process *p, char type)

{

    if (q->rear->arrival_time >= MAX - 1)

    {

        printf("\nQueue overflow no more elements can be inserted");

        return;
    }

    if ((q->front->arrival_time == -1) && (q->rear->arrival_time == -1))

    {
        // printf("rear in first initialization=%d\n ", rear->arrival_time);
        // printf("inserting the first process ...rear = %d \n ", rear->arrival_time);
        q->front->arrival_time += 1;

        q->rear->arrival_time += 1;
        // printf("q->rear after ++ =%d\n ", q->rear->arrival_time);
        q->pri_que[q->rear->arrival_time] = p;

        return;
    }

    else
        // q->rear->arrival_time--;
        // printf("q->rear before check=%d\n ", q->rear->arrival_time);
        // display_pqueue();
        // printProcess(p);
        check(q, p, type);
    // printf("ana hena");
    q->rear->arrival_time++;
}



void check(struct Queue *q, struct Process *p, char type)

{

    // printf("inserting another process ..q->rear = %d\n ", q->rear->arrival_time);
    int i, j;
    // printf("from check function\n");
    // display_pqueue();
    // printf("ana aho %d\n", q->rear->arrival_time);
    for (i = 0; i <= q->rear->arrival_time; i++)

    {
        // printf("pat=%d\n", p->arrival_time);
        // printf("priqat=%d\n", pri_que[i]->arrival_time);
        switch (type)
        {
        case 'a' /* arrival time */:
            if (p->arrival_time < q->pri_que[i]->arrival_time)

            {
                // printf("entered if condition %d \n", i);
                for (j = q->rear->arrival_time + 1; j > i; j--)

                {
                    // printf("entered \n");
                    q->pri_que[j] = q->pri_que[j - 1];
                }

                q->pri_que[i] = p;

                return;
            }
            break;
        case 'p' /* priority */:
            if (p->priority < q->pri_que[i]->priority)

            {
                // printf("entered if condition %d \n", i);
                for (j = q->rear->arrival_time + 1; j > i; j--)

                {
                    // printf("entered \n");
                    q->pri_que[j] = q->pri_que[j - 1];
                }

                q->pri_que[i] = p;

                return;
            }
            break;
        case 'r' /* remaining time */:
            if (p->remaining_time < q->pri_que[i]->remaining_time)

            {
                // printf("entered if condition %d \n", i);
                for (j = q->rear->arrival_time + 1; j > i; j--)

                {
                    // printf("entered \n");
                    q->pri_que[j] = q->pri_que[j - 1];
                }

                q->pri_que[i] = p;

                return;
            }
            break;
        case 'w' /* remaining time */:
            if (i==0)

            {
                // printf("entered if condition %d \n", i);
                for (j = q->rear->arrival_time + 1; j > i; j--)

                {
                    // printf("entered \n");
                    q->pri_que[j] = q->pri_que[j - 1];
                }

                q->pri_que[i] = p;

                return;
            }
            break;

        default:
            break;
        }
    }

    q->pri_que[i] = p;
}

/* Function to delete an element from queue */

void delete_by_priority(struct Queue *q, struct Process *p)

{

    int i;

    if ((q->front->arrival_time == -1) && (q->rear->arrival_time == -1))

    {

        printf("\nQueue is empty no elements to delete");

        return;
    }

    for (i = 0; i <= q->rear->arrival_time; i++)

    {

        if (p == q->pri_que[i])

        {

            for (; i < q->rear->arrival_time; i++)

            {

                q->pri_que[i] = q->pri_que[i + 1];
            }

            q->pri_que[i] = NULL;

            q->rear->arrival_time--;

            if (q->rear->arrival_time == -1)

                q->front->arrival_time = -1;

            return;
        }
    }

    //#TODO
    // printf("\n%d not found in queue to delete", data);
}

/* Function to display queue elements */

void display_queue(struct Queue *q)

{

    if ((q->front->arrival_time == -1) && (q->rear->arrival_time == -1))

    {

        printf("\nQueue is empty\n");

        return;
    }

    for (int i = q->front->arrival_time; i <= q->rear->arrival_time; i++)

    {
        printf("%d \n", i);
        printProcess(q->pri_que[i]);
    }

    q->front->arrival_time = 0;
}

struct msgProcessBuff
{
    long mtype;
    struct Process p;/*** when it was sent as a pointer the difference in memories between processes was a problem***/
};
struct msgIntBuff
{
    long mtype;
    int val;
};
void sendIntMesssage(key_t queue_key,int get_value,int pid,int value_to_send,struct msgIntBuff* message)
{
    queue_key = msgget(get_value, IPC_CREAT | 0644);
    // printf("queue key int value sender :%d \n",queue_key);
    if (queue_key == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    int send_val;
    message->mtype = pid % 10000; /* arbitrary value */
    message->val = value_to_send;
    send_val = msgsnd(queue_key, message, sizeof(message->val), !IPC_NOWAIT);

    if (send_val == -1)
    {
        perror("Errror in send");
    }
}
void sendProcessMesssage(key_t queue_key,int get_value,int pid,struct Process *process_to_send,struct msgProcessBuff * message)
{
    queue_key = msgget(get_value, IPC_CREAT | 0644);
    // printf("queue key value sender :%d \n",queue_key);
    if (queue_key == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    int send_val;
    message->mtype = pid % 10000; /* arbitrary value */
    message->p = *process_to_send;
    send_val = msgsnd(queue_key, message, sizeof(message->p), !IPC_NOWAIT);

    if (send_val == -1)
    {
        perror("Errror in send");
    }
}

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
        value_to_receive =  message->val;
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

    }
}
