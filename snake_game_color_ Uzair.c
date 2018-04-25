#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<Windows.h>
#include<time.h>
/*
some final things are left, but by large most of it is completed.
Group members: (Section F)

Leader's Detail
Name: Uzair Khan
ID: 152183
Email: k152183@nu.edu.pk

Member Details

1
Name: Hammad Jafri
ID: 152194

2
Name: Usama Waseem
ID: 152362

3
Name: Hamza Mustafa
ID: 152832

Project Details : Snake game	
	*/

void GoToXY(int column, int line)
{
    COORD coord;
    coord.X = column;
    coord.Y = line;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsole, coord);
}

void hidecursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

colorfunc(int red_green_blue_text, int black_white_back)
{
	if(red_green_blue_text == 0 && black_white_back == 0) /*red and black*/
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);	
				
	if(red_green_blue_text == 0 && black_white_back == 1)/*red and white*/
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);	

	if(red_green_blue_text == 1 && black_white_back == 0)/*green and black*/
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);	

	if(red_green_blue_text == 1 && black_white_back == 1)/*green and white*/
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);	

	if(red_green_blue_text == 2 && black_white_back == 0)/*blue and black*/
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);	

	if(red_green_blue_text == 2 && black_white_back == 1)/*blue and white*/
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);	
	
	if(red_green_blue_text == -1 && black_white_back == 0)/*color reset*/
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1 | 2);

	if(red_green_blue_text == -1 && black_white_back == 1)/*light blue and white*/
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1 | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);

}
main()
{
	hidecursor();/*hides the cursor in the console*/
	char menu[][40] = {"> 1-Player" , "  2-Player", "  Help & Options", "  Map Editor" , "  Highscores" , "  About" , "  Exit"}; /*main menu*/
	int game_end = 0;

	int menu_selection_choice = 0, limit = 6;

	while(game_end != 99)/*99 is returned only when exit is presed in the main menu*/
	{
//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1 | 2);
	colorfunc(-1, 0); /*color reset*/
	
		title_screen(menu);/*function to print the menu and the big SNAKE ASCII*/
	
		while(up_down_menu_selection(menu, &menu_selection_choice, limit) == 99)
			title_screen(menu);
		/*function which changes the array. pressing up or down changes the position of the arrow.	 pressing up or down in the function returns 99. the loop runs again and the changes array is printed*/
	
		game_end = menu_choice(menu_selection_choice);
		/*when enter is prentered the above loop exits and the value of the array index number where the cursor was is returned. this function is a swithc statement that
		uses the array index number to launch different functions*/
	}

}
				
/*char maze is a 2d char array which holds the game map. selection is a boolean that shows whether a user picked a predefined map or not. snakecolor is the users choice for snake colour,
 it is -1 if default, back color is the background color. option_speed[] holds the wait values for usleep. snake_speed indicates which specific value was picked. var_speed is a boolean
which tells whether variable speed is on or not. time_val is the time limit for the game round. if it is zero then there is no time limit.*/			
snake_1_player(char (*maze)[100], int selection, int snake_color, int back_color, int snake_speed, int option_speed[3], int var_speed, int time_val)
{
	int sizerow = 24, sizecol = 79, move = 0, exit = 0, retry = 0; 	/*sizerow and sizecol are the constraints of the console windows. move tells in which direction the snake is currently moving. retry is 
	used later when asking user to play again. exit is a flag which is used to check whether the game has ended*/
	int snakepos[500][2], snakesize = 5, snake1_points_var;/*snakepos is a 2d int array that stores the row and column coordinates of the snake. the head is stored in the [0] index number and the tail in the snakesize = 1 index number
	y-coordinate or row is stored in the first column, and snake x coordinate or column in stored in the second colums of the array. snakesize denotes how big the snakeis. 5 means initial stage. snake1_points_var is a variable that states how
	much points the player has gathered*/
	int last_snake_one[1][2]; /*postion of tail of snake. the position of the tail is stored because later the coordinates of the tail need to be cleared. before that however the coordinates in snakepos will be updated, and so this last_ array
	is used to store the row and column of the tail.*/
	int var_speed_start = 15;/*states at what point does the variable speed increase start to function. for example if put at 15, variable speed increase will happen when the snakesize is 15 or when the user ahs eaten
	10 dots.*/
	int tail_check, var_point_check = var_speed_start, sleep_val = option_speed[snake_speed];/*tail_check is a flag to check whether the snake has eaten a dot since the last time tail_check checks it or not. this check is used because the index
	number of the snake tail in the snakepos needs to be knows, and of the snakesize has increased since last checked then the last snake array can store some undefined values. var_point_check is used in the
	end function to increase the speed of the snake. sleep_val is the modified sleep value of the game after variable speed kicks in*/
	int copr = 0, copc = 0; //coordinates of points in row, coordinates of points in columns	
	int snake_point_speed = snake_speed; /*snake speed int used in point calaculation formula*/
	/* Intializes random number generator. used to generate the coordinates of the dot that the snake eats */
	time_t t, start, end;
	srand((unsigned) time(&t));

	colorfunc(-1, back_color);/*turns the color of the screen to light blue text with the background color that the user enterd. light blue text because the obstacles generated will be light blue.*/
	system("cls");/*necessary to turn whole screen white if player picks that background color and a map*/
	
	if(selection == 0)/*tells whether the user opened a map or not. if no map was preselected by the user then selection is zero and the game prints a new map*/
		printing_maze(maze, sizerow, sizecol);/*clears the maze of any garbage values and prints it. prints a simple maze*/
	
	else
		clear_game_maze_retry(maze);/*this function clears the maze of anything that constitutes a snake like X or =. it then reprints the maze*/
		
	snake_1_init(snakesize, snakepos, maze);/*initializes a snake into the maze. snakesize is sent and the function first generates the coordinates for the snake and then enters the coordintates into the maze
	bear in mind that the snake isnt actually printed, just its coordinates are entered in the maze*/	
	
	start = time(NULL);/*used for the times rounds. measures the value of when the games started to use as a reference point*/
	tail_check = snakesize;/*initializes the check to the current snakesize*/
	
	do
	{
	
	if (tail_check == snakesize)
		snake_tail_update(last_snake_one, snakepos, snakesize); /*stores the position of the snake tail so that it can be cleared later*/
	/*if the check is the same as the curent snakesize then the snake has not eaten any new dots since the last time it was checked. the index number of the snake tail is the 
	same as before*/
		
	else
		snake_tail_update(last_snake_one, snakepos, snakesize-1); /*stores the position of the snake tail so that it can be cleared later*/
	/*if check is different than snakesize then snakesize - 1 is sent to the function to store the appropriate value of the tail. if snakesize=1 wasnt sent then the last snake array would try to store
	the coordinates  from garbage because the function would be pointed towards garbage. see the function for more details*/	
		
		tail_check = snakesize;/*if tail check was differnt than snakesize above, this  makes it equal again so that for the next iteration the correct check can be used. if this wasnt equated
		then the above if else condition would be continously and erroneously true.*/
		
	snakepos_update(snakepos, snakesize); /*updates every coordinate of the snake to the one in front of it. the coordinates in the last sindex number are updated to become the second last and so forth.
	this functin therefore results in the loss of the tail coordinates  stored in snakepos. this is why the coordinates were soted above int the snake+tail+update function*/
		
	move = snake_1_movement(snakepos, move); /*takes keyboard input to control snake. move is returned to main to show how the snake is corrently moving*/


	if(maze[copr][copc] != '*') 
			dot_check(maze, &copr, &copc);
	/*dot cordinates checking. if dot is uneaten this doesnt run. This runs when the coordinates of maze[copr][copc] show an X for snakehead. this means that the dot is eaten
	and therefore the functino to generaate new dot coordinate runs*/	
	
	
	snakesize = snakehead_on_dot(maze, snakepos, snakesize);/*checks whether the snakehead is on the dot. takes the current snakesize as a parameter and returns the new snakesize, in case the dot WAS eaten 
	and the snakesize increased*/	
		
	if (sleep_val == option_speed[snake_point_speed + 1] && snake_point_speed < 2)
		snake_point_speed++;
//	snake1_points_var = (snakesize - 5) + (5 * snake_speed * (snakesize - 5)) + (var_speed *( (option_speed[snake_speed] - sleep_val) / 5000)* (snakesize - 5));/*other possible formula*/
	snake1_points_var = (snakesize - 5) + (5 * snake_point_speed * (snakesize - 5));
	/*equation to generate the points of a person. there are 3 parts to this. how many dots the snake has eaten, what speed he is playing at and whether the speed is increaing through variable speed or not*/
	
	snake_1_points(snake1_points_var);	/*takes current snakepoints as input and simply prints them at the bottom of the screen*/
					
		
	exit = game_end(maze, snakepos);/*checks whether the game has ended by conmparing the coordinates of the snakehead in the first index row of snakepox with the char array amze. if the snakehead is on
	sometihng that the char array shows had an obstacle, then the game ends. the function returns 1 if the game has ended.*/
	
	colorfunc(snake_color, back_color);	/*turns the color of the text to what the user picked for the snake*/
	
	if (exit != 1)
			snake_movement_coordinates_update(maze, last_snake_one, snakepos); /*for snake one*/
	/*if the game ahs not yet ended then this function runs. the maze, tail array and snakecoordinates are passed, and the function clears the previous tail position and the previous head position
	to other characters. see function for more details*/
			
	colorfunc(-1, back_color);/*resents the console text color to light blue. background color remains the same*/
				
	if (time_val != 0)/*if time_val si not zero means that a timed round is gound on.*/
	{
		end = time(NULL);/*check the corrent time*/
		GoToXY(32,24);/*goes to a special postion to write the round time*/
		printf("Round %2d Time", (time_val) - (end - start));/*timeval is the total time. end is always greater than start, there difference fhoes how much time has passed since
		the start of the game. subtracting that from timeval shows how much time has left. time_val is the total time limit*/
		
		if(end - start == time_val)/*if the round time limit has been reached*/
			exit = 1;/*game has ended. exit the game*/
	}
	
		
	if (var_speed == 0 || ((var_speed == 1) && (snakesize < var_speed_start)))/*if variable speed is off, or if vaurable speed is on bu the threshold for pseed increase has not yet been reached*/
		usleep(option_speed[snake_speed]); 
		/*pause the function for this amount of time. lowering the parameter of the function increases speed since the program has o wait less, and vice versa. option
		speed is the array that stores the predefined usleep pause values for the different snake speeds. snake_speed is a paramenter passed which indicates what speed the user picked*/
	
	else if(snakesize >= var_speed_start && var_speed == 1) /*if variable speed is on and the user has eaten the dots required for speed increase*/
		{
			/*var_point_check is always greater than snakesize by 1 or more. whenthe difference is only one, whenever the snake eats a dot, the snakesize increases and the
			two are equal. this indicates the threshold has been reachec*/
			if (var_point_check == snakesize)
			{	
				if (sleep_val > option_speed[2]) /*if current wait time is greater than minimum possible*/
					sleep_val -= 1000; /*reduce the wait time*/
				var_point_check++; /*increment the check, so that this condition only runs once per increase in snakesize*/
			}
			usleep(sleep_val); /*the modified wait value*/
		}
		
	}while(exit != 1);	/*run the loop whie the game has not ended*/
	
	colorfunc(-1, 0); /*color reset for menus and highscores*/
	
	highscore_printing(snake1_points_var, 1);/*send the player points to the highscore funciton*/

	retry = retry_condition_1_player(snake1_points_var); /*ask the player if he wants to play again. retry condition returned*/
	
	
	if (retry == 0)/*if player wants to play again, retry is zero*/
	{
		clear_game_maze_retry(maze);/*clear the char array maze of = and X so that only obstacles are left*/
		snake_1_player(maze, selection, snake_color, back_color, snake_speed, option_speed, var_speed, time_val);/*play again.*/
				
	}
		
}/*end of snake 1 player function*/

