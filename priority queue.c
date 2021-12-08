/* 

     * C Program to Implement Priority Queue to Add and Delete Elements

     */

#include <stdio.h>

#include <stdlib.h>

#include <headers.h>

#define MAX 500

void insert_by_priority(struct Process *);

void delete_by_priority(struct Process *);

void create();

void check(struct Process *);

void display_pqueue();

struct Process *pri_que[MAX];

struct Process *front, *rear;

void main()

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
}

/* Function to create an empty priority queue */

void create()

{
    struct Process p1;
    struct Process *pptr = &p1;
    pptr->arrival_time = -1;
    front = pptr;
    rear = pptr;
}

/* Function to insert value into priority queue */

void insert_by_priority(struct Process *p)

{

    if (rear->arrival_time >= MAX - 1)

    {

        printf("\nQueue overflow no more elements can be inserted");

        return;
    }

    if ((front->arrival_time == -1) && (rear->arrival_time == -1))

    {

        front->arrival_time++;

        rear->arrival_time++;

        pri_que[rear->arrival_time] = p;

        return;
    }

    else

        check(p);

    rear->arrival_time++;
}

/* Function to check priority and place element */

void check(struct Process *p)

{

    int i, j;

    for (i = 0; i <= rear->arrival_time; i++)

    {

        if (p->arrival_time < pri_que[i]->arrival_time)

        {

            for (j = rear->arrival_time + 1; j > i; j--)

            {

                pri_que[j] = pri_que[j - 1];
            }

            pri_que[i] = p;

            return;
        }
    }

    pri_que[i] = p;
}

/* Function to delete an element from queue */

void delete_by_priority(struct Process *p)

{

    int i;

    if ((front->arrival_time == -1) && (rear->arrival_time == -1))

    {

        printf("\nQueue is empty no elements to delete");

        return;
    }

    for (i = 0; i <= rear->arrival_time; i++)

    {

        if (p == pri_que[i])

        {

            for (; i < rear->arrival_time; i++)

            {

                pri_que[i] = pri_que[i + 1];
            }

            pri_que[i] = NULL;

            rear->arrival_time--;

            if (rear->arrival_time == -1)

                front->arrival_time = -1;

            return;
        }
    }

    //#TODO
    // printf("\n%d not found in queue to delete", data);
}

/* Function to display queue elements */

void display_pqueue()

{

    if ((front->arrival_time == -1) && (rear->arrival_time == -1))

    {

        printf("\nQueue is empty");

        return;
    }

    for (; front->arrival_time <= rear->arrival_time; front->arrival_time++)

    {

        printf(" %d ", pri_que[front->arrival_time]);
    }

    front->arrival_time = 0;
}