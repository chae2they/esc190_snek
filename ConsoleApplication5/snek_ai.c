#include "snek_api.h"
#include <string.h>
#include <time.h>
//#include <math.h>
#define LIST_ERROR -2


//ENQUEUE AND DEQUEUE REQUIRES A FIX BASED ON YOUR NEW NODE.PARENT ADDITION TO YOUR STRUCTURE

int a_star(int start[2], int end[2], GameBoard* board) 
//Receive starting coordinate (which is going to be snake->head)
//end coordinate, the coordinate of Moogle
{
	/*-----NOTE TO SELF-----*/
	//Clue at the moment: Is there a case where identical node can exist in both open and closed at the same time?
	//ALSO RECONSIDER RE-CODING ENQUEUE AND DEQUEUE
	//Another thing: Check the cases of how I enqueue into the list and see why I am checking the goddamn nodes in the weirdest way possible
	//Another sketchy place: When I am "updating" the node in pq_open, things might be acting up. 
	//I might 1. Recode enqueue and dequeue 2. Replace "updating" with just dequeuing and enqueuing.

	//CAREFUL ATTEMPT
	if (start[0] == end[0] && start[1] == end[1]) return 0;

	//PQueue* pq_open = make_PQ(); //List of Nodes to explore
	//PQueue* pq_closed = make_PQ(); //Nodes already explored
	PQueue pq_open;
	pq_open.head = NULL;
	PQueue pq_closed;
	pq_closed.head = NULL;
	Vector2 shortest_path_map[BOARD_SIZE][BOARD_SIZE];

	//Node* head = make_Node(start, start, end, NULL);
	Node head = Init_Node(start, start, end);
	//head.coor_x = start[0];
	//head.coor_y = start[1];
	//head.dist_from_start = 0; //We are at the start
	//head.dist_from_end = distance(head.coor_x, head.coor_y, end[0], end[1]);
	//head.dist_sum = head.dist_from_start + head.dist_from_end;
	//head.next = NULL;
	//head.parent = NULL;
	////head.shortest_path = NULL;

	enqueue(&pq_open, head);
	while (pq_open.head) //While priority queue is not empty (Non-zero)
	{
		Node curr;
		if (!Pdequeue(&pq_open, &curr)) return LIST_ERROR;
		enqueue(&pq_closed, curr);

		if ((curr.coor_x == end[0]) && (curr.coor_y == end[1]))
		{ //Target found; now to return direction
			//Vector2 start_coord = { start[0], start[1] };
			Vector2 curr_vec;
			Vector2 prev_vec = { curr.coor_x, curr.coor_y };
			curr_vec.coor_x = shortest_path_map[curr.coor_x][curr.coor_y].coor_x;
			curr_vec.coor_y = shortest_path_map[curr.coor_x][curr.coor_y].coor_y;
			//prev_vec = curr;
			while (!((curr_vec.coor_x == start[0]) && (curr_vec.coor_y == start[1])))
			//while (!((shortest_path_map[curr_vec.coor_x][curr_vec.coor_y].coor_x == start[0]) && (shortest_path_map[curr_vec.coor_x][curr_vec.coor_y].coor_y == start[1])))
			{
				//curr_vec.coor_x = shortest_path_map[curr_vec.coor_x][curr_vec.coor_y].coor_x;
				//curr_vec.coor_y = shortest_path_map[curr_vec.coor_x][curr_vec.coor_y].coor_y;
				prev_vec = curr_vec;
				curr_vec = shortest_path_map[curr_vec.coor_x][curr_vec.coor_y];
				
			}
			int x_dir = prev_vec.coor_x - start[0];
			int y_dir = prev_vec.coor_y - start[1];
			
			//Clean all allocated nodes
			Node* pq_clearer;
			pq_clearer = pq_open.head;
			while (pq_clearer)
			{
				Node* temp = pq_clearer;
				pq_clearer = pq_clearer->next;
				free(temp);
			}
			pq_clearer = pq_closed.head;
			while (pq_clearer)
			{
				Node* temp = pq_clearer;
				pq_clearer = pq_clearer->next;
				free(temp);
			}

			if ((x_dir == -1) && (y_dir == 0)) return 1; //Going left
			if ((x_dir == 1) && (y_dir == 0)) return 2; //Going right
			if ((x_dir == 0) && (y_dir == -1)) return 3; //Going up
			if ((x_dir == 0) && (y_dir == 1)) return 4; //Going down

		}
		
		//The following are adjacent, reachable nodes's coordinate that we will be enqueueing if possible.
		//For each, 
		//1. Check if it's a square that is reachable (ie. Snake is not occupying, occupancy == 0)
		//2. Check if it's already explored (ie. It's in closed list)
		//3. If it's neither 1 or 2
		//4. Check if it's in open
		//5. If it's in open, see if the current node offers a shorter path. If not, ignore
		//6. If it isn't in open, enqueue.
		int up[2] = {curr.coor_x, curr.coor_y - 1};
		//Node* node_above = make_Node(up, start, end, curr);
		Node node_above = Init_Node(up, start, end);
		//Node node_in_queue;
		int found = Find_Node_in_PQueue(&pq_closed, node_above, NULL);
		if ((0 <= up[0]) && (up[0] < BOARD_SIZE)
			&& (0 <= up[1]) && (up[1] < BOARD_SIZE)
			&& (board->occupancy[up[1]][up[0]] == 0)
			&& (!found))
		{	//If snake can travel in that direction and Node hasn't been explored yet

			Node* node_in_queue_ptr; 
			if (Find_Node_in_PQueue(&pq_open, node_above, &node_in_queue_ptr)) //If node is in open already
			{   
				if (node_in_queue_ptr->dist_sum > node_above.dist_sum) //If it offers a shorter distance
				{
					node_in_queue_ptr->dist_from_start = node_above.dist_from_start;
					node_in_queue_ptr->dist_from_end = node_above.dist_from_end;
					node_in_queue_ptr->dist_sum = node_above.dist_sum;
					
					Vector2 temp = {curr.coor_x, curr.coor_y};
					shortest_path_map[node_above.coor_x][node_above.coor_y] = temp;
					//node_in_queue->path_parent = curr;
					//free(node_above);
				}
				//else free(node_above); //If not move on
			}
			else
			{
				enqueue(&pq_open, node_above);
				Vector2 temp = {curr.coor_x, curr.coor_y};
				shortest_path_map[node_above.coor_x][node_above.coor_y] = temp;
			}
		}

		//else free(node_above); //We don't need this; since we can't access that node/it's in closed.


		int down[2] = {curr.coor_x, curr.coor_y + 1};
		//Node* node_below = make_Node(down, start, end, curr);
		Node node_below = Init_Node(down, start, end);
		found = Find_Node_in_PQueue(&pq_closed, node_below, NULL);
		if ((0 <= down[0]) && (down[0] < BOARD_SIZE)
			&& (0 <= down[1]) && (down[1] < BOARD_SIZE)
			&&(board->occupancy[down[1]][down[0]] == 0)
			&& (!found))
		{	//If snake can travel in that direction and Node hasn't been explored yet

			Node* node_in_queue_ptr;
			if (Find_Node_in_PQueue(&pq_open, node_below, &node_in_queue_ptr)) //If node is in open already
			{   
				if (node_in_queue_ptr->dist_sum > node_below.dist_sum)
				{
					node_in_queue_ptr->dist_from_start = node_below.dist_from_start;
					node_in_queue_ptr->dist_from_end = node_below.dist_from_end;
					node_in_queue_ptr->dist_sum = node_below.dist_sum;
					//node_in_queue->path_parent = curr;
					//free(node_below);
					Vector2 temp = { curr.coor_x, curr.coor_y };
					shortest_path_map[node_below.coor_x][node_below.coor_y] = temp;
				}
				//else free(node_below);
			}
			else
			{
				enqueue(&pq_open, node_below);
				Vector2 temp = { curr.coor_x, curr.coor_y };
				shortest_path_map[node_below.coor_x][node_below.coor_y] = temp;
			}
		}
		//else free(node_below);


		int left[2] = {curr.coor_x - 1, curr.coor_y};
		//Node* node_left = make_Node(left, start, end, curr);
		Node node_left = Init_Node(left, start, end);
		found = Find_Node_in_PQueue(&pq_closed, node_left, NULL);
		if ((0 <= left[0]) && (left[0] < BOARD_SIZE)
			&& (0 <= left[1]) && (left[1] < BOARD_SIZE)
			&&(board->occupancy[left[1]][left[0]] == 0)
			&& (!found))
		{	//If snake can travel in that direction and Node hasn't been explored yet

			Node* node_in_queue_ptr;
			if (Find_Node_in_PQueue(&pq_open, node_left, &node_in_queue_ptr)) //If node is in open already
			{   
				if (node_in_queue_ptr->dist_sum > node_left.dist_sum)
				{
					node_in_queue_ptr->dist_from_start = node_left.dist_from_start;
					node_in_queue_ptr->dist_from_end = node_left.dist_from_end;
					node_in_queue_ptr->dist_sum = node_left.dist_sum;
					//node_in_queue->path_parent = curr;
					//free(node_left);
					Vector2 temp = { curr.coor_x, curr.coor_y };
					shortest_path_map[node_left.coor_x][node_left.coor_y] = temp;
				}
				//else free(node_left);
			}
			else
			{
				enqueue(&pq_open, node_left);
				Vector2 temp = { curr.coor_x, curr.coor_y };
				shortest_path_map[node_left.coor_x][node_left.coor_y] = temp;
			}
		}
		//else free(node_left);

		int right[2] = {curr.coor_x + 1, curr.coor_y};
		//Node* node_right = make_Node(right, start, end, curr);
		Node node_right = Init_Node(right, start, end);
		found = Find_Node_in_PQueue(&pq_closed, node_right, NULL);
		if ((0 <= right[0]) && (right[0] < BOARD_SIZE)
			&& (0 <= right[1]) && (right[1] < BOARD_SIZE)
			&& (board->occupancy[right[1]][right[0]] == 0)
			&& (!found))
		{	//If snake can travel in that direction and Node hasn't been explored yet

			Node* node_in_queue_ptr; 
			if (Find_Node_in_PQueue(&pq_open, node_right, &node_in_queue_ptr)) //If node is in open already
			{  
				if (node_in_queue_ptr->dist_sum > node_right.dist_sum)
				{
					node_in_queue_ptr->dist_from_start = node_right.dist_from_start;
					node_in_queue_ptr->dist_from_end = node_right.dist_from_end;
					node_in_queue_ptr->dist_sum = node_right.dist_sum;
					//node_in_queue->path_parent = curr;
					//free(node_right);
					Vector2 temp = { curr.coor_x, curr.coor_y };
					shortest_path_map[node_right.coor_x][node_right.coor_y] = temp;
				}
				//else free(node_right);
			}
			else
			{
				enqueue(&pq_open, node_right);
				Vector2 temp = { curr.coor_x, curr.coor_y };
				shortest_path_map[node_right.coor_x][node_right.coor_y] = temp;
			}
		}
		//else free(node_right);

	}
	Node* pq_clearer;
	pq_clearer = pq_open.head;
	while (pq_clearer)
	{
		Node* temp = pq_clearer;
		pq_clearer = pq_clearer->next;
		free(temp);
	}
	pq_clearer = pq_closed.head;
	while (pq_clearer)
	{
		Node* temp = pq_clearer;
		pq_clearer = pq_clearer->next;
		free(temp);
	}
	return 0; //No path was found
}