/*char maze is a 2d char array which holds the game map. selection is a boolean that shows whether a user picked a predefined map or not. snakecolor is the users choice for snake colour,
 it is -1 if default. snake_2_color is the color choice for the second snake back color is the background color. option_speed[] holds the wait values for usleep. snake_speed indicates which specific value was picked. var_speed is a boolean
which tells whether variable speed is on or not. time_val is the time limit for the game round. if it is zero then there is no time limit.*/			
snake_2_player(char (*maze)[100], int selection, int snake_color, int snake_2_color, int back_color, int snake_speed, int option_speed[3], int var_speed, int time_val)
{
	int sizerow = 24, sizecol = 79, move = 0, move2 = 1, exit = 0, retry = 0, losing_player; 	/*sizerow and sizecol are the constraints of the console windows. move tells in which direction the snake is currently moving. retry is 
	used later when asking user to play again. exit is a flag which is used to check whether the game has ended*/
	int snakepos[500][2], snakepos2[500][2], snakesize = 5, snakesize2 = 5;/*snakepos is a 2d int array that stores the row and column coordinates of the snake. the head is stored in the [0] index number and the tail in the snakesize = 1 index number
	y-coordinate or row is stored in the first column, and snake x coordinate or column in stored in the second colums of the array. snakesize denotes how big the snakeis. 5 means initial stage. snake1_points_var is a variable that states how
	much points the player has gathered. snakepos2 are the snake position coordinates for player 2. snakesize2 is  the length of the second snake*/
	int snake1_points_var, snake2_points_var; /*the points of the first and second player respectively*/
	int last_snake_one[1][2], last_snake_two[1][2]; /*postion of tail of snake of snake one and snake two the position of the tail is stored because later the coordinates of the tail need to be cleared. before that however the coordinates in snakepos will be updated, and so this last_ array
	is used to store the row and column of the tail.*/
	int var_speed_start = 15;/*states at what point does the variable speed increase start to function. for example if put at 15, variable speed increase will happen when the snakesize is 15 or when the user ahs eaten
	10 dots.*/
	int tail_check[2], var_point_check = var_speed_start, sleep_val = option_speed[snake_speed];/*tail_check is a flag to check whether the snake has eaten a dot since the last time tail_check checks it or not. this check is used because the index
	number of the snake tail in the snakepos needs to be knows, and of the snakesize has increased since last checked then the last snake array can store some undefined values. since there are two snakes
	now, tail_check is an array. var_point_check is used in the	end function to increase the speed of the snake. sleep_val is the modified sleep value of the game after variable speed kicks in*/
	int copr = 0, copc = 0; //coordinates of points in row, coordinates of points in columns	
	int snake_point_speed = snake_speed; /*snake speed int used in point calculation formula*/

	/* Intializes random number generator */
	time_t t, start, end;
	srand((unsigned) time(&t));

	colorfunc(-1, back_color); /*turns the color of the screen to light blue text with the background color that the user enterd. light blue text because the obstacles generated will be light blue.*/
	system("cls");/*necessary to turn whole screen white if player picks that background color and a map*/
	 
	start = time(NULL); /*used for the times rounds. measures the value of when the games started to use as a reference point*/
	
	if(selection == 0)/*tells whether the user opened a map or not. if no map was preselected by the user then selection is zero and the game prints a new map*/
		printing_maze(maze, sizerow, sizecol);/*clears the maze of any garbage values and prints it. prints a simple maze*/
	
	else
		clear_game_maze_retry(maze);/*this function clears the maze of anything that constitutes a snake like X or =. it then reprints the maze*/
		
	snake_1_init(snakesize, snakepos, maze);/*initializes a snake into the maze. snakesize is sent and the function first generates the coordinates for the snake and then enters the coordintates into the maze
	bear in mind that the snake isnt actually printed, just its coordinates are entered in the maze*/	
	snake_2_init(snakesize2, snakepos2, maze);/*same as above but for snake two*/
	
	tail_check[0] = snakesize; /*initializes the check to the current snakesize*/
	tail_check[1] = snakesize2;
	
	do
	{
	
	/*if the check is the same as the curent snakesize then the snake has not eaten any new dots since the last time it was checked. the index number of the snake tail is the 
	same as before*/
	if (tail_check[0] == snakesize)
		snake_tail_update(last_snake_one, snakepos, snakesize);/*stores the position of the snake tail so that it can be cleared later*/ 
	
	else
		snake_tail_update(last_snake_one, snakepos, snakesize - 1);
	
	if (tail_check[1] == snakesize2)
		snake_tail_update(last_snake_two, snakepos2, snakesize2);
		
	else
		snake_tail_update(last_snake_two, snakepos2, snakesize2 - 1);
		/*if check is different than snakesize then snakesize - 1 is sent to the function to store the appropriate value of the tail. if snakesize=1 wasnt sent then the last snake array would try to store
	the coordinates  from garbage because the function would be pointed towards garbage. see the function for more details*/	

	/*if tail check was differnt than snakesize above, this  makes it equal again so that for the next iteration the correct check can be used. if this wasnt equated
		then the above if else condition would be continously and erroneously true.*/
	tail_check[0] = snakesize;
	tail_check[1] = snakesize2;
	
	/*updates every coordinate of the snake to the one in front of it. the coordinates in the last sindex number are updated to become the second last and so forth.
	this functin therefore results in the loss of the tail coordinates  stored in snakepos. this is why the coordinates were soted above int the snake+tail+update function*/
	snakepos_update(snakepos, snakesize); 
	snakepos_update(snakepos2, snakesize2);/*same as above but for snake 2*/
		
	move = snake_1_movement(snakepos, move); /*takes keyboard input to control snake. move is returned to main to show how the snake is corrently moving*/
	move2 = snake_2_movement(snakepos2, move2);
	
	
	if(maze[copr][copc] != '*') 
		dot_check(maze, &copr, &copc);
		/*dot cordinates checking. if dot is uneaten this doesnt run. This runs when the coordinates of maze[copr][copc] show an X for snakehead. this means that the dot is eaten
	and therefore the functino to generaate new dot coordinate runs*/	
	
		
	snakesize = snakehead_on_dot(maze, snakepos, snakesize);	
	snakesize2 = snakehead_on_dot(maze, snakepos2, snakesize2);
	/*checks whether the snakehead is on the dot. takes the current snakesize as a parameter and returns the new snakesize, in case the dot WAS eaten 
	and the snakesize increased*/	

	if (sleep_val == option_speed[snake_point_speed + 1] && snake_point_speed < 2)
		snake_point_speed++;
//	snake1_points_var = (snakesize - 5) + (5 * snake_speed * (snakesize - 5)) + (var_speed *( (option_speed[snake_speed] - sleep_val) / 5000)* (snakesize - 5));/*other possible formula*/
	snake1_points_var = (snakesize - 5) + (5 * snake_point_speed * (snakesize - 5));
	snake2_points_var = (snakesize2 - 5) + (5 * snake_point_speed * (snakesize2 - 5));
	/*equation to generate the points of a person. there are 3 parts to this. how many dots the snake has eaten, what speed he is playing at and whether the speed is increaing through variable speed or not*/
	
	/*equation to generate the points of a person. there are 3 parts to this. how many dots the snake has eaten, what speed he is playing at and whether the speed is increaing through variable speed or not*/
	snake_2_points(snake1_points_var, snake2_points_var);/*prints the points of the players at the bottom of the screen*/
			
	exit = game_end(maze, snakepos);
	/*checks whether the game has ended by conmparing the coordinates of the snakehead in the first index row of snakepox with the char array amze. if the snakehead is on
	sometihng that the char array shows had an obstacle, then the game ends. the function returns 1 if the game has ended. THIS CHECK IS FOR PLAYER ONE*/
		
	colorfunc(snake_color, back_color);/*turns the color of the text to what the user picked for the snake*/
	
	if (exit != 1) /*if game has not ended for player 1 i.e. he has not hit any obstacles then his current coordinates are enterd into the maze*/
		snake_movement_coordinates_update(maze, last_snake_one,snakepos); /*for snake one*/
		/*the maze, tail array and snakecoordinates are passed, and the function clears the previous tail position and the previous head position
		to other characters. see function for more details*/

	losing_player = 1; /*this is a flag. if player one has hit an obstacle then exit has turned one above. the below conditions for player 2 will not run. losing_player has been determined as one and
	the game will exit the loop*/
	
	if (exit != 1)/*if player 1 has not hit a obstacle*/
	{
		exit = game_end(maze, snakepos2);/*check if player 2 has hit an obstacle*/
		losing_player = 2;/*if player 2 did hit an obstacle then the game will exit the loop. losing_player has turned from 1 to 2. the game therefore knows that player 2 was the one that hit
		an obstacle*/
	}
	
	if (exit != 1)/*if neither player one nor player 2 have hit an obstacle*/
	{
		colorfunc(snake_2_color, back_color);/*turn the console text color to the one chosen for player 2*/
		snake_movement_coordinates_update(maze, last_snake_two,snakepos2); /*for snake two*/
		/*if the game has not yet ended then this function runs. the maze, tail array and snakecoordinates are passed, and the function clears the previous tail position and the previous head position
		to other characters. see function for more details*/

	}

	colorfunc(-1, back_color); /*color reset*/
	
	if (time_val != 0)/*if time_val is not zero means that a timed round is gound on.*/
	{
		end = time(NULL);/*check the corrent time*/
		GoToXY(28,24);/*goes to a special postion to write the round time*/
		printf("Round   %2d   Time", (time_val) - (end - start));/*timeval is the total time. end is always greater than start, there difference fhoes how much time has passed since
		the start of the game. subtracting that from timeval shows how much time has left. time_val is the total time limit*/
		
		if(end - start == time_val)/*if the round time limit has been reached*/
		{
			exit = 1;/*game has ended. exit the game*/
			
			if (snake1_points_var == snake2_points_var)/*same points when time runs out. draw*/
				losing_player == 0;
				
			if (snake1_points_var > snake2_points_var)/*player 1 wins*/
				losing_player = 2;
				
			else /*player 2 wins*/
				losing_player = 1;
		}
	}

	if (var_speed == 0 || ((var_speed == 1) && (snakesize < var_speed_start)))/*if variable speed is off, or if vaurable speed is on bu the threshold for pseed increase has not yet been reached*/
		usleep(option_speed[snake_speed]); 
		/*pause the function for this amount of time. lowering the parameter of the function increases speed since the program has o wait less, and vice versa. option
		speed is the array that stores the predefined usleep pause values for the different snake speeds. snake_speed is a paramenter passed which indicates what speed the user picked*/
	
	
	else if((snakesize >= var_speed_start && var_speed == 1) || (snakesize2 >= var_speed_start && var_speed == 1))
		{
			/*var_point_check is always greater than snakesize by 1 or more. whenthe difference is only one, whenever the snake eats a dot, the snakesize increases and the
			two are equal. this indicates the threshold has been reachec*/

			if ((var_point_check == snakesize) || (var_point_check == snakesize2))
			{	
				if (sleep_val > option_speed[2])/*if current wait time is greater than minimum possible*/
					sleep_val -= 1000; /*reduce the wait time*/
				var_point_check++; /*increment the check, so that this condition only runs once per increase in snakesize*/
			}
			usleep(sleep_val); /*the modified wait value*/
		}
		
	}while(exit != 1);	/*run this loop while game is running*/

	/*highscore only runs if one of them wins. does not run in the condition of a draw. draw only happens when time runs out and points are euqal between players*/
	if (losing_player == 2) /*if player 2 llost, player 1 won, and so send player 1 score to highscore function, the second parameter 2 is a flag for 2 player highscores*/
		highscore_printing(snake1_points_var, 2);

	else if (losing_player == 1) /*if player 1 lost, player 2 won and so send player 2's points to highscore function, the second parameter 2 is a flag for 2 player highscores*/
		highscore_printing(snake2_points_var, 2);


	retry = retry_condition_2_player(snake1_points_var, snake2_points_var, losing_player); /*ask if user wants to play again*/
	
	if (retry == 0)/*if retry == 0, start game again*/
		snake_2_player(maze, selection, snake_color, snake_2_color ,back_color, snake_speed, option_speed, var_speed, time_val);
		
}/*end of snake 2 player function*/





