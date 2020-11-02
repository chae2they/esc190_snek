/**
AUTHOR: SAIMA ALI
LATEST WORKING VERSION 
FEBRUARY 2ND, 2020
ESC190H1S PROJECT
SNAKE API
 **/

#include <stdlib.h>
#include <stdio.h>

#define CYCLE_ALLOWANCE 1.5
#define BOARD_SIZE 10

#define LIFE_SCORE 1 //score awarded for simply staying alive one frame

#define AXIS_X -1
#define AXIS_Y 1

#define UP -1
#define DOWN 1
#define LEFT -1
#define RIGHT 1

#define AXIS_INIT AXIS_Y
#define DIR_INIT DOWN

#define x 0
#define y 1

#define MOOGLE_POINT 20
#define HARRY_MULTIPLIER 3

//Addition for sqrt2
#define ABS(n) (((n) < 0) ? -(n) : (n)) /* Absolute function */
#define TOL 0.001 /* Tolerance */

#define MINDIFF 2.25e-308  
//Addition for sqrt2

int CURR_FRAME;
int SCORE;
int MOOGLE_FLAG;

typedef struct SnekBlock{
	int coord[2];
	struct SnekBlock* next;
} SnekBlock;

typedef struct Snek{
	struct SnekBlock* head;
	struct SnekBlock* tail;
	int length;
} Snek;

typedef struct GameBoard {
	int cell_value[BOARD_SIZE][BOARD_SIZE];
	int occupancy[BOARD_SIZE][BOARD_SIZE];
	struct Snek* snek;
} GameBoard;



//ADDITION FOR AI
typedef struct Node {
	//For implementing Priority Queue
	//Dear grader, you will soon read that this is for implementing a normal queue
	//Although, in the dequeue() function, you will realise that even though it's a queue, it dequeues like a Priority Queue.
	//Yes, so I've sacrificed runtime complexity in order to give myself an easier time coding. This might be fixed later.

	//Also note: The struct is defined so that it can perform as a single list AND work for A* - parent tracking as well (Hence Parent and Path_parent)
	int coor_x; //Coordinate of node
	int coor_y;
	float dist_from_start;
	float dist_from_end;
	float dist_sum;
	struct Node* next;
	struct Node* parent;
	//struct Node* shortest_path;
}Node;

typedef struct Vector2
{	//This struct was created in order to keep track of the path to be taken by the snake
	//So each Vector2 struct will function as an indictator to where the current node came from
	//This will be used with a Vector2 arr[x][y] array that finds and keeps track of every node's path parent
	//Ex. During A* search, if after (0, 1), the snake searches (1, 1), 
	//then at Vector2 arr[1][1], you will find Vector2 struct (0, 1) to indicate that (0, 1)
	//Is the path parent of (1,1)
	int coor_x;
	int coor_y;
}Vector2;

typedef struct PriorityQueue
{
	struct Node* head;
}PQueue;

int a_star(int start[2], int end[2], GameBoard* board);
int Reach_Moogle_Check(int start[2], int end[2], GameBoard* board);
int advance_frame_copy(int axis, int direction, GameBoard* gameBoard);
GameBoard* copy_board(GameBoard* board);
void clear_board(GameBoard* board);
float distance(int start_x, int start_y, int end_x, int end_y);
int Find_Node_in_PQueue(PQueue* pq, Node node, Node** ret_node_ptr);
Node Init_Node(int curr[2], int start[2], int end[2]);
void enqueue(PQueue* pq, Node node);
int Pdequeue(PQueue* pq, Node* ret_node);
int Find_Moogle(GameBoard* board, int ret_coor[]);
float sqrt2(float a);
//ADDITION FOR AI

GameBoard *init_board();
Snek *init_snek(int a, int b);
int hits_edge(int axis, int direction,  GameBoard *gameBoard);
int hits_self(int axis, int direction,  GameBoard *gameBoard);
int is_failure_state(int axis, int direction,  GameBoard *gameBoard);
int advance_frame(int axis, int direction,  GameBoard *gameBoard);
void end_game(GameBoard **board);
void show_board(GameBoard* gameBoard);
int get_score();