int Reach_Moogle_Check(int start[2], int end[2], GameBoard* board)
{	//This function will simply check if the snake can reach its tail after the snake reaches the apple
	//This is accomplished by making a copy of the board, and running it before actually making the move.
	//Reaching this function means end[2] is NOT EMPTY and there IS an apple

	//Copies are created within function to make sure that the original isn't broken

	//Current problem: editing board_copy will also make changes to the board. 
	int head_coord[2];
	head_coord[0] = start[0];
	head_coord[1] = start[1];
	int apple_coord[2];
	apple_coord[0] = end[0];
	apple_coord[1] = end[1];
	GameBoard* board_copy;
	board_copy = copy_board(board);

	int direction = DIR_INIT;
	int axis = AXIS_INIT;

	head_coord[0] = board_copy->snek->head->coord[0];
	head_coord[1] = board_copy->snek->head->coord[1];

	while (!(head_coord[0] == apple_coord[0] && head_coord[1] == apple_coord[1])) //Until the head reaches the apple
	{		
		direction = a_star(head_coord, apple_coord, board_copy);
		switch (direction)
		{
		case 1: axis = AXIS_X; direction = LEFT; break;
		case 2: axis = AXIS_X; direction = RIGHT; break;
		case 3: axis = AXIS_Y; direction = UP; break;
		case 4: axis = AXIS_Y; direction = DOWN; break;
		}
		
		if (direction == 0)
		{
			clear_board(board_copy);
			return 0; //No path to apple. Abort.
		}
		//advance_frame(axis, direction, board_copy);
		int fail = advance_frame_copy(axis, direction, board_copy);//If we get a fail == 0 here, that means the snake has died in simulation.
		if (fail == 0) return 1; //Just return a 1 and let it run until the end for most points

		head_coord[0] = board_copy->snek->head->coord[0]; //Update head last because once it reaches the apple
		head_coord[1] = board_copy->snek->head->coord[1]; //We will exit out of the while loop without further action
														 //Also we are not interested in printing the board
														 //So it doesn't matter if we update this last
	}
	//If we are out of the loop, the head is at the apple. Now to check if it can reach the tail
	if (board_copy->snek->length <= 4)
	{	//By the nature of the game snake, if the length is equal or less than 4, then snake can always reach tail
		clear_board(board_copy);
		return 1;
	}
	//If snake is 5 or greater in length
	//Check if you can reach one of the 4 squares around the tail
	//This is because you can't reach the tail itself, 
	//Due to A* logic (tail cannot be explored because of occupancy,
	//so it's not possible to get an exit condition with tail).

	//tail coordinate
	int tail_coord[2];
	tail_coord[0] = board_copy->snek->tail->coord[0];
	tail_coord[1] = board_copy->snek->tail->coord[1];

	//for testing if reach tail
	int reach_tail = 0;

	//If ANY of the nodes below returns a direction that is NONZERO, there IS a path to the tail
	//Also, if the head happens to consume the apple RIGHT BESIDE THE TAIL
	//Return 0; it's a safety measure so that the head can always leave 1 space between 
	//The head and the tail

	int up[2] = { tail_coord[0], tail_coord[1] - 1 };
	if (board_copy->occupancy[up[1]][up[0]] == 0) //If there is no snake body there
	{
		if (tail_coord[0] == up[0] && tail_coord[1] == up[1]) //If it's right beside a tail, don't go
		{
			clear_board(board_copy);
			return 0;
		}
		direction = a_star(head_coord, up, board_copy);
		if (direction != 0) reach_tail = 1;
	}

	int down[2] = { tail_coord[0], tail_coord[1] + 1 };
	if (board_copy->occupancy[down[1]][down[0]] == 0)
	{
		if (tail_coord[0] == down[0] && tail_coord[1] == down[1])
		{
			clear_board(board_copy);
			return 0;
		}
		direction = a_star(head_coord, down, board_copy);
		if (direction != 0) reach_tail = 1;
	}

	int right[2] = { tail_coord[0] + 1, tail_coord[1] };
	if (board_copy->occupancy[right[1]][right[0]] == 0)
	{
		if (tail_coord[0] == right[0] && tail_coord[1] == right[1])
		{
			clear_board(board_copy);
			return 0;
		}
		direction = a_star(head_coord, right, board_copy);
		if (direction != 0) reach_tail = 1;
	}

	int left[2] = { tail_coord[0] - 1, tail_coord[1] };
	if (board_copy->occupancy[left[1]][left[0]])
	{
		if (tail_coord[0] == left[0] && tail_coord[1] == left[1])
		{
			clear_board(board_copy);
			return 0;
		}
		direction = a_star(head_coord, left, board_copy);
		if (direction != 0) reach_tail = 1;
	}

	//direction = a_star(head_coord, tail_coord, board_copy); //Returns 0 here?
	if (reach_tail) //There is path to tail
	{
		clear_board(board_copy);
		return 1;
	}
	else //There is no path to tail
	{
		clear_board(board_copy);
		return 0;
	}
}