/*print and clear the maze, used when player has not selected any predefined map to play. also used in map editor when user wants to create a new map
receives the char array maze to modify, and the row and column restraints fo the console*/
printing_maze(char maze[][100], int sizerow, int sizecol)
{
	int i, j;
	
	system("cls");	
	
	for (i = 0; i < sizerow; i++) //clearing the maze
	{
		for(j = 0; j < sizecol; j++)
			maze[i][j] = ' ';
	}/*wipes the maze  array of garbage values*/
	

	for (i = 0; i < sizerow; i++) //printing the maze
	{
		for(j = 0; j < sizecol; j++)
		{
			if (i == 0)
			{
				printf("_");
				maze[i][j] = '_';
			}
			
			else if (j == 0)
			{
				printf("|");
				maze[i][j] = '|';
			}
	
				
			else if (j == sizecol - 1)
			{
				printf("|");
				maze[i][j] = '|';
			}
				
			else if (i == sizerow - 1)
			{
				printf("-");
				maze[i][j] = '-';
			}
			
				
			else
				printf("%c", maze[i][j]);
					
		}/*if the first or last row/column then put a special character to signify boundary and sotre that character in the maze array. print all characters of the maze onto the console
		which is only the boundsaries and the spaces where there are no obstacles.*/
			
		printf("\n");
	}		
}/*end of printing maze function*/

/*initializes snake into the maze. snakesize is the initial length of the snake. snakepos is the array to store the coordinates of the snake parts*/
snake_1_init(int snakesize, int snakepos[][2], char maze[][100]) /*initializing co orsinates of snake 1*/
{	/*initcol is used to initialize the snake. when game starts, the whole snake in row 10, but the column coordinates of every snake part is different. initcol acheives that.*/
	int i, row, col, initcol = 10;
	
	/*start from the last index number or the tail of the snake. first the snake tail coordinates are initialized, then initcol is incremented. this incremented value is then used to initialize the 
	second last piece of snake. initcol is incremented again and the cycle goes on. head coordinates are in index[0] of snakepos array. in this loop, only the coordinates of the snake are
	generated. they have not been printed or stored yet.*/
	for (i = snakesize - 1; i >= 0; i--) //initializing snake into row 10 
	{
		snakepos[i][0] = 10;
		snakepos[i][1] = initcol++;	
	}
	
	
/*stores the positions of snake into the maze array. also prints them. if the index number is [0] then print an X for the head otherwise print a = for the body*/
	for(i = 0; i < snakesize; i++) //for snake postion in maze
	{
		row = snakepos[i][0];
		col = snakepos[i][1];
		maze[row][col] = '=';	
	
		if (i == 0)
			maze[row][col] = 'X';
	}		

}/*end of snake 1 init*/

/*initializes snake into the maze. snakesize is the initial length of the snake. snakepos is the array to store the coordinates of the snake parts. called only in tweo player snake. only difference is
initcol value and decrementation. made a new function but same could be acheived with using just a flag/boolean value. but this functino was amde in the early days*/
snake_2_init(int snakesize, int snakepos2[][2], char maze[][100]) /*initilazing coordinates of snake 2*/
{
	int i, row, col, initcol = 60;
	
	/*start from the last index number or the tail of the snake. first the snake tail coordinates are initialized, then initcol is decremented. this decremented value is then used to initialize the 
	second last piece of snake. initcol is decremented again and the cycle goes on. head coordinates are in index[0] of snakepos array. in this loop, only the coordinates of the snake are
	generated. they have not been printed or stored yet. its decremented here because the tail of snake two is the rightmost part of the snake with tus the lasrgest column coordinate. the snake faces
	left and  thus its column corrdinates are least. row coordinates for all parts are the same*/
		
	for (i = snakesize - 1; i >= 0; i--) //initializing snake into row 10 
	{
		snakepos2[i][0] = 10;
		snakepos2[i][1] = initcol--;	
	}
	
/*stores the positions of snake into the maze array. also prints them. if the index number is [0] then print an X for the head otherwise print a = for the body*/
	for(i = 0; i < snakesize; i++) //for snake postion in maze
	{
		row = snakepos2[i][0];
		col = snakepos2[i][1];
		maze[row][col] = '=';	
	
		if (i == 0)
			maze[row][col] = 'X';
	}		

}/*end of snake 2 init function*/

/*updates the snakepos coordinate array. the last index number[] coordinates are updated to become the second last and so on. every coordinate is updated. a sideeffect of this function is that the
coordianates of the tail are lost. to store them another funciton is utilized. this function recieves the snakepos coordinate array and the snake length to find out from which row of the array to
start updating*/
snakepos_update(int snakepos[][2], int snakesize)
{
	int i;
	for(i = snakesize - 1; i > 0 ; i--) 
	{
		snakepos[i][0] = snakepos[i-1][0];/*updates row*/
		snakepos[i][1] = snakepos[i-1][1];/*updates column*/
	}		
}/*end of snakepos update function*/


/*function which determines where and how the snake is moving. uses asynckeystate to determine which key was pressed since the last call to this function. could not use getch as that
would pause the game. accepts the parameters snakepos[] for the snakecoordinates, specifically for the head of the snake. move is an int whice tells where the snake was previously moving. move
is also returned to the main funcion after being updated so that the new move can be sent later when the function is called again. could have used call by reference and used move as 
a pointer but since only one value was being returned i didnt use pointer.*/
int snake_1_movement(int snakepos[][2], int move)
{/*move 0 means right, 1 means left, 2 means up, and 3 means down*/
	/*in the four if counditions below, the idea is the same. check if  a specific key is being pressed, for example up. if the snake is not already going down then that if condition is true
	*/
	int row = snakepos[0][0]; //snakehead
	int col = snakepos[0][1]; //snakehead
		
	if ((GetAsyncKeyState(VK_RIGHT) & 0x8000 != 0) && move != 1) //if right key is pressed and snake is not already going left
			move = 0;
	
	if ((GetAsyncKeyState(VK_LEFT) & 0x8000 != 0) && move != 0) //if left key is pressed and snake is not already going right
		move = 1;
	
	if ((GetAsyncKeyState(VK_UP) & 0x8000 != 0) && move != 3) //if up key is pressed and snake is not already going down
		move = 2;
	
	if ((GetAsyncKeyState(VK_DOWN) & 0x8000 != 0) && move != 2) //if down key is pressed and snake is not already going up
		move = 3;
	
	snake_movement_else(snakepos, row, col, move);/*function which employs switch to find the new coordiantes for snakehead. returns*/
		
return move;	/*move is returned to main so that it can be passed again when funcion is called again*/
}

int snake_2_movement(int snakepos[][2], int move)
{/*move 0 means right, 1 means left, 2 means up, and 3 means down*/
		/*in the four if counditions below, the idea is the same. check if  a specific key is being pressed, for example up. if the snake is not already going down then that if condition is true
	 for two player this function tests WASD keys whose key coeds are in the pasameters of the getasynckeystate function*/

	int row = snakepos[0][0]; //snakehead
	int col = snakepos[0][1]; //snakehead
	
	if ((GetAsyncKeyState(0x44) & 0x8000 != 0) && move != 1) //if d key is pressed and snake is not already going left
			move = 0;
		
	if ((GetAsyncKeyState(0x41) & 0x8000 != 0) && move != 0) //if a key is pressed and snake is not already going right
			move = 1;

	if ((GetAsyncKeyState(0x57) & 0x8000 != 0) && move != 3) //if w key is pressed and snake is not already going down
			move = 2;
		
	if ((GetAsyncKeyState(0x53) & 0x8000 != 0) && move != 2) //if s key is pressed and snake is not already going up
			move = 3;

	snake_movement_else(snakepos, row, col, move);/*function which employs switch to find the new coordiantes for snakehead. returns*/
			
return move;	/*move is returned to main so that it can be passed again when funcion is called again*/
}

/*function whicih is basically a switch for variable move which is passed from the function above. determines which the direction the snake is going in, and according to that,
adjusts the row and column coordinates of the snakehead.*/
snake_movement_else(int snakepos[][2], int row, int col, int move)
	{		
			switch(move)
			{
				case 0:/*if snake is going right*/
					col++;/*increment the current column value of the snakehead*/
					snakepos[0][1] = col;/*assign the updated column value to the snakehead*/
				break;
				
				case 1:/*if snake is going left*/
					col--;/*decrement the current column value of the snakehead*/
					snakepos[0][1] = col;/*assign the updated column value to the snakehead*/
				break;
				
				case 2:/*if snake is going up*/
					row--;/*decrement the current row value of the snakehead*/
					snakepos[0][0] = row;/*assign the updated row value to the snakehead*/
				break;
				
				case 3:/*if snake is going down*/
					row++;/*increment the current row value of the snakehead*/
					snakepos[0][0] = row;/*assign the updated row value to the snakehead*/
				break;
									
			}
	}/*end of snake_movement_else function*/
	
/*updates the maze array with the new coordinates of the snake. the previous coordinates of the tail stored in the array last[] are passed, aswell as snakepos[]*/		
snake_movement_coordinates_update(char maze[][100], int last [][2], int snakepos [][2])
{

		GoToXY(last[0][1], last[0][0]);/*goes to previous tail coordinates*/
		printf(" ");/*clears the postion by printing space*/
		maze[last[0][0]][last[0][1]] = ' ';/*updates that postition in the maze as clear*/

		GoToXY(snakepos[1][1], snakepos[1][0]);/*goes to previous head position, now the part directly behind the new head. in first row of the snakepos[] array, while the new head
		 is in the [0] array row*/
		printf("=");/*prints an = over the X that was already present*/
		maze[snakepos[1][0]][snakepos[1][1]] = '=';/*updates the maze with the character =  instead of X*/

		GoToXY(snakepos[0][1], snakepos[0][0]);/*goes to new head position. the new head postion was obtained by the snake movement */
		printf("X");/*prints an to denote the snakehead*/
		maze[snakepos[0][0]][snakepos[0][1]] = 'X';/*stores the char X as representation of head in the char array maze[][]*/

}

/*receives the points of a person and displays them at the bottom*/
snake_1_points(int snake1_points_var)
{
		GoToXY(0,24);
		printf("Player 1 score %d", snake1_points_var);
}

/*receives points of two persons and displays them at the bottom*/
snake_2_points(int snake1_points_var, int snake2_points_var)
{
		GoToXY(0,24);
		printf("Player 1 score : %d", snake1_points_var);	
		
		GoToXY(60,24);
		printf("%d : Player 2 score", snake2_points_var);	
	
}		

