#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DICE_ROLLS 6
#define SIZE 73

/* struct Vertex is used to represent every cell on the board */
typedef struct Vertex{
	int visited;
	int key;
	int edgeCount;
	struct Vertex* edges [DICE_ROLLS];
} Vertex;

/* struct QueueItem is used to represent every element in the Queue */
typedef struct QueueItem{
	int vertex;	// represents current cell
	int dice;	// represents dice rolled to reach cell vertex
	int distance;	// number of dice rolls so far
} QueueItem;

/* Queue data structure, used as temporary memory resource */
typedef struct Queue{
	QueueItem array[SIZE];
	int front;
	int rear;
	int size;
} Queue;

/* Utility functions for Queue data structure */ 

Queue* createQueue();			// create new Queue
void enqueue(Queue*, QueueItem);	// push element to queue
QueueItem dequeue(Queue*);		// pop element from queue
int isEmpty(Queue*);			// check if queue is empty
int isFull(Queue*);			// check if queue is full
QueueItem front(Queue*);		// read first element (without popping)
QueueItem rear(Queue*);			// read last element (without popping)
void printQueue(Queue*);		// print elements in the queue 

/* Functions for the main program */

Vertex* createVertex (int);			// create new cell
void createEdge (Vertex*[],int, int);		// create new edge (link between two cells)
void createBoard (Vertex*[], int);		// creates a new board (list of connected cells)
void createChutesAndLadders(int[], int);	// fills an array with chute and ladder mappings
Queue* BFS (Vertex*[], int[], int);		// path finding algorithm (Breadth First Search concept)

/* Main	*/
int main (void)
{
	Vertex* board[SIZE];			// data structure representing the board 
	int jumps[SIZE];			// array to store chute and ladder mappings
	double start, end, cpu_time;

	createBoard(board, SIZE);		// initialize board with cells(vertices) and links(edges)
	createChutesAndLadders(jumps, SIZE);	// initialize array with chute and ladder mappings

	int problem = 1;			// used to set problem to be solved

	/*
		set problem as 1 for dice roll seq. for least number of steps
		set problem as 2 for dice roll seq. for avoiding all chutes
		set problem as 3 for dice roll seq. for avoiding all chutes and ladders
	*/

	start = (double) clock()/CLOCKS_PER_SEC;
	Queue* sequence = BFS(board, jumps, problem);	// get dice roll sequence for current problem
	end = (double) clock()/CLOCKS_PER_SEC;
	cpu_time = end - start;
	printf("%f\n",cpu_time);
	printQueue(sequence);				// print dice roll sequence

	
	return 0;
}

void createChutesAndLadders(int jumps[], int length)
{
	for(int i =0; i < length; i++)
		jumps[i] = -1;

	/* Adding Ladders */
	jumps[2] = 20;
	jumps[4] = 45;
	jumps[22] = 42;
	jumps[26] = 46;
	jumps[51] = 70;

	/* Adding Chutes */
	jumps[18] = 0;
	jumps[55] = 7;
	jumps[48] = 19;
	jumps[35] = 27;
	jumps[64] = 44;
	jumps[69] = 50;
}

Vertex* createVertex (int key)
{
	Vertex* vertex = malloc(sizeof(Vertex));
	vertex->visited = 0;
	vertex->edgeCount = 0;
	vertex->key = key;
	return vertex;
}

void createEdge (Vertex* board[],int key1, int key2)
{
	Vertex* v1 = board[key1];
	Vertex* v2 = board[key2];
	if ((v1->edgeCount+1) < DICE_ROLLS)
		v1->edges[v1->edgeCount++] = v2;
}

void createBoard (Vertex* board[], int length)
{
	for( int i = 0; i < length; i++)
		board[i] = createVertex(i+1);

	for (int i = 0; i < length; i++)
		for(int j = 1; j <= DICE_ROLLS && ((i+j) < length); j++)
			createEdge(board, i, i+j);
}

Queue* BFS (Vertex* board[], int jumps[], int problem)
{	
	Queue* sequence = createQueue();
	Queue* sequenceCopy = createQueue();	/* copy needed as sequence Queue is exhaustive */
    
	/* initializing Queue */
	board[0]->visited = 1;	
	QueueItem qi = {0, 0}; 
 
	enqueue(sequence, qi);
	enqueue(sequenceCopy, qi);
	
	QueueItem qe;  
	while (!isEmpty(sequence))
	{
		qe = dequeue(sequence);
		int vertex = qe.vertex; 

		if (vertex == SIZE-1)
		    break;
		
		QueueItem max;		
		for (int j=1; j<=6 && vertex+j<SIZE; j++)	// j represents dice roll values from 1 to 6
			if (board[vertex+j]->visited == 0)
			{
				board[vertex+j]->visited = 1;

				QueueItem a;
				a.vertex = vertex+j;
				a.dice = j;
				a.distance = qe.distance + 1;				

				/*	logic 	*/

				if(problem == 1)
				{
					 if (jumps[vertex+j] != -1)	// true for chutes and ladders
					    a.vertex = jumps[vertex+j];
				}
				else if (problem == 2)
				{
					if (jumps[vertex+j] != -1)	// true for chutes and ladders
					{	
						if(jumps[vertex+j] > (vertex+j))	// in case of ladder
					    		a.vertex = jumps[vertex+j];
						else	// in case of chute
							continue;	// skip that dice roll (j)
					}
				}
				else if (problem == 3)
				{
					if (jumps[vertex+j] != -1)	// true for chutes and ladders
					    continue;			//skip that dice roll (j)
				}
				else
				{
					printf("Error\n");
					return sequence;
				}
				
				if(a.vertex >= max.vertex)	//reason for shortest path
					max = a;
			}
		enqueue(sequence, max); 
		enqueue(sequenceCopy, max);		
   	 }
    return sequenceCopy;
}

Queue* createQueue()
{
    Queue* queue = (Queue*) malloc(sizeof(Queue));
    queue->front = queue->size = 0; 
    queue->rear = SIZE - 1; 
    return queue;
}
 
int isFull(Queue* queue)
{  return (queue->size == SIZE);  }

int isEmpty(Queue* queue)
{  return (queue->size == 0); }

void enqueue(Queue* queue, QueueItem item)
{
	
	if (isFull(queue))
		return;
	queue->rear = (queue->rear + 1)%SIZE;
	queue->array[queue->rear] = item;
	queue->size = queue->size + 1;
}

QueueItem dequeue(Queue* queue)
{
	QueueItem nil={-1,-1,-1};
	if (isEmpty(queue))
		return nil;
	QueueItem item = queue->array[queue->front];
	queue->front = (queue->front + 1)%SIZE;
	queue->size = queue->size - 1;
	return item;
}

QueueItem front(Queue* queue)
{
	QueueItem nil={-1,-1,-1};
	if (isEmpty(queue))
		return nil;
	return queue->array[queue->front];
}

QueueItem rear(Queue* queue)
{
	QueueItem nil={-1,-1,-1};
	if (isEmpty(queue))
		return nil;
	return queue->array[queue->rear];
}

void printQueue (Queue* queue)
{
	QueueItem qi = dequeue(queue);
	printf("position : %2d, ", qi.vertex+1);
	while(isEmpty(queue) == 0)
	{
		QueueItem qi = dequeue(queue);
		printf("dice: %2d, Number of rolls : %2d\n",qi.dice , qi.distance);
		printf("position : %2d, ", qi.vertex+1);
	}
	printf("\n");
}