int advance_frame_copy(int axis, int direction, GameBoard* gameBoard) {
	if (is_failure_state(axis, direction, gameBoard)) {
		return 0;
	}
	else {
		// update the occupancy grid and the snake coordinates
		int head_x, head_y;
		// figure out where the head should now be
		if (axis == AXIS_X) {
			head_x = gameBoard->snek->head->coord[x] + direction;
			head_y = gameBoard->snek->head->coord[y];
		}
		else if (axis == AXIS_Y) {
			head_x = gameBoard->snek->head->coord[x];
			head_y = gameBoard->snek->head->coord[y] + direction;
		}

		int tail_x = gameBoard->snek->tail->coord[x];
		int tail_y = gameBoard->snek->tail->coord[y];

		// update the occupancy grid for the head
		gameBoard->occupancy[head_y][head_x] = 1;

		if (gameBoard->snek->length > 1) { //make new head
			SnekBlock* newBlock = (SnekBlock*)malloc(sizeof(SnekBlock));
			newBlock->coord[x] = gameBoard->snek->head->coord[x];
			newBlock->coord[y] = gameBoard->snek->head->coord[y];
			newBlock->next = gameBoard->snek->head->next;

			gameBoard->snek->head->coord[x] = head_x;
			gameBoard->snek->head->coord[y] = head_y;
			gameBoard->snek->head->next = newBlock;

			if (gameBoard->cell_value[head_y][head_x] > 0) {  //eat something
				//eat_moogle(gameBoard, head_x, head_y);
			}
			else { //did not eat
			 //delete the tail
				gameBoard->occupancy[tail_y][tail_x] = 0;
				SnekBlock* currBlock = gameBoard->snek->head;
				while (currBlock->next != gameBoard->snek->tail) {
					currBlock = currBlock->next;
				} //currBlock->next points to tail

				currBlock->next = NULL;
				free(gameBoard->snek->tail);
				gameBoard->snek->tail = currBlock;
			}

		}
		else if ((gameBoard->snek->length == 1) && gameBoard->cell_value[head_y][head_x] == 0) { // change both head and tail coords, head is tail
			gameBoard->occupancy[tail_y][tail_x] = 0;
			gameBoard->snek->head->coord[x] = head_x;
			gameBoard->snek->head->coord[y] = head_y;
			gameBoard->snek->tail->coord[x] = head_x;
			gameBoard->snek->tail->coord[y] = head_y;

		}
		else { //snake is length 1 and eats something
			//eat_moogle(gameBoard, head_x, head_y);
			gameBoard->snek->head->coord[x] = head_x;
			gameBoard->snek->head->coord[y] = head_y;
		}
	}
}