/*stores the coordinates of the snaketail in the last[] array before they will be wiped out in the snakepos_update function*/
snake_tail_update(int last[][2], int snakepos[][2], int snakesize)/*snakesize used to determine which index number has the coordinates of the tail*/
{
	last[0][0] = snakepos[snakesize-1][0];
	last[0][1] = snakepos[snakesize-1][1];
}

/*runs only when the position where the dot was before does not have a dot not i.e. there is a snakehead there*/
dot_check(char maze[][100], int *copr, int *copc)
{
/*receives maze array[][] array and two pointer variables coordinates of points row(copr) and coordinates of points columns(copc).
these updated values will be needed in the main to check the presenece of a dot. as there are two values so calling by reference was necessary*/
	do
	{
		*copr = (rand() % 23) + 1;/*generate a random number between 0 and 23 and then add one to it. the addition was because row [0] is the maze boundary*/
		*copc = (rand() % 77) + 1;/*generate a random number between 0 and 77 and then add one to it. the addition was because column [0] is the maze boundary*/
	}while(maze[*copr][*copc] != ' ');/*keep generating new coordinates while the present coordinates are not blank i.e. they have some obstacle present*/

	GoToXY(*copc, *copr);/*go to the coordinate position*/
	printf("*");/*print a star there*/
	maze[*copr][*copc] = '*';/*update the maze with the new dot coordinates*/
}
	
/*checks whether the snakehead is on dot. ifit is, increases the snakesize. returns snakesize to the main*/	
int snakehead_on_dot(char maze[][100], int snakepos[][2], int snakesize)
{
	if (maze[snakepos[0][0]][snakepos[0][1]] == '*') //snakehead on dot. size increases.
	{
		maze[snakepos[0][0]][snakepos[0][1]] = ' ';/*update the coordinates of the dot to become blank*/
		snakesize++;/*increase snakesize*/
	}		
	return snakesize;
}	

/*game end conditions. checks whether the snakehead is on an obstacle or not.*/	
game_end(char maze[][100], int snakepos[][2])
{
	/*game end condition. row col means snakehead. condition means when snakehead is on 
	something other than blank spaces or dot*/
	if (maze[snakepos[0][0]][snakepos[0][1]] != ' ' && maze[snakepos[0][0]][snakepos[0][1]] != '*') 
			return 1;		

	else
		return 0;
}
	
retry_condition_1_player(int snakesize)/*retry condition. see up_down_menu_selection function for more details*/
{ 
	int menu_selection_choice = 0, limit = 1;
	char option[][40] = {"> Yes", "  No"};
	
	printing_retry_menu_1_player(option, snakesize);

	while(up_down_menu_selection(option, &menu_selection_choice, limit) == 99)
		printing_retry_menu_1_player(option, snakesize);

	return menu_selection_choice;
	
}

retry_condition_2_player(snakesize, snakesize2, losing_player)/*retry condition. see up_down_menu_selection function for more details*/
{
	int menu_selection_choice = 0, limit = 1;
	char option[][40] = {"> Yes", "  No"};

	printing_retry_menu_2_player(option, snakesize, snakesize2, losing_player);

	while(up_down_menu_selection(option, &menu_selection_choice, limit) == 99)
		printing_retry_menu_2_player(option, snakesize, snakesize2, losing_player);

	return menu_selection_choice;
}

printing_retry_menu_1_player(char option[][40], int snake1_points_var)/*prints retry screen for 1 player*/
{
	int i;
	
	system("cls");
	printf("\n\n\n\t\t\t\tYOUR SCORE");
	printf("\n\n\n\t\t\t\t     %d", snake1_points_var);
	
	printf("\n\n\n\t\t\t\tPlay again?\n\n");

	
	for(i = 0; i < 2; i++)
	printf("\t\t\t\t  %s\n\n", option[i]);

}


printing_retry_menu_2_player(char option[][40], int snake1_points_var, int snake2_points_var, int losing_player)/*prints retry screen for 2 player*/
{
	int i;
	
	system("cls");
	
	if (losing_player == 0)/*time ran out and scores were equal*/
		printf("\n\n\n\t\t\t    IT'S A DRAW!'");
	
	else
		printf("\n\n\n\t\t\t    PLAYER %d IS THE WINNER", (losing_player % 2) + 1);/*losing player is known. calculates and displays winning player*/
	
	printf("\n\n\n\t\tPLAYER 1 SCORE\t\t\tPLAYER 2 SCORE");
	printf("\n\n\n\t\t\t%d\t\t\t\t%d", snake1_points_var, snake2_points_var);
	
	printf("\n\n\n\t\t\t\tPlay again?\n\n");

	
	for(i = 0; i < 2; i++)
	printf("\t\t\t\t  %s\n\n", option[i]);

}

																/*END OF SNAKE GAME FUNCTIONS*/
																/*START OF MENU FUNCTIONS*/
/*prints the title screen*/
title_screen(char menu[][40])
	{
		system ("cls");
		int i;
		
		printf("\t\t ######  ##    ##    ###    ##    ## ######## \n"
				"\t	##    ## ###   ##   ## ##   ##   ##  ##       \n"
				"\t	##       ####  ##  ##   ##  ##  ##   ##       \n"
				"\t	 ######  ## ## ## ##     ## #####    ######   \n"
				"\t	      ## ##  #### ######### ##  ##   ##       \n"
				"\t	##    ## ##   ### ##     ## ##   ##  ##       \n"
				"\t	 ######  ##    ## ##     ## ##    ## ######## \n");
	
		printf("\n\n\n");
		
		for (i = 0; i < 7; i++)
			printf("\t\t\t\t%s\n\n" , menu[i]);
	}

/*function employing switch to call various funcitons when you press enter in the main menu*/
int menu_choice(int present)
{
	switch(present)
	{
		case 0:/*1 player*/
			snake_any_player_before_game(1);
			break;
			
		case 1:/*two player*/
			snake_any_player_before_game(2);
			break;
			
		case 2:/*help screen*/
			help_options();
			break;
						
		case 3:/*map editor*/
			editor_or_open();
			break;
			
		case 4:/*highscore*/
			highscore_selection();
			break;
			
		case 5:/*about screen*/
		about_screen();
			break;
			
		case 6: /*exit pressed*/
			return 99;
			break;
	}
}

/*one of the main functions used everywhere in the program receives a 2d array and stores it as options[][40], also receives an address stored in a pointer and an int called limit.
when function is called, it waits for a key press. if key pressed is down or up then getasynckeystate functions work. They modify the option[][] array by changing the arrow > in the
present row [0] position to ' ' and the new row [0] position to >. the new row is determined by whether up or down key was pressed. limit tells how many options there are, so thatprogram doesnt bug 
out trying to change something that doesnt exist. menu_selection_choice is a pointer because it is changed in this function and the parent function which called this specific function
needs the updated menu_selection_choice value, so tha when you do press enter, the appropriate value can be used to call other functions. if up or down key is pressed, then this funciton returns
99 which tells the parent function that enter key was not pressed. this function is always employed under a loop in a parent dfunction that keeps calling this function until the result
is something other than 99. the only way this returns something other than 99 is when you press enter, in whcih case it returns menu_selection_choice.*/

/*as the function is used everywhere here is an example case for this function. first string is >  yes in the below options array.
	
	char option[][40] = {">  Yes", "  No"};

	int menu_selection_choice = 0, limit = 1; menu_selection_choice = 1 tells where the arrwo is pressed by default. limit 5 tells the max index number of char options array
	
	printing_retry_menu_1_player(option, snakesize); a screen is printed that shows the array.

	while(up_down_menu_selection(option, &menu_selection_choice, limit) == 99) //this function modifies the array by changing the position of the arrow >. menu selection choice tells where the arrow currently is
		printing_retry_menu_1_player(option, snakesize); //menu is printed again if the user did not press enter. this is the updated option[] array sent which was changed in the above function
	return menu_selection_choice;

//	up_down_menu_selection as shown above keeps being called while the user presses up or down
*/
up_down_menu_selection(char option[][40], int *menu_selection_choice, int limit)
{	
	char char_enter;
	while(1)
	{
		char_enter = getch();/*wait for key press*/
		
		if ((GetAsyncKeyState(VK_UP) & 0x8000 != 0) && *menu_selection_choice > 0) //if up key is pressed in menu, char '>' moves upwards
		{
			option[*menu_selection_choice][0] = ' ';
			*menu_selection_choice -= 1;
			option[*menu_selection_choice][0] = '>';
			return 99;
		}
		
		else if ((GetAsyncKeyState(VK_DOWN) & 0x8000 != 0) && *menu_selection_choice < limit)//if down key is pressed in menu, char '>' moves downwards
		{			
			option[*menu_selection_choice][0] = ' ';
			*menu_selection_choice += 1;
			option[*menu_selection_choice][0] = '>';
			return 99;
		}

		else if(char_enter == 13)/*enter is pressed*/
				return *menu_selection_choice;

	}
}

/*function called if 1 or 2 player option pressed in main menu. int flag etells whether it was called by 1 player or two player*/
snake_any_player_before_game(int flag)
{	
	char option[][40] = {"  Go Back to main menu" , "> Choose your snake & background colour" , "  Choose your starting speed", "  Choose your map",  "  Choose to play a timed round", "  Play"};
	/*options to be displayed to user*/
	char maze[100][100];/*to store a mpa if user picks it*/
	int selection = 0;/*for map selection*/
	
	int option_speed[] = {65000, 40000, 30000}; /*sending for variable speed. tells wait time for snake in usleep() function*/
	int snake_speed = 1;/*which option in the array option_speed[] was picked. 1 is default*/
	int var_speed = 0;/*whether variable speed is on or not*/

	int snake_color = -1, back_color = 0, snake_2_color = -1;/*snakecolor -1 is default light blue. back_color 0 is black*/
	
	int time_val = 0;/*which option in the array time_array was picked for the round time limit. 0 is default and means no time limit*/
	int time_array[4] = {0 , 45 , 60 , 120};/*options for time limit in round*/
	
	int menu_selection_choice = 1, limit = 5;/*menu_selection_choice = 1 tells where the arrwo is pressed by default. limit 5 tells the max index number of char options array*/
	
	do
	{
		printing_snake_menu_any_player(option, flag);/*print the initial option screen*/
		
		while(up_down_menu_selection(option, &menu_selection_choice, limit) == 99)/*wait for user to press up or down or enter*/
					printing_snake_menu_any_player(option, flag);/*reprint option screen with the changed array it the user did not press enter*/

		switch(menu_selection_choice)/*switch that runs when user presses enter in the menu above*/
		{
			case 0:/*go back. does nothing and user goes to main screen*/
				break;
				
			case 1: /*snake and background color*/
				color_screen(&snake_color, &back_color, &snake_2_color , flag);				
				break;
				
			case 2: /*snake speed*/
				snake_speed = snake_speed_func(snake_speed, option_speed, &var_speed); 
				break;
				
			case 3:/*choose map option. selection is a boolean that tells whether a map was opened or not. it turns one or true here, and is passed to snake game function.*/
				selection = 1;
					open_map(maze, 2);
				break;
				
			case 4:/*timed round option*/
				time_val = time_func(time_val);
				break;
				
			case 5:/*play now. uses flag to call different funcions*/
				if (flag == 1)
					snake_1_player(maze, selection, snake_color, back_color, snake_speed, option_speed, var_speed, time_array[time_val]);
				
				else 
					snake_2_player(maze, selection, snake_color, snake_2_color ,back_color, snake_speed, option_speed, var_speed, time_array[time_val]);
				break;
		}

	}while(menu_selection_choice != 0 && menu_selection_choice != 5); /*keep coming back to this menu until user presses play or go back. if play si pressed then return
	to main screen after user has played his game.*/
	
}

