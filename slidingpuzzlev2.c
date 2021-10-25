#include <stdlib.h>
#include <stdio.h>
#include <time.h>
int boardsize = 4;//board is initially 4x4
int **board;
int blankX;//tracks the x position of the empty space, which is initially the bottom right corner
int blankY;//tracks the y position of the empty space, which is initially the bottom right corner
FILE *fp;
/**
 * prints the entire board
 * @return nothing
 */
void displayBoard()
{
  for(int r = 0; r < boardsize; r++)
  {
    for(int c = 0; c < boardsize; c++)
    {
      if(board[r][c] == 0)
      {
        printf("   ");//print 3 spaces instead of 0. stays lined up with the rest of the board
      }
      else
      {
        printf("%3d", board[r][c]);//line up each tile by printing %3d instead of %d
      }
    }
    printf("\n");
  }
}
/**
 * checks if the move is valid by checking if the empty space is in any way adjacent to the chosen tile
 * @param tile: the int value held by the tile to be moved
 * @return int representing either which direction the tile should be moved, or 0 indicating the move is invalid
 */
int isMoveValid(int tile)
{
  if(blankX+1 < boardsize && board[blankX+1][blankY] == tile)//see if the tile is right of the blank space while staying in bounds
  {
    return 1;//returns an integer for each possible position relative to the blank space
  }
  else if(blankX-1 > -1 && board[blankX-1][blankY] == tile)//see if the tile is left of the blank space while staying in bounds
  {
    return 2;
  }
  else if(blankY+1 < boardsize && board[blankX][blankY+1] == tile)//see if the tile is above the blank space while staying in bounds
  {
    return 3;
  }
  else if(blankY-1 > -1 && board[blankX][blankY-1] == tile)//see if the tile is below the blank space while staying in bounds
  {
    return 4;
  }
  else
  {
    return 0;//return 0 if the tile is in no way adjacent to the empty space
  }
}
/**
 * moves the tile (taken as parameter 1) around the board by checking where the empty space is in relation to it using isMoveValid
 * and then swapping the tile and empty space around
 * @param tile: the int value held by the tile to be moved
 * @return nothing
 */
void moveTile(int tile)
{
  switch(isMoveValid(tile))
  {
    case 1:
      board[blankX+1][blankY] = 0;//set the space where the tile is to 0, using blankX and blankY to recall where that space is
      board[blankX][blankY] = tile;//...and set the blank space to hold the value of the tile that is being "moved"
      blankX++;//adjust blankX to reflect the new location of the blank space
      break;//above three comments apply to the first three lines respectively for each case and so are not repeated below
    case 2:
      board[blankX-1][blankY] = 0;
      board[blankX][blankY] = tile;
      blankX--;
      break;
    case 3:
      board[blankX][blankY+1] = 0;
      board[blankX][blankY] = tile;
      blankY++;
      break;
    case 4:
      board[blankX][blankY-1] = 0;
      board[blankX][blankY] = tile;
      blankY--;
      break;
    case 0:
      printf("Invalid Move.\n");//print this if the move isn't valid
  }
}
/**
 * initialize a new board.
 * @param boardDimension: size of one side of the board.
 */
void initialize(int boardDimension)
{
  board = malloc(boardDimension * sizeof(*board));
  for(int i = 0; i < boardDimension; i++)
  {
    board[i] = malloc(boardDimension * sizeof(board[0]));
  }
  int r = rand()%boardDimension;
  int c = rand()%boardDimension;
  int tileToPlace = 1;
  while(tileToPlace < ((boardDimension * boardDimension)))
  {
    if(board[r][c] == 0)//if the randomly chosen tile is empty,
    {
      board[r][c] = tileToPlace;//place the tile there
      tileToPlace++;//move on to the next tile
    }
    r = rand()%boardDimension;
    c = rand()%boardDimension;//pick another spot
  }
  for(r = 0; r < boardDimension; r++)
  {
    for(c = 0; c < boardDimension; c++)
    {
      if(board[r][c] == 0)
      {
        blankX = r;//find whatever tile was left blank
        blankY = c;//and store x and y coordinates in blankX and blankY
      }
    }
  }
  printf("Setting up a game for a board of size %d\n", boardDimension);
  displayBoard();
}
/**
 * Checks if the board is sorted from one to the last tile. Blank space is ignored
 * return: 1 if true, 0 if false
 */