GameBoard* copy_board(GameBoard* board)
{
	GameBoard* copied_ret_board = malloc(sizeof(GameBoard));
	//Copying board information
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		for (int j = 0; j < BOARD_SIZE; ++j)
		{
			copied_ret_board->cell_value[i][j] = board->cell_value[i][j];
			copied_ret_board->occupancy[i][j] = board->occupancy[i][j];
		}
	}

	//Copy of snake we will work with later
	Snek* snake_copy = malloc(sizeof(Snek));
	snake_copy->length = board->snek->length;
	snake_copy->head = NULL;
	snake_copy->tail = NULL;

	//Make head to initiate
	SnekBlock* curr = board->snek->head;
	SnekBlock* copied_head = malloc(sizeof(SnekBlock));
	copied_head->coord[0] = curr->coord[0];
	copied_head->coord[1] = curr->coord[1];
	copied_head->next = NULL;
	snake_copy->head = copied_head;

	curr = curr->next;

	while (curr) //While there's a snekblock in the original snake
	{
		//Make copied snake_block
		SnekBlock* copied_snekblock = malloc(sizeof(SnekBlock));
		copied_snekblock->coord[0] = curr->coord[0];
		copied_snekblock->coord[1] = curr->coord[1];
		copied_snekblock->next = NULL;

		//copied_curr is for iterating through the copied snekblock list to find an opening
		SnekBlock* copied_curr = snake_copy->head;
		while (copied_curr->next != NULL) //Ends if the next node is going to be a NULL node
		{
			copied_curr = copied_curr->next; 
		}
		copied_curr->next = copied_snekblock;
		curr = curr->next; //At tail, curr->next == NULL and escape while loop.
	}
	//Exiting this loop means all of original snake's snekblock has been copied into new snake

	//Finding tail
	SnekBlock* copied_curr = snake_copy->head;
	while (copied_curr->next != NULL) //If copied_curr is tail, then 
									   //(copied_curr->next == NULL) is true, hence loop quits
	{
		copied_curr = copied_curr->next;
	}
	snake_copy->tail = copied_curr; //Put the pointer of the tail, at snake_copy->tail

	copied_ret_board->snek = snake_copy;

	return copied_ret_board;
}