/*simple menu that prints options*/
printing_snake_menu_any_player(char option[][40], int flag)
{
	system("cls");
	int i;
	printf("\n\t\t\tSNAKE %d PLAYER", flag);
	printf("\n\n\n");
	
	for (i = 0; i < 6; i++)
		printf("\n\n\t\t\t%s", option[i]);	
}

/*function for implementing timed rounds*/
time_func(int menu_selection_choice)
{
	int limit = 3;
	char option[][40] = {"  No time limit", "  45 seconds", "  60 seconds", "  120 seconds"};/*array to modified and printed*/
	/*see up_down_menu_selection function for explanation*/	
	
	option[menu_selection_choice][0] = '>';/*where the arrow is according to previous or default time choice*/
	printing_time_menu(option);

	while(up_down_menu_selection(option, &menu_selection_choice, limit) == 99)
		printing_time_menu(option);

	return menu_selection_choice; /*the index number of the time choice selected is returned to the parent function*/
}

/*function that prints array*/
printing_time_menu(char option[][40])
{
	int i;
	system("cls");
	printf("\n\t\t\t     Timed round selection\n\n\n");
	
	for (i = 0; i < 4; i++)
		printf("\n\t\t\t      %s\n", option[i]);
}

/*function called when user wants to pick snake speed. receives variable snake speed from function that tells curernt speed choice, option_speed array which has different wait values for game 
usleep() function which causes snake to have different speeds. int *var speed is a flag that tells whether variable speed is on or not.*/
snake_speed_func(int menu_selection_choice, int option_speed[], int * var_speed)
{
	char option[][100] = {"  Baby Snake (The slowest. For those who play it safe)", "  Snake (Not slow, not fast, but steady.)" , "  Black Mamba (The fastest. Get ready for a wild ride)", "  Variable Speed"};
	char option_var[][20] = {"  No", "  Yes"};/*two char arrays to print in the menu. one deals with snake speed choice and the other with variable speed choice*/
	int limit = 3;/*max index number in option array[]*/
	
	
	/*the below key_ ints are flags used in the different functions*/
	int key_up = 0, key_down = 0, key_left = 0, key_right = 0;
		
	option[menu_selection_choice][0] = '>';	/*where the current arrow is according to the current snake speed choice*/
	option_var[*var_speed][0] = '+'; /*where the current + is according to the current var speed choice*/
	speed_printing_menu(option, option_speed, menu_selection_choice, &key_up, &key_down, &key_left, &key_right, *var_speed, option_var, 0);
	/*a variant of up_down_menu_selection function, please see that specific function for more details. this function also deals with flags for keystrokes because of its necessity.*/
	while(up_down_menu_selection_large(option, &menu_selection_choice, limit, &key_up, &key_down, &key_left, &key_right, var_speed, option_var) == 99)
		speed_printing_menu(option, option_speed, menu_selection_choice, &key_up, &key_down, &key_left, &key_right, *var_speed, option_var, 1);

	if (menu_selection_choice == 3)/*if user pressed enter when on variable speed choice*/
		menu_selection_choice--;/*turn his choice to highest speed*/
	return menu_selection_choice;
}

speed_printing_menu(char option[][100], int option_speed[], int menu_selection_choice, int *key_up, int *key_down, int *key_left, int *key_right, int var_speed, char option_var[][20], int flag)
{/*receives array to print, of snake speed and variable speed. four keystroke flag pointers, and a flag for whther it was called from 1 or 2 player. keystroke flags are here because this function
shows the movement of snake when you hover over an option. normal up_down_menu_selection function depends on getch(), which can not work here, as the snake is running on an infinite loop.
keyflags are here so that when a person presees a key while snake is running, the snake lopo exits and the keystoke pressed is passed as a true flag to another function where the array is changed.
The changed array is displayed here again. for more details on snake specific functions and their necessity for moving snake, see snake_1_player or snake_2_player*/
	system("cls");
	printf("\n\t\t\tSPEED SELECTION");
	int i, move = 0;
	int snakepos[][2] = {{20, 14}, {20, 13}, {20, 12}, {20, 11}, {20, 10}};/*starting snake position*/
	int last[1][2], exit = 0;/*tail coordinates of snake. for more details see snake_1_player or snake_2_player*/
	char maze[40][100];/*2d char array for snake*/
	*key_up = 0; /*keystroke flags*/
	*key_down = 0;
	*key_left = 0; 
	*key_right = 0;


	for(i = 0; i < 4; i++)
		printf("\n\n   %s", option[i]);
	/*print options*/
	
	printf("\t%s\t%s", option_var[0], option_var[1]);
	
	printf("\n\n Turning variable speed on will steadily increase the speed of your snake after\n you have gathered 10 points upto the maximum speed possible.");

	GoToXY(20,10);/*go to starting position for snake*/	

	while(exit == 0)/*run infinite loop*/
	{
		snake_tail_update(last, snakepos, 5); /*stores the position of the snake tail so that it can be cleared later*/

		snakepos_update(snakepos, 5);/*updates snake coordinates*/
	
		if (move == 0 && snakepos[0][1] < 63)/*if snake is moving right and has not yet reached right boundary*/
			snakepos[0][1] += 1;/*uincrement column and move snake right*/
		
		else if (move == 1 && snakepos[0][1] > 15)/*if snake is moving left and has not yet reached left boundary*/
			snakepos[0][1] -= 1;/*decrement column and move snake left*/
		
		else if(snakepos[0][1] == 63)/*if snake is on right boundary*/
			{
				snakepos[0][0] -= 1;/*move it one row up*/
				move = 1;/*turn it left*/
			}
			
		else if(snakepos[0][1] == 15)/*if snake is on left boundary*/
		{
			snakepos[0][0] += 1;/*move it one row down*/
			move = 0;/*turn it right*/
		}
					
		snake_movement_coordinates_update(maze, last, snakepos); /*update maze with snake coordinates*/

		if (menu_selection_choice < 3)/*wait for the amount of time that the arrow is hovering on in options.*/
			usleep(option_speed[menu_selection_choice]);
		
		else/*if arrow is on variable speed, move snake at fast speed/ wait minimum time*/
			usleep(option_speed[2]);

		if ((GetAsyncKeyState(VK_UP) & 0x8000 != 0))/*if up is pressed*/
		{
			exit = 1;/*exit loop*/
			*key_up = 1;/*turn up keystroke flag true*/
		}
		
		if ((GetAsyncKeyState(VK_DOWN) & 0x8000 != 0))/*if down is pressed*/
		{
			exit = 1;/*exit loop*/
			*key_down = 1;/*turn down keystroke flag true*/
		}			

		if ((GetAsyncKeyState(VK_LEFT) & 0x8000 != 0) && (menu_selection_choice == 3))/*if left is pressed and arrow is on variable speed choice*/
		{
			exit = 1;/*exit loop*/
			*key_left = 1;/*turn left keystroke flag true*/
		}
		
		if ((GetAsyncKeyState(VK_RIGHT) & 0x8000 != 0) && (menu_selection_choice == 3))/*if right is pressed and arrow is on variable speed choice*/
		{
			exit = 1;/*exit loop*/
			*key_right = 1;/*turn right keystroke flag true*/
		}
		
		if ((GetAsyncKeyState(VK_RETURN) & 0x8000 != 0) && flag == 1 )/*if enter is pressed*/
			exit = 1;/*exit loop*/

	}

}
/*specific up_down_menu_selection function for snake speed menu*/
up_down_menu_selection_large(char option[][100], int *menu_selection_choice, int limit, int *key_up, int *key_down, int *key_left, int *key_right, int *var_speed, char option_var[][20])
{	/*larger char option value for array*/
	char char_enter;
	while(1)
	{
		
		if(*key_up == 0 && *key_down == 0 && *key_right == 0 && *key_left == 0)/*if no arrow key was pressed, ask for user input*/
			char_enter = getch();
				
		if (((GetAsyncKeyState(VK_UP) & 0x8000 != 0) || *key_up == 1) && *menu_selection_choice > 0) /*if up key is pressed in menu, char '>' moves upward. now also works if flag is true*/
		{
			option[*menu_selection_choice][0] = ' ';
			*menu_selection_choice -= 1;
			option[*menu_selection_choice][0] = '>';
			return 99;
		}
		
		else if (((GetAsyncKeyState(VK_DOWN) & 0x8000 != 0) || *key_down == 1) && *menu_selection_choice < limit)/*if down key is pressed in menu, char '>' moves downwards. now also works if keystroke flag is true*/
		{			
			option[*menu_selection_choice][0] = ' ';
			*menu_selection_choice += 1;
			option[*menu_selection_choice][0] = '>';
			return 99;
		}
	
		/*if left key is pressed in menu, char '+' moves left. now also works if keystroke flag is true. current arrow must be on variable speed also. *var_speed must be 1, so that it can be 0 index number now. */
		else if (((GetAsyncKeyState(VK_LEFT) & 0x8000 != 0) || *key_left == 1) && *menu_selection_choice == 3 && *var_speed > 0)
		{			
			option_var[*var_speed][0] = ' ';
			*var_speed -= 1;
			option_var[*var_speed][0] = '+';
			return 99;
		}

		/*if right key is pressed in menu, char '+' moves right. now also works if keystroke flag is true. current arrow must be on variable speed also. *var_speed must be 0, so that it can be 1 index number now. */
		else if (((GetAsyncKeyState(VK_RIGHT) & 0x8000 != 0) || *key_right == 1) && *menu_selection_choice == 3 && *var_speed < 1)
		{	
			option_var[*var_speed][0] = ' ';
			*var_speed += 1;
			option_var[*var_speed][0] = '+';
			return 99;
		}

		else if(char_enter == 13)/*if enter is pressed, return the value of menu _selection_shoice to parent function. for more details, see up_down_menu_selection funciton*/
				return *menu_selection_choice;
			
	}
}

/*function to pick the color of snake and background. *text_color is for snake 1, *back_color is for background, *snake_2_color is for snake 2, and flag is for whether this was called from single or 2 player*/
color_screen(int *text_color, int *back_color, int *snake_2_color, int flag)
{
	char option[][40] = {"> Snake Color", "  Snake 2 Color", "  Background Color"};
	char option_snake[][40] = {"  Default(Light Blue)", "  Red", "  Green", "  Blue"};
	char option_snake_2[][40] = {"  Default(Light Blue)", "  Red", "  Green", "  Blue"};
	char option_back[][40] = {"  Default(Black)", "  White"};
	
	int limit = flag;
	int menu_selection_choice = 0;
	
	option_snake[*text_color + 1][0] = '+';/*putting + according to previous choice*/

	option_snake_2[*snake_2_color + 1][0] = '+';/*putting + according to previous choice*/
		
	option_back[*back_color][0] = '+';/*putting + according to previous choice*/
	
	color_printing_menu(option, option_snake, option_back, text_color, back_color, snake_2_color, option_snake_2,flag);
	/*a variant of up_down_menu_selection function*/
	while(color_menu_selection(option, option_snake, option_back, &menu_selection_choice, limit, text_color, back_color, snake_2_color, option_snake_2,flag) == 99)
		color_printing_menu(option, option_snake, option_back, text_color, back_color, snake_2_color, option_snake_2, flag);

}

