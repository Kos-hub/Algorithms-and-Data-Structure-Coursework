#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//Global variables
int row_max = 0;
int col_max = 0;
int game_mode = 0; //This defines how many matches we need for winning.

//Functions

//Board functions
void init_board(int**); //Fills up the board with 0s.
void printboard(int**); //Prints the board on the screen.

//Gameplay functions
int is_valid_location(int**, int); //Checks if the column has been filled.
int get_next_free_row(int**, int); //Gets the last (or first, depending on the points of view) free row where to "drop" the piece.
void drop_piece(int**, int, int, int);

//Winning function
int winning_move(int**, int, int, int);

main(void) 
{
	int** board; //Initializing board.
	bool game_over = false; //Setting the game over boolean.
	int turn = 0; //Initializing turn. Turn 0 = Player 1; Turn 1 = Player 2.
	int col; //Initalizing the column selected by the user.

	//MAIN MENU SCRIPT.
	printf("----MAIN MENU----\n");
	printf("Select game mode: ");
	scanf("%d", &game_mode);

	printf("Select rows: ");
	scanf("%d", &row_max);

	printf("Select columns: ");
	scanf("%d", &col_max);
	//END OF MAIN MENU SCRIPT.

	//Making the 2D array resizable.
	board = malloc(sizeof(int*) * row_max);

	for (int i = 0; i < row_max; i++) 
	{
		board[i] = malloc(sizeof(int) * col_max);
	}

	//Initialising and printing the board.
	init_board(board);
	printboard(board);

	while (!game_over) 
	{
		if (turn == 0) 
		{
			//Player input
			printf("Player 1 make your choice (0-%d): ", col_max-1);
			scanf("%d", &col);

			if (col < col_max && is_valid_location(board, col)) //We check if the column is filled.
			{
				//Gameplay
				int row = get_next_free_row(board, col); //If the column is not filled (or wrong), then we can call the function that gets the next free row where we are going to drop the piece
				drop_piece(board, col, row, 1); //After this we call a simple function that drops the piece into the board and reprints it. (1 defines the player 1 piece.)

				if (winning_move(board, col, row, 1)) 
				{
					printf("Player 1 WINS!!!");
					//Game ending
					game_over = true;
					break;
				}

				//Turn swapping
				turn = 1;
				continue;

			}
			else {
				printf("Wrong input! Either out of range or filled up column!\n");
			}
		}
		else if (turn == 1) 
		{
			
			printf("Player 2 make your choice (0-%d): ", col_max - 1);
			scanf("%d", &col);

			if (col < col_max && is_valid_location(board, col)) 
			{
				//Gameplay
				int row = get_next_free_row(board, col);
				drop_piece(board, col, row, 2); //2 defines the player 2 piece

				if (winning_move(board, col, row, 2))
				{
					printf("Player 2 WINS!!!");
					//Game ending
					game_over = true;
					break;
				}

				//Turn swapping
				turn = 0;
				continue;
			}
			else 
			{
				printf("Wrong input! Either out of range or filled up column!\n");
			}
		}
	}
	//Wait for input to finish
	int exit = 0;
	printf("\nPress 0 to exit: ");
	scanf("%d", &exit);
	free(board);
}

void init_board(int** b) 
{
	for (int i = 0; i < row_max; i++)
	{
		for (int j = 0; j < col_max; j++)
		{
			b[i][j] = 0; //Fills the array with 0.
		}
	}
}

void printboard(int** b) 
{
	system("cls"); //Clears the screen to get a better view at the table.
	int i, j, k;

	printf("||||CONNECT FOUR||||\n\n");

	for (i = 0; i < row_max; i++)
	{
		for (j = 0; j < col_max; j++)
		{
			printf("| %d", b[i][j]); //Prints all the elements in the array.
		}
		printf("|\n");

		for (k = 0; k < col_max; k++) {
			printf("---"); //Prints a separating line for each space.
		}
		printf("\n");
	}
	printf("  "); //This is used to align the numbers underneath.
	for (i = 0; i < col_max; i++) 
	{
		printf("%d  ", i); //For each column we get a number from 0 to the maximum number of columns - 1.
	}
	printf("\n");
}