void clear_board(GameBoard* board)
{
	//Clear snake
	SnekBlock* curr = board->snek->head;
	while (curr)
	{
		SnekBlock* temp = curr;
		curr = curr->next;
		free(temp);
	}

	free(board->snek);
	free(board);
}

float distance(int start_x, int start_y, int end_x, int end_y)
{
	//This is for assisting with A* 
	float dist;
	dist = sqrt2((start_x - end_x)*(start_x - end_x) + (start_y - end_y)*(start_y - end_y));
	return dist;
}

int Find_Node_in_PQueue(PQueue* pq, Node node, Node** ret_node_ptr) 
//Current problem of infinite loop is here: 
//For some reason, I am appending infinite amount of objects into this list
{
	Node* curr = pq->head;
	while (curr)
	{
		if ((curr->coor_x == node.coor_x) && (curr->coor_y == node.coor_y))  //If you find a Node with the same coordinate, return the Node
		{
			if (ret_node_ptr) *ret_node_ptr = curr;
			return 1;
		}
		curr = curr->next;
	}
	return 0;
}


Node Init_Node(int curr[2], int start[2], int end[2])
{
	Node node;
	node.coor_x = curr[0];
	node.coor_y = curr[1];
	node.dist_from_start = distance(curr[0], curr[1], start[0], start[1]);
	node.dist_from_end = distance(curr[0], curr[1], end[0], end[1]);
	node.dist_sum = node.dist_from_start + node.dist_from_end;
	node.next = NULL;
	node.parent = NULL;

	return node;
}