/*prints the menu. receives multiple arrays, and addresses of *text_color, *back_color and *snake_2_color, so that any changes here will be reflected in parent menu. flag indicates whether it was called
from 1 or 2 player*/
color_printing_menu(char option[][40], char option_snake[][40], char option_back[][40], int *text_color, int *back_color, int *snake_2_color, char option_snake_2[][40], int flag)
{
	int i, j;
	colorfunc(-1 , 0);
	system("cls");/*color and screen reset*/
	printf("\n\t\t\tColor Selection\n\n");
	printf("  %s\t\t%s\t%s\t%s\t  %s\n\n", option[0], option_snake[0], option_snake[1], option_snake[2], option_snake[3]);
		
		/*print menu, according to flag*/
	if (flag == 2)
		printf("  %s\t%s\t%s\t%s\t  %s\n\n", option[1], option_snake_2[0], option_snake_2[1], option_snake_2[2], option_snake_2[3]);

	printf("  %s  \t%s\t%s", option[2], option_back[0], option_back[1]);
	
	colorfunc(*text_color, *back_color);/*change color acccording to 1 snake*/
	int row = 10, col = 30, x, y;/*intitialize row and columns to print snake*/
	GoToXY(col , row);/*go to that coordinate*/
	for(x = 0; x < 5; x++)/*print first snake*/
	{
		for(y = 0; y < 17; y++)
			{
				if (x == 2 && y > 5 && y < 8)
				{
					printf("====X");
					y += 4;
				}
				
				else
					printf(" ");
			}
			row++;
		GoToXY(col, row);
	}

	if (flag == 2)/*if function called from two player*/
	{	
		colorfunc(*snake_2_color, *back_color);/*change console cursor color to that of snake 2 choice*/
		
		for(x = 0; x < 5; x++)/*print second snake*/
		{
			for(y = 0; y < 17; y++)
				{
					if (x == 2 && y > 5 && y < 8)
					{
						printf("X====");
						y += 4;
					}
					
					else
						printf(" ");
				}
				row++;
			GoToXY(col, row);
		}
	}


	colorfunc(-1 , 0);/*reset color*/
}

/*variant of up_down_menu_selection function. for more details see that funciton*/
color_menu_selection(char option[][40], char option_snake[][40], char option_back[][40], int *menu_selection_choice, int limit, int *snake_color, int *back_color, int *snake_2_color, char option_snake_2[][40],int flag)
{	
	char char_enter;
	int option_snake_limit = 3, option_back_limit = 1;
	
	while(1)
	{
		char_enter = getch();
		
		if ((GetAsyncKeyState(VK_UP) & 0x8000 != 0) && *menu_selection_choice > 0) //if up key is pressed in menu, char '>' moves upwards
		{
			option[*menu_selection_choice][0] = ' ';
			*menu_selection_choice -= 1;
			
			if(flag == 1 && *menu_selection_choice == 0)/*if for one player, skip first row or the option for second snake*/
				{
					option[2][0] = ' ';
					option[0][0] = '>';
				}
				
			else 	
				option[*menu_selection_choice][0] = '>';
			
			return 99;
		}
		
		else if ((GetAsyncKeyState(VK_DOWN) & 0x8000 != 0) && *menu_selection_choice < limit)//if down key is pressed in menu, char '>' moves downwards
		{			
			option[*menu_selection_choice][0] = ' ';
			*menu_selection_choice += 1;
			
			if(flag == 1 && *menu_selection_choice == 1) /*for single player, skip first row or option for second snake*/
				option[2][0] = '>';
				
			else 	
				option[*menu_selection_choice][0] = '>';
			
			return 99;
		}

		else if ((GetAsyncKeyState(VK_LEFT) & 0x8000 != 0))//if left key is pressed in menu, char '+' moves left
		{			
			if (*menu_selection_choice == 0 && *snake_color > -1)/*where the arrow was pointing, and if the limit has not been reached*/
				{
					option_snake[*snake_color + 1][0] = ' ';/*change array of first player if arrow was pointed there*/
					*snake_color -= 1;
					option_snake[*snake_color + 1][0] = '+';
				}
		
			if (*menu_selection_choice == 1 && flag == 2 && *snake_2_color > -1)
				{
					option_snake_2[*snake_2_color + 1][0] = ' ';/*change array of second player if arrow was pointed there*/
					*snake_2_color -= 1;
					option_snake_2[*snake_2_color + 1][0] = '+';
				}
				
			else if (*menu_selection_choice == flag && *back_color > 0)
				{
					option_back[*back_color][0] = ' ';/*change array of background if arrow was pointed there*/
					*back_color -= 1;
					option_back[*back_color][0] = '+';
				}
					return 99;
		}
	
		else if ((GetAsyncKeyState(VK_RIGHT) & 0x8000 != 0))//if right key is pressed in menu, char '+' moves right,
		{	
			if (*menu_selection_choice == 0 && *snake_color < 2)
				{
					option_snake[*snake_color + 1][0] = ' ';/*change array of first player if arrow was pointed there*/
					*snake_color += 1;
					option_snake[*snake_color + 1][0] = '+';
//					return 99;
				}
				
			if (*menu_selection_choice == 1 && flag == 2 && *snake_2_color < 2)
				{
					option_snake_2[*snake_2_color + 1][0] = ' ';/*change array of second player if arrow was pointed there*/
					*snake_2_color += 1;
					option_snake_2[*snake_2_color + 1][0] = '+';
//					return 99;
				}
					
			else if (*menu_selection_choice == flag && *back_color < 1)
				{
					option_back[*back_color][0] = ' ';/*change array of background if arrow was pointed there*/
					*back_color += 1;
					option_back[*back_color][0] = '+';
				}
					return 99;
		}
	
		
		else if(char_enter == 13)
			{
				return *menu_selection_choice;
			}
	
		
	}
}

/*simple function to clear the whole maze of any snake body parts like X or = while leaving the obstacle present and printing them as they were*/
clear_game_maze_retry(char maze[][100])
{
	int i, j, sizerow = 24, sizecol = 79;
	GoToXY(0,0);
	for (i = 0; i < sizerow; i++)
	{
		for(j = 0; j < sizecol; j++)
			{
				if (maze[i][j] == '=' || maze[i][j] == 'X' || maze[i][j] == '*')
					maze[i][j] = ' ';
			
				printf("%c", maze[i][j]);
			}
			
		printf("\n");
	}
	
	
}
														/*Start of help screen functions*/

help_options()
{	/*print help screen*/
	system("cls");
	
	printf( "          Player 1 Controls               Player 2 Controls \n"
			"              _______		              _______\n"  
			"              |     |                         | /|\\ |                \n"
			"	      |  W  |               	      |  |  |               \n"
			"	 _____|_____|______          	 _____|_____|______          \n"      
			"	|     |     |     |         	|     |  |  |     |         \n"           
			"	|  A  |  S  |  D  |         	| <-- | \\|/ | --> |         \n"
			"	|_____|_____|_____|         	|_____|_____|_____|         \n"                                                      );     
                                          
    
    
    printf("\n\nYou can pick a number of options, from choosing your own map to choosing your \nsnake color, speed, to playing a timed rounds. Experiment and play different \ncustomized games!\n\n");
    printf("The objective of two player is to get the greatest number of points before time runs out, or to make your opponent crash. Either will net you the victory!\n\n");
    printf("Map editor is a powerful tool to make your own maps. Let your creativity shine and make new maps to play in!");
	printf("\n\nPress any key to continue...");
	getch();         /*wait for user input to exit*/                        
                                          
                                          
}

about_screen()
{
		system("cls");
	printf("\n\t\t\t\tABOUT SCREEN\n\n");
	
	printf(" This snake game was made as a project by a group of four FASTian freshmen. \nThe team consisted of: \n\n"
			" Uzair Khan\t\t ID: K15-2183\t (Group Leader)\n\n"
			" Hammad Jafri\t\t ID: K15-2194\t \n\n"
			" Usama Waseem\t\t ID: K15-2362\t\n\n"
			" Hamza Mustafa\t\t ID: K15-2832\t\n\n");
			
	printf(" We would also like to thank our profressor, Sir Nauman, and our mentor, \n Jazib for their help.");
	printf("\n\n We hope you enjoy the game.");
	
	printf("\n\n Press any key to continue...");
	getch();         /*wait for user input to exit*/                        

}
													
													/*Start of map editor functions*/
													
													
editor_or_open()
{/*called from main menu when user presses map editor*/
	int menu_selection_choice = 0, limit = 3;
	char option[][40] = {">  Open saved map", "   Make a new map", "   Help & Controls", "   Go back"};
	
	printing_editor_menu(option);
		/*for more details, see up_down_menu_selection function*/
	while(up_down_menu_selection(option, &menu_selection_choice, limit) == 99)
			printing_editor_menu(option);

	char maze[100][100];/*char array where maps opened will be stored*/

	switch(menu_selection_choice)/*which function to call based on above selection*/
	{
		case 0:
			open_map(maze, 1);/*calls open map function and sends this maze array. the 1 indicates this function was called from the editor*/
			break;
			
		case 1:
			map_editor(maze , 1);/*calls map editor function and sends this maze array. the 1 indicates this function was called from the editor*/
			break;
			
		case 2:
			map_help_screen();/*calls map help screen*/
			break;
	}

}

printing_editor_menu(char option[][40])/*prints map editor screen*/
{
	system("cls");
	int i;
	
	printf("\n\n\t\t\t\tMAP EDITOR\n\n\n\n");
	
	for(i = 0; i < 4; i++)
		printf("\t\t\t   %s\n\n", option[i]);

}

map_help_screen()
{/*help screen that tells all controls for map editor*/
	system("cls");
	
	printf( "      	      _______		        \n"  
			"Press         |     |        to move the cursor.\n"
			"	      |  W  |               \n"
			"	 _____|_____|______          \n"      
			"	|     |     |     |         \n"           
			"	|  A  |  S  |  D  |         \n"
			"	|_____|_____|_____|         \n");     
                                          
    printf ("	 _____         _____           \n"      
			"Press	|     |       |     |   to change the structure.      \n"           
			"	|  J  |  and  |  L  |         \n"
			"	|_____|       |_____|         \n");     
                                          
	printf( "      	 _______		        \n"  
			"Press    |     |    to save the current structure in the map.\n"
			"	 |  K  |               \n"
			"	 |_____|       \n");      
                                          
	printf( "      	 _______		        \n"  
			"Press    |     |    to save the current map. Everytime you press this button, \n"
			"	 |  I  |         the map in the editor is saved under a new name.      \n"
			"	 |_____|       \n");      
			
	printf( "      	 _______		        \n"  
			"Press    |     |    to exit the editor.\n"
			"	 |  Y  |               \n"
			"	 |_____|       \n");      

	printf("Press any key to go back to the previous menu...");
	getch();/*wait for user input to exit*/
	editor_or_open();/*call its parent function again*/
}

map_editor(char maze[][100], int saved_or_new)
{	/*amze is a 2d char array that is passed from previous function. it either contains a previous map structure if called from open
map function, or is completely blank if called from make a new map option in map editor menu. saved or new tells whether function was called from open previous map
or make a new map*/

	int row = 10, col = 10;/*where to start generating structures/obstacles in the console*/
	int sizerow = 24, sizecol = 79;/*restraints of console window*/
	
	if (saved_or_new == 1)/*if new map, then use printing maze funciton to generate a blank map*/
		printing_maze(maze, sizerow, sizecol);
	
	FILE *structures;/*create a file pointer to read structures from txt fule*/
	structures = fopen("maps\\\\structures.txt" , "r");/*open structures.txt file in the maps filder*/
	char structure_array[10][50];/*make this array to store the read structures*/
	char file_character;

	file_character = fgetc(structures);/*the character read from the file with the fgetc funciton will be stored in the char file_character*/
	int char_array_row = 0, char_array_col = 0, array_struct_number = 0;

	while(file_character != EOF)/*keep reading file until end of file has not been encountered*/
	{
		structure_array[char_array_row][char_array_col++] = file_character;/*store all characters in this row of the array*/
		file_character = fgetc(structures);/*read a new character*/

		if(file_character == '1')/*if new character read is 1, it means that the previous structure read has ended. a new structure is about to be read*/
		{
			char_array_row++;/*increase the row/index number where the characters are being stored*/
			char_array_col = 0;/*reset this count to zero*/
			file_character = fgetc(structures);/*read the character again to skip 1*/
		}
	}
	fclose(structures);/*close the file pointer after file has been read*/

	char ch;
	
	while(ch != 'y')/*loop to contorl the movement of structures*/
	{	
		ch = getch();/*wait for user input*/

		if (ch == 'd') //if d key is pressed
			col++;/*increase col to make the structure move towards the right*/
		
		else if (ch == 'a') //if left key is pressed
			col--;/*decrease col to make the structure move towards the left*/
		
		else if (ch == 'w') //if up key is pressed
			row--;/*decrease the row to make the structure move upwards*/
		
		else if (ch == 's') //if down key is pressed 
			row++;/*increase the row number to make the structure move downwards*/
		
		
		else if (ch == 'j' && array_struct_number > 0)/*if j is presesed and this is not the first structure in the array*/
			array_struct_number--;/*decrease this int to show structure in the previous index number*/
		
		else if (ch == 'l' && array_struct_number < char_array_row)/*if l is presesed and this is not the last structure in the array*/
			array_struct_number++;/*increase this int to show structure in the next index number*/
		
		else if (ch == 'k')/*if l is pressed, call this funciton to enter this structure into the maze*/
			structure_enter(maze, col, row, structure_array[array_struct_number]);/*parameters are the maze array, the col and row cursor position, and the structure array to be entered*/			
		
		else if (ch == 'i')/*if i is pressed, call this function to save the current map*/
			savemap(maze);/*parameters are only the current maze*/
		
		
		clear_screen_no_flickering(maze);/*reprint the screen wxcept the boundaries so that flickering does not occur*/

		pattern_printing(structure_array[array_struct_number], col, row);/*print the specific pattern onto the screen*/

	}
	
}
/*parameters are the structure to be printed on the screen, at the col and row position of the cursor. only prints the structure */
pattern_printing(char array_of_struct[50] , int col, int row)
{
	int i;
		GoToXY(col, row); /*goes to the col row postiion of the cursor*/

	for (i = 0; array_of_struct[i] != '\0'; i++)/*starts a loop to print the structure at that position*/
	{
			if(i == 0 && array_of_struct[i] == '\n')/*if it is the first character and is a new line, then skip this iteration of the loop*/
				continue;
				
			printf("%c", array_of_struct[i]);/*print the character*/
			
			if(array_of_struct[i+1] == '\n')/*if the next character is a newline*/
			{	row++;/*increment the row*/
				GoToXY(col, row);/*go to the new row and column position*/
				i++;/*increment i. now array_of_struct[i] == n. when i is incremented again in the loop condition above, the newline will be skipped*/
			}
		
	}
}/*endof pattern printing*/

/*function to clear the screen without clearing the boundaries. reduces flickering*/
clear_screen_no_flickering(char maze[][100])/*recieves the maze array as parameter*/
{
	int i, j;
	int sizerow = 24, sizecol = 79;/*restraints of console windows*/
	
	for(i = 1; i < sizerow - 1; i++)
	{	
		GoToXY(1, i);/*starts with first column first row of maze. 0 row and 0 column is boundary of maze, as well as sizerow row and sizecol column*/
	
		for(j = 1; j < sizecol - 1; j++)/*start from 1 uptil the column before right hand boundary*/
		{	
			if(maze[i][j] == ' ')/*if maze was clear then reprint space on it*/
				printf(" ");
				
			else/*otherwise print whatever was on it*/
				printf("%c", maze[i][j]);
		}/*this function works because the structures shown on the screen during map editor are simply printed, they arent entered into the maze, so this funciton sees the structures
		if the console structure, it sees them as blank spaces and clears them, structures which have been enteredalter the maze[][] values, so they are reprinted*/
	}
}

/*function to enter the structures in the maze. parameters are maze, row and col positions of the cursor, and the specific structure array to be entered*/
structure_enter(char maze[][100], int col, int row, char array_of_struct[50])
{
		int i, j = col;

		GoToXY(col, row);/*go to the postition where the cursor was*/

	for (i = 0; array_of_struct[i] != '\0'; i++)/*run this lopo until terminatin character is not encountered*/
	{
			if(i == 0 && array_of_struct[i] == '\n')/*if first character is a newline then skip this iteration of the loop*/
				continue;
		
			maze[row][j++] = array_of_struct[i];/*edit the maze value of this coordinate with the structure character*/
			
			if(array_of_struct[i+1] == '\n')/*if the next character in the array is a newline*/
			{	row++;/*increment the row number*/
				GoToXY(col, row);/*go to the updated coordinates, this is directly below the original cursor position*/
				i++;/*increment i so that this newline character will be skipped when i is incremented again above*/
				j = col;/*reset j to the col value so that the correct coordinate can be entered in the maze array*/
			}
	}

		GoToXY(20, 24);/*go to the bottom of the screen to enter this message*/
			printf("Structure entered.");
	
	sleep(1);/*wait for a moment*/
	
	print_maze_simple(maze);/*reprint the maze*/
}

/*reprints the entire maze*/
print_maze_simple(char maze[][100])
{
	system("cls");
		
	int sizerow = 24, sizecol = 79;
	int i, j;

	for (i = 0; i < sizerow; i++)
	{
		for ( j = 0; j < sizecol; j++)
			printf("%c", maze[i][j]);

		printf("\n");
	}
}

/*function that is called when entire maze needs to be saved. saves maze in a newfile. in the map directory there are multiple files. most of them are files which contain the maps.
one file is the maplist.txt file which contains the names of all the maps. another funciton reads that txt file to gather all of the names of the maps that exist now. using that,
a new map name is generated by seeing the map00x name, and incrementing the x value so that the map name is 1 above the previous one. for example, if the latest map stored was named map009.txt, then 
the new one will be named map010.txt. this is implememted by using strtol function and incrementing the int value obtained, thein incrementing it and using strcat to create a new map name.
once the new map is generated, the map is saved into that file,, and maplist.txt isupdated to reflect the new map list.*/
savemap(char maze[][100])
{
	char array_map_names[100][40];/*srray to store the map names*/
	int array_row = map_dir_read(array_map_names);/*calls function to read maplist.txt. for explanation see above. arroay_row returned tells max number of maps.*/
	char newmap[40], new_map_final[40];/*maketwo new arrays*/
	strcpy(newmap, array_map_names[array_row]);/*copy the name of the last map into the newmap array*/
	int i, j;
	
	/*before this loop runs, the array conttains an example "> map005.txt". after this loop runs the first 5 characters are omitted. the new string is now 
	"005.txt", which is a valid string to be used in strtol below*/
	for(i = 0; newmap[i] != '\0'; i++)
		newmap[i] = newmap[i+5];
	
	int map_num = strtol(newmap, NULL, 10);/*strtol converts a valid string containing number chararcters to an int calue. parameters are the string, 
	the character at which to stop, which I've given NULL, and the base number system wo use, which i've given 10 as the decimal system. the int value is returned. here
	it is sotred in map_num*/
	map_num++;/*the value is incremented for storing the new map name*/
	sprintf(new_map_final, "maps\\\\map%03d.txt", map_num);/*sprintf writes a formatted string into a target array. here i have written the string  which contains the new map directory
	and map name into the new_map_final directory. parameters of the function are the array where the string will be stored , and the formatted string to be stored*/

	FILE *save, *maplist_append;/*make two file pointers.one for saving new map file, another for appending the maplist.txt file*/
	
	save = fopen(new_map_final, "w");/*make a new file with the string in the new_map_final array. as shown above, it contains the new file name with the directory address. this file will be created using w mode*/
	
	int sizerow = 24, sizecol = 79;/*restraints of console windows*/
		
	for (i = 0; i < sizerow; i++)/*print the entire map array into the file*/
	{
		for(j = 0; j < sizecol; j++)
		 fprintf(save, "%c", maze[i][j]);
		 
		fprintf(save, "\n");
	}
	
	fclose(save);/*close the file*/
	
	/*new_map_final array contained a map name like "maps\\\\map003.txt". this loop removes the first five characters. remember that while there are two backslash characters, it is an 
	escape sequence so only one will be counted. the result of this loop is that the array new_map_final contains the string "map003.txt"*/
	for(i = 0; new_map_final[i] != '\0'; i++)
		new_map_final[i] = new_map_final[i+6];
		
	maplist_append = fopen("maps\\\\maplist.txt", "a");/*open the maplist.txt file for editing*/
	
	fprintf(maplist_append, "\n%s", new_map_final);/*add this new map name into the end of the map list txt file*/
	
	fclose(maplist_append);/*close the file pointer*/
	
	GoToXY(20, 24);/*go to bottom of screen to print the map name where the current map is saved*/
	printf("Map saved as %s.", new_map_final);
	sleep(1);/*wait for a second*/
	
	print_maze_simple(maze);/*reprint the entire screen*/

}

/*this function opens a map. can be called from map editor menu or snake__any_player_before_game function to pick a map for playing. parameters are the map where the enw map will be
stored, as well as a flag whcih indicates from where the function was called*/
open_map(char (*map_ptr)[100], int preview_editor_game)
{
	char array_map_names[100][40];/*makes a new array fro storing map names*/
	int array_row = map_dir_read(array_map_names);/*reads map names from maplist.txt using map_dir_read function. max map number is returned*/
	int after_preview = preview_editor_game;/**/
	int p_toggle = 0;/*flag which indicates whether preview is enabled or not*/
	int menu_selection_choice = 0, limit = array_row;

	printing_map_menu(array_map_names, array_row, &p_toggle);/*prints map menu*/

	do
	{
	/*special version of up_down_menu_selection function. includes an extra parameter to know whether preview is enabled or not.*/
	while(up_down_menu_selection_preview(array_map_names, &menu_selection_choice, limit, &p_toggle) == 99)
		printing_map_menu(array_map_names, array_row, &p_toggle);
	
	if (p_toggle == 1)/*if preview was enabled before enter was pressed*/
		preview_editor_game = 0;/*change the flag value*/
		
	else/*otherwise reassign the original value. this statement exists because this entire finction employs a do while loop, and the value of preview_editor_game can change multiple times*/
		preview_editor_game = after_preview;
		
	map_selection(array_map_names[menu_selection_choice], preview_editor_game, map_ptr);/*sends map name to function to open*/
	}while(preview_editor_game == 0);/*keep this lopo repeating while the map sent to view was under preview condition*/
}

