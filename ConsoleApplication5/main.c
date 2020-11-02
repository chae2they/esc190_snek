#include "snek_api.h"
//#include "snek_ai.h"
//#include <unistd.h>

void play_game() {
	printf("starting\n");
	//board initialized, struct has pointer to snek

	GameBoard* board = init_board();
	show_board(board);

	int axis = AXIS_INIT;
	int direction = DIR_INIT;
	
	int play_on = 1; //Simply 1 or 0. In failure state, this becomes 0 and game is over.
	int coord[2]; //In current algorithm, this is for keep track of where the head
	
	while (play_on){
		coord[x] = board->snek->head->coord[x];
		coord[y] = board->snek->head->coord[y];

		int moogle_coor[2];
		if(Find_Moogle(board, moogle_coor) && Reach_Moogle_Check(coord, moogle_coor, board))
			//If Find_Moogle() returns 0, reach_moogle_check is not even checked)
		{
			direction = a_star(coord, moogle_coor, board);
			switch (direction)
			{
			case 1: axis = AXIS_X; direction = LEFT; break;
			case 2: axis = AXIS_X; direction = RIGHT; break;
			case 3: axis = AXIS_Y; direction = UP; break; 
			case 4: axis = AXIS_Y; direction = DOWN; break;
			}
		}
		else if(board->snek->length <= 4)
		{   //If no apple in sight, or you can't consume the apple safely, but your snake isn't long enough:
			//The program would be created so that the tail is always in sight
			
			unsigned short go_x = (axis == AXIS_Y && direction == 1 && coord[y] == (BOARD_SIZE - 1)) || (axis == AXIS_Y && direction == -1 && coord[y] == 0);
			unsigned short go_y = (axis == AXIS_X && direction == 1 && coord[x] == (BOARD_SIZE - 1)) || (axis == AXIS_X && direction == -1 && coord[x] == 0);
			if (go_x) {
				// going to hit the bottom or top
				// go right or left
				axis = AXIS_X;
				if (coord[x] < (int)(BOARD_SIZE / 2)) {
					direction = RIGHT;
				}
				else {
					direction = LEFT;
				}
			}
			else if (go_y) {
				axis = AXIS_Y;
				if (coord[y] < (int)(BOARD_SIZE / 2)) {
					direction = DOWN;
				}
				else {
					direction = UP;
				}
			}
		}
		else
			//This is finally the case where the snake is long enough to kill itself.
		{ 
			int tail_coord[2];
			tail_coord[0] = board->snek->tail->coord[0];
			tail_coord[1] = board->snek->tail->coord[1];
			//board->occupancy[tail_coord[1]][tail_coord[0]] = 0; //Temporary; just for A_star to work
			//direction = a_star(coord, tail_coord, board);
			//board->occupancy[tail_coord[1]][tail_coord[0]] = 1; //Change it back before updating anything

			//switch (direction)
			//{
			//case 0: axis = AXIS_INIT; direction = DIR_INIT; break; //No path left; only death
			//case 1: axis = AXIS_X; direction = LEFT; break;
			//case 2: axis = AXIS_X; direction = RIGHT; break;
			//case 3: axis = AXIS_Y; direction = UP; break;
			//case 4: axis = AXIS_Y; direction = DOWN; break;
			//}

			//From testing: I have to stop the snake from running into its tail
			//Do this by exploring the areas near the tail and chasing those instead
			int place_to_go = 0;
			int above_tail[2] = { tail_coord[0], tail_coord[1] - 1 };
			if ((board->occupancy[above_tail[1]][above_tail[0]] == 0) && (!place_to_go)) //If it's an empty occupancy
			{
				if (!((above_tail[0] == coord[0]) && (above_tail[1] == coord[1]))) //And it's not the same one as current one
				{
					direction = a_star(coord, above_tail, board);
					if(direction != 0) place_to_go = 1;
				}
			}

			int below_tail[2] = { tail_coord[0], tail_coord[1] + 1 };
			if ((board->occupancy[below_tail[1]][below_tail[0]] == 0) && (!place_to_go)) //If it's an empty occupancy
			{
				if (!((below_tail[0] == coord[0]) && (below_tail[1] == coord[1]))) //And it's not the same one as current one
				{
					direction = a_star(coord, below_tail, board);
					if (direction != 0) place_to_go = 1;
				}
			}

			int right_of_tail[2] = { tail_coord[0] + 1, tail_coord[1] };
			if ((board->occupancy[right_of_tail[1]][right_of_tail[0]] == 0) && (!place_to_go)) //If it's an empty occupancy
			{
				if (!((right_of_tail[0] == coord[0]) && (right_of_tail[1] == coord[1]))) //And it's not the same one as current one
				{
					direction = a_star(coord, right_of_tail, board);
					if (direction != 0) place_to_go = 1;
				}
			}

			int left_of_tail[2] = { tail_coord[0] - 1, tail_coord[1] };
			if ((board->occupancy[left_of_tail[1]][left_of_tail[0]] == 0) && (!place_to_go)) //If it's an empty occupancy
			{
				if (!((left_of_tail[0] == coord[0]) && (left_of_tail[1] == coord[1]))) //And it's not the same one as current one
				{
					direction = a_star(coord, left_of_tail, board);
					if (direction != 0) place_to_go = 1;
				}
			}
			switch (direction)
			{
			case 0: axis = AXIS_INIT; direction = DIR_INIT; break; //No path left; only death
			case 1: axis = AXIS_X; direction = LEFT; break;
			case 2: axis = AXIS_X; direction = RIGHT; break;
			case 3: axis = AXIS_Y; direction = UP; break;
			case 4: axis = AXIS_Y; direction = DOWN; break;
			}
		}
		play_on = advance_frame(axis, direction, board);
		show_board(board);
		printf("Going ");

		if (axis == AXIS_X){
			if (direction == RIGHT){
				printf("RIGHT");
			} else {
				printf("LEFT");
			}
		} else {
			if (direction == UP){
				printf("UP");
			} else {
				printf("DOWN");
			}
		} printf("\n");
		//usleep(555550);
		//_sleep(555);
		_sleep(200);
	}

	
	end_game(&board);

}

int main() 
{
	play_game();
	return 0;
}