void enqueue(PQueue* pq, Node node)
{
	Node* new_node = malloc(sizeof(Node));
	*new_node = node;
	new_node->parent = NULL;
	new_node->next = NULL;

	Node* curr_node = pq->head;
	if (curr_node == NULL) //Just for the first element in PQ 
		pq->head = new_node;
	else
	{
		while (curr_node)
		{
			if (curr_node->next == NULL) //When there's an empty node space,
			{
				curr_node->next = new_node;
				new_node->parent = curr_node;
				break;
			}
			curr_node = curr_node->next;
		}
	}
}

int Pdequeue(PQueue* pq, Node* ret_node) 
{
	Node* temp = pq->head; //Temporary node for finding which node I have to return
	Node* curr_node = pq->head;
	//Node* temp_parent = NULL;
	if (pq->head == NULL) return 0;
	while (curr_node)
	{
		if (curr_node->dist_sum < temp->dist_sum) //If we find a new node with smaller dist_sum, replace
		{
			temp = curr_node;
		}
		if ((curr_node->dist_sum == temp->dist_sum)   //If the dist_sum are same, but dist_from_end is smaller, replace.
			&& (curr_node->dist_from_end < temp->dist_from_end))
		{
			temp = curr_node;
		}
		curr_node = curr_node->next;
	}
	Node final_return = *temp; //Address of the Node we're trying to return
	if (temp->parent == NULL)
	{ //If it's the head
		pq->head = temp->next;
		if (temp->next) temp->next->parent = NULL;
	}
	else
	{
		temp->parent->next = temp->next;
		if (temp->next)  temp->next->parent = temp->parent;
	}//Failing Null check in the case where we pull from the head of the list
	final_return.next = NULL; //Cut off tail
	
							  
	free(temp);
	//return final_return;
	*ret_node = final_return;
	return 1;
}

int Find_Moogle(GameBoard* board, int ret_coor[])
{
	int Moogle_Coor[2];
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		for (int j = 0; j < BOARD_SIZE; ++j)
		{
			if (board->cell_value[j][i] != 0) //If there is a Moogle
			{
				//Moogle_Coor[0] = i;
				//Moogle_Coor[1] = j;
				ret_coor[0] = i;
				ret_coor[1] = j;
				return 1;
			}
		}
	}
	return 0;
}

float sqrt2(float square)
{
	float root = square / 3, last, diff = 1;
	if (square <= 0) return 0;
	do {
		last = root;
		root = (root + square / root) / 2;
		diff = root - last;
	} while (diff > MINDIFF || diff < -MINDIFF);
	return root;
}