int is_valid_location(int** b, int col) {
	if (b[0][col] == 0) //Simply checks the first row. If it's 0, it's not filled, then it's a valid location. Else, it's not.
	{
		return 1;
	}
	else 
	{
		return 0;
	}
}

int get_next_free_row(int** b, int col) 
{
	for (int i = row_max - 1; i >= 0; i--) //We count backwards because we want to start from the bottom, not from the top.
	{
		if (b[i][col] == 0) 
		{
			return i; //Returns the free row.
		}
	}
	return -1; //Returns negative if nothing happens.
}

void drop_piece(int** b, int col, int row, int piece)
{
	b[row][col] = piece;
	printboard(b);
}

int winning_move(int** b, int col, int row, int piece) 
{
	int i, j;
	int count = 0; //Counting how many pieces we got in a row, in a column, in diagonal.
	int limit = game_mode - 1;
	int col_limit = col_max - limit; //Calculating the limits of our board depending on how many rows we want to have.
	int row_limit = row_max - limit; //Same as above.

	//Checks horizontal win
	for (i = 0; i <= col_max; i++) 
	{
		if (b[row][i] == piece) 
		{
			count++;
		}
		else 
		{
			count = 0;
			break;
		}

		if (count == game_mode) 
		{
			return 1;
		}
	}
	
	//Checks vertical win
	for (i = 0; i < row_max; i++) 
	{
		if (b[i][col] == piece) 
		{
			count++;
		}
		else 
		{
			count = 0;
		}

		if (count == game_mode) 
		{
			return 1;
		}
	}

	//Checks all rows for diagonal win (From top left to bottom right)
	for (int rowTemp = 0; rowTemp < row_limit; rowTemp++) //We only check specific rows because we are not going to check them all, since it would be impossible to have 4 (or more or less) pieces in a diagonal
	{
		for (i = rowTemp, j = 0; i < row_max && j < col_max; i++, j++) //Then we pass the current row to this for loop and we check for each single piece going down.
		{
			if (b[i][j] == piece) 
			{
				count++;
			}
			else 
			{
				count = 0;
				break;
			}

			if (count == game_mode) 
			{
				return 1;
			}
		}
	}

	//Checks all the columns for diagonal win (From top left to bottom right)
	for (int colTemp = 0; colTemp < col_limit; colTemp++) //Same thing has before, but we check for columns now.
	{
		for (i = 0, j = colTemp; i < row_max && j < col_max; i++, j++) 
		{
			if (b[i][j] == piece)
			{
				count++;
			}
			else
			{
				count = 0;
				break;
			}

			if (count == game_mode)
			{
				return 1;
			}
		}
	}

	//Checks all rows for diagonal win (From bottom left to top right)
	for (int rowTemp = row_max - 1; rowTemp >= 3; rowTemp--) //Same for the top left to bottom right, we limit our serch because it is impossibile to have 4 (or more or less) pieces in a diagonal
	{
		for (i = rowTemp, j = 0; i >= 0 && j < col_max; i--, j++)
		{
			if (b[i][j] == piece)
			{
				count++;
			}
			else
			{
				count = 0;
				break;
			}

			if (count == game_mode)
			{
				return 1;
			}
		}
	}

	//Checks all columns for diagonal win (From bottom left to top right)
	for (int colTemp = 0; colTemp < col_limit; colTemp++) //Same thing has before, but we check for columns now.
	{
		for (i = row_max - 1, j = colTemp; i >= 0 && j < col_max; i--, j++)
		{
			if (b[i][j] == piece)
			{
				count++;
			}
			else
			{
				count = 0;
				break;
			}

			if (count == game_mode)
			{
				return 1;
			}
		}
	}
	return 0;
}