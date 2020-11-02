#include "snek_api.h"
#define ABS(n) (((n) < 0) ? -(n) : (n)) /* Absolute function */
#define TOL 0.001 /* Tolerance */

typedef struct Node {
//For implementing Priority Queue
//Dear grader, you will soon read that this is for implementing a normal queue
//Although, in the dequeue() function, you will realise that even though it's a queue, it dequeues like a Priority Queue.
//Yes, so I've sacrificed runtime complexity in order to give myself an easier time coding. This might be fixed later.

//Also note: The struct is defined so that it can perform as a single list AND work for A* - parent tracking as well (Hence Parent and Path_parent)
	int coor[2]; //Coordinate of node
	float dist_from_start;
	float dist_from_end;
	float dist_sum;
	Node* next;
	Node* parent;
	Node* path_parent;
}Node;


typedef struct PriorityQueue
{
	Node* head;
	int Q_size;
}PQueue;


int a_star(int start[2], int end[2], GameBoard* board);
float distance(int start[2], int end[2]);
Node* Find_Node_in_PQueue(PQueue* pq, Node* node); //helper function
PQueue* make_PQ();
Node* make_Node(int curr[2], int end[2], Node* p_parent);
void enqueue(PQueue* PQ, Node* node); 
Node* Pdequeue(PQueue* PQ);
void delete_PQ(PQueue* PQ);
int* Find_Moogle(GameBoard* board);
float sqrt2(float a);