/*see up_down_menu_selection function for details. only thing changed here is a condition for pressing space was added*/
up_down_menu_selection_preview(char option[][40], int *menu_selection_choice, int limit, int * p_toggle)
{	
	char char_enter;
	while(1)
	{
		char_enter = getch();
		
		if ((GetAsyncKeyState(VK_UP) & 0x8000 != 0) && *menu_selection_choice > 0) //if up key is pressed in menu, char '>' moves upwards
		{
			option[*menu_selection_choice][0] = ' ';
			*menu_selection_choice -= 1;
			option[*menu_selection_choice][0] = '>';
			return 99;
		}
		
		else if ((GetAsyncKeyState(VK_DOWN) & 0x8000 != 0) && *menu_selection_choice < limit)//if down key is pressed in menu, char '>' moves downwards
		{			
			option[*menu_selection_choice][0] = ' ';
			*menu_selection_choice += 1;
			option[*menu_selection_choice][0] = '>';
			return 99;
		}

		else if(char_enter == 13)
			return *menu_selection_choice;
			
		
		else if(char_enter == ' ' || char_enter == ' ')
			{
				*p_toggle = (*p_toggle + 1) % 2;
				return 99;
			}
			
	}
}

/*simply prints the map menu*/
printing_map_menu(char array_map_names[][40], int array_row, int * p_toggle)
{ 
	system("cls");
	int i, j;
	printf("\n\t\t\tMap Selection menu\n\n\n");
	
	if (*p_toggle == 0)
	{
		printf("\t\tPress Space to enable preview mode.\n\n");
		printf("\t\tPreview mode : Disabled\n\n\n");
	}
	
	else
	{
		printf("\t\tPress Space to disable preview mode.\n\n");
		printf("\t\tPreview mode : Enabled\n\n\n");
	}
			
	for(i = 0; i < array_row + 1; i++)
		printf("\t\t\t  %s\n", array_map_names[i]);
}

/*function to read maps\\\\maplist.txt. parameter is 2d char array where map names will be stored*/
int map_dir_read(char array_map_names[][40])
{
	FILE *read;/*file pointer to read file*/
	read = fopen("maps\\\\maplist.txt", "r");
	char filechar;
	int array_row = -1, j = 0;/*array_row -1 indicates that there are no map names right now. got this idea from stack where initially top is -1*/
	
	filechar = fgetc(read);/*read the first character of file*/
	
	while(filechar != EOF)/*keep reading file until end*/
	{
		if (array_row == -1)/*array_row tells the max index number in which maps are stored. -1 tells no maps are stored. 0 says 1 is, aand so on.*/
			array_row = 0;/*only runs once*/
			
		if (j == 0) /*to get two blank spaces in front of every map name*/
		{
			array_map_names[array_row][j++] = ' ';
			
			if(array_row == 0) /*to print arrow in front of first map name*/
				array_map_names[array_row][0] = '>';	
			
			array_map_names[array_row][j++] = ' ';
		}
		
		if(filechar == '\n')/*if filechar is newline*/
			{
				array_map_names[array_row][j++] = '\0';/*end this char array with terminating character*/
				array_row++;/*go to next row for next index number*/
				j = 0;/*reset j to 0*/
				filechar = fgetc(read);/*read character again to skip newline*/
				continue;/*skip the next lines of this iteration of the loop*/
			}
			
		array_map_names[array_row][j++] = filechar;/*store the character into the array*/

		filechar = fgetc(read);/*read the next character*/
	}
	fclose(read);/*close the file pointer*/
	
	return array_row;/*return the max index number of the array*/
}

/*function to open the maps. map name is passed, as well as the condition from where the parent funciton was called, and the char map array where the new map will be stored*/
map_selection(char map_name_before[40], int preview_editor_game, char (*ptr_map)[100])
{
	system("cls");/*wipe screen*/

	int i, row = 0, col = 0;
	char map_name[40];
	
	strcpy(map_name, map_name_before);/*copy the recieved map name into the new map_name directory*/
	
	for(i = 0; map_name[i] != '\0'; i++)/*example map name that was passed : "> map005.txt". this loop eliminates the first two characters, leaving only "map005.txt"*/
		map_name[i] = map_name[i+2];
				
	FILE *map_to_open;/*create file pointer*/
	char map_path[20] = "maps\\\\";/*make a char array with the map path beginning*/
	strcat(map_path, map_name);/*add the map name to the end of this map path to create a complete mpath with map name. example: "maps\\\\map005.txt".*/
	map_to_open = fopen(map_path, "r");/*open the file. map_path contains the file name with directory path also. open in read mode*/
	char filechar = fgetc(map_to_open);/*read first character of file*/

	while(filechar != EOF)/*keep reading until file has not ended*/
	{
		
		/*if this function was called from a dame map selection condition, do not print this character. otherwise print this character. without this condition,
		there was a  momentary flickering when picking a new map in game mode*/
		if (preview_editor_game != 2)
			printf("%c", filechar);
		
		if (filechar == '\n')/*if the character is a newline*/
			{
				col = 0;/*reset col*/
				row++;/*increment row*/
			}
		
		else
			ptr_map[row][col++] = filechar;/*store this character in these coordinates of the map array*/

		filechar = fgetc(map_to_open);	/*get the next character*/
	}
	
	if (preview_editor_game == 0)/*if this function was called under preview condition*/
	{
		GoToXY(20,24);/*go to bottom of screen*/
		map_name_before[0] = ' ';/*print a space instead of the arraw that is present as the first character of the map name*/
		printf("%s", map_name_before);/*print the map name*/
		map_name_before[0] = '>';/*put the arrow back into the character name,. this arrow is necessary for navigating the open map screen*/
		sleep(3);/*wait for three seconds so map can be previewed*/
	}
	
	else if(preview_editor_game == 1)/*if this was opened from editor*/
		map_editor(ptr_map, 0);/*call the mpa editor function and pass this new map array to it. the 0 indicates that this is a saved map.*/

}
																/*END OF MAP EDITOR FUNCTIONS*/
																/*Start of highscore functions*/
/*this function is called when you select highscore selection from main menu*/
highscore_selection()
{
	int menu_selection_choice = 0, limit = 2;/*for more details see up_down_menu_selection funcion*/
	char option[][40] = {">  Open single player highscores", "   Open two player highscores", "   Go back"};
	
	printing_highscore_menu(option);

	while(up_down_menu_selection(option, &menu_selection_choice, limit) == 99)
			printing_highscore_menu(option);

	switch(menu_selection_choice)/*which highscore to call(single/double player) , if any,*/
	{
		case 0:
			highscore_printing(-1, 1);/*-1 indicates it is being called from nmenu, 1 indicates call highscores for one player*/
			break;
			
		case 1:
			highscore_printing(-1, 2);/*see above*/
			break;
	}

}

printing_highscore_menu(char option[][40])/*prints highscores and option picking of single or double player highscores*/
{
	system("cls");
	int i;
	
	printf("\n\n\t\t\t\tHIGHSCORES\n\n\n\n");
	
	for(i = 0; i < 3; i++)
		printf("\t\t    %s\n\n", option[i]);

}

struct highScore/*highscore structure, how all highscores are permanently stored in txt*/
{
    char user[100];
    int score;
}temp, array_h[10];

/*snake_points is -1 if opened from main menu, it is the snake points of aplayer otherwise. player is the flag that tells whcih highscore file to open*/
highscore_printing(int snake_points, int player)/*prints highscores, opens highscore txt file for reading, stores contents in array, edits the array, removes previous file and writes new array in file*/
{
	int i, num = 0;
	
	system("cls");
	FILE* highread;/*creates file pointer for highscore reading*/
    
	if (player == 1)
    	highread = fopen("highscores\\highscores_single_player.txt", "r");/*open highscores for 1 player*/
	
    else
    	highread = fopen("highscores\\highscores_two_player.txt", "r");
			
    while(fread(&temp, sizeof(temp), 1, highread) == 1)/*keep reading highscore txt file while there are records*/
	   	array_h[num++] = temp;/*keep storing read highscores from file into an array*/
	
	
	fclose(highread);/*close the file pointer*/
	
	
	if (snake_points < array_h[num-1].score && snake_points != -1)
		return 0;
		
	/*for highscore entry. if the scoes of the player are greater than the minimum scoers present in the array,
	call function to enter new highscores. highscores are != -1 means that this funciton was called from a game , not the main menu. if scores are lesser than the 
	minimum scores present in the array, end this highscore funcition*/
	else if (snake_points != -1)
		highscore_entry(snake_points, num);
	
	/*if function has reached this point and snake points were no -1, it means that there was a new highscore entry. thus increase the number of total records if they were less than 10.
	since the limit of records is 10, do not increase the number of records if previously total number of records was already ten.*/
	if ((num < 10) && snake_points != -1)
		num++;
		/*num is increased because num is the total records currently stored. if it is less than 10 then the total records are increased since the new records are also included. num is incersed here for the 
		upcoming loop to write the strictures into a file*/
	
	printf("Rank\t\t\tName\t\t\tPoints\n");

	for(i = 0 ; i < num ; i++)/*print the (if new highscore entered, modified) array to display*/
		printf("%d\t\t\t%s\t\t\t%d\n", i + 1, array_h[i].user, array_h[i].score);

	if (snake_points != -1)/*if a new entry was made, update highscore txt by removing previous file and writing new file*/
	{
		FILE *hwrite;/*create a new file pointer for writing new highscore file*/
	
	/*remove the previous highscore file from where the highscores were read above. player indicates which highscore file to delete*/
		
		if (player == 1)
		{
			remove("highscores\\highscores_single_player.txt");
			hwrite = fopen("highscores\\highscores_single_player.txt", "w");
		}
			
	    else
	    {
			remove("highscores\\highscores_two_player.txt");
	    	hwrite = fopen("highscores\\highscores_two_player.txt", "w");	
		}
		
		for( i = 0; i < num; i++)/*write the updated into the new highscore file*/
			fwrite(&array_h[i], sizeof(struct highScore), 1, hwrite);
	
		fclose(hwrite);/*close the file pointer to saev the new file*/
	}
	
	printf("\n\n\n\nPress any key to continue...");/*print eexit condition*/
	getch();/*wait for user input. after this function ends, retry screen is called if it was called from a game, otherwise main menu is seen*/
		
}

/*function to enter new highscores. called if score of player was greater than the lowest that was previously present. snake points is the score of the user.
num is the number of records that were previously stored.*/
highscore_entry(int snake_points, int num)
{
	int j;

	printf("Enter Your Name\n");
	fflush(stdin);/*clear the input buffer*/
	scanf("%s", &temp.user);/*enter user name to be shown in highscores. write name in temp struct*/
	temp.score = snake_points;/*assign user points to temp struct*/
	system("cls");/*wipe the screen*/

	if(num == 0)/*if the number of previous records was zero, this is the first record. store it in the first position*/
		array_h[0] = temp;
		
	for (j = num - 1; j >= 0; j--)/*run this loop from the lowest record INDEX NUMBER up to highest. loop ends when j is less than zero, meaning the new snakepoints was greater than all previous entries*/
	{	/*below if condition keeps running until the snake_points is less than the score in the enxt rank*/
		if(snake_points >= array_h[j].score)/*if snake points are greater than this entry*/
		{
			if(j != 9)/*if the current pointed record is not in the 9 index position/ 10 rank/ last possible position*/
				array_h[j+1] = array_h[j];/*move the current record to 1 index number below it*/
					
			array_h[j] = temp;/*after the previous record has been moved to a lower rank, update the present rank with the temp record.
			if the current pointed record is rank 10 and is thus lowest possible. update it so that new temp record becomes rank 10*/
			
		}
	}
}/*end of highscore entry function*/
							/*END OF HIGHSCORE FUNCTIONS*/
															/*End of highscore functions*/