int wonGame()
{
  int c, r, lastTile = 0;
  int wonGame = 1;
  for(r = 0; r < boardsize; r++)
  {
    for(c = 0; c < boardsize; c++)
    {
      if(board[r][c] != 0)//skip over the tile if it's empty
      {
        if(board[r][c] - 1 != lastTile)//see if the tile at [r][c] is (not) one greater than the one before it
        {
          wonGame = 0;
          break;
        }
        else//if the tile IS one greater than the one before it, set lastTile to the value stored in the current tile
        {
          lastTile = board[r][c];
        }
      }
    }
    if(wonGame == 0)//checks at the end of each row if the condition to set wonGame to false was triggered
    {//if it was, breaks rather than going through the rest of the board which might entail another 9 loops
      break;
    }
  }
  return wonGame;
}
void teardown()
{
  free(board);
  printf("Ending the game.\n");
}
/**
 * saves the game, or returns -1 if there was an issue
 * @param fileName: name of the file
 * @return 0 if successful, -1 if error
 */
int saveGame(char *fileName)
{
  fp = fopen(("%s", fileName), "w");
  if(fp != NULL)//make sure nothing went wrong
  {
    fprintf(fp, "%3d\n", boardsize);//the first line indicates the size of the board to be loaded
    int r, c = 0;
    for(r = 0; r < boardsize; r++)
    {
      for(c = 0; c < boardsize; c++)
      {
        fprintf(fp, "%3d ", board[r][c]);//3 digits so it all lines up, and a space to break up each tile
      }
      fprintf(fp, "\n");
    }
    fclose(fp);
    return 0;
  }
  else
  {
    fclose(fp);
    return -1;//return this if something went wrong
  }
}
/**
 * loads a file determined by fileName, or returns -1 if theres an issue
 * @param fileName: file to load
 * return 0 if successful, -1 if theres an error
 */
int loadGame(char *fileName)
{
  fp = fopen(("%s", fileName), "r");
  if(fp != NULL)
  {
    fscanf(fp, "%d", &boardsize);//first line should be an int indicating board size
    int r, c = 0;
    free(board);
    board = malloc(boardsize * sizeof(*board));
    for(int i = 0; i < boardsize; i++)
    {
      board[i] = malloc(boardsize * sizeof(board[0]));
    }
    for(r = 0; r < boardsize; r++)
    {
      for(c = 0; c < boardsize; c++)
      {
        fscanf(fp, "%d", &board[r][c]);//fill the newly created board with values from the file being loaded
        if(board[r][c] == 0)
        {
          blankX = r;
          blankY = c;//update blankX and blankY for the new board
        }
      }
    }
    fclose(fp);
    return 0;
  }
  else
  {
    fclose(fp);
    return -1;
  }
}
int main()
{
  srand(time(NULL));
  char input = ' ';
  char *fileName = malloc(sizeof(char));
  int tile = 0;
  initialize(boardsize);
  while(input != 'q')
  {
    if(wonGame() == 1)
    {
      printf("You won! Winning board shown below\n");
      displayBoard();
      printf("Setting up a new game\n");
      free(board);
      boardsize = 0;
      while(getchar()!='\n');
      while(boardsize < 2 || boardsize > 10)
      {
        printf("Enter the size of the new board. It must be between 2 and 10\n");
        scanf("%d", &boardsize);
      }
      initialize(boardsize);
    }
    printf("Menu: [n]ew, [s]ave, [l]oad, [p]rint, [m]ove, [q]uit? ");
    scanf(" %c", &input);
    if(input == 'n')
    {
      free(board);
      boardsize = 0;
      while(boardsize < 2 || boardsize > 10)
      {
        printf("Enter the size of the new board. It must be between 2 and 10\n");
        scanf("%d", &boardsize);
      }
      initialize(boardsize);
    }
    if(input == 's')
    {
      printf("Please enter the name of the board to save\n");
      scanf("%s", fileName);
      if(saveGame(fileName) != -1)
      {
        printf("Game successfully saved\n");
      }
      else
      {
        printf("There was an issue saving the game\n");
      }
    }
    if(input == 'l')
    {
      printf("Please enter the name of the board to load\n");
      scanf("%s", fileName);
      if(loadGame(fileName) != -1)
      {
        printf("Loaded game successfully\n");
      }
      else
      {
        printf("Failed to load game. Does the file not exist?\n");
      }
    }
    if(input == 'p')
    {
      displayBoard();
    }
    else if(input == 'm')
    {
       printf("Which tile to move? ");
       scanf("%d", &tile);
       moveTile(tile);
    }
  }
  teardown();
}
