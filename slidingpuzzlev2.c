#include <stdlib.h>
#include <stdio.h>
#include <time.h>
int boardsize = 0;
int **board;
int blankX;//tracks the x position of the empty space, which is initially the bottom right corner
int blankY;//tracks the y position of the empty space, which is initially the bottom right corner
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
void initialize()
  {
    while(boardsize < 2 || boardsize > 10)
    {
      printf("Enter the size of the new board. It must be between 2 and 10\n");
      scanf("%d", &boardsize);
    }
    board = malloc(boardsize * sizeof(*board));
    for(int i = 0; i < boardsize; i++)
    {
      board[i] = malloc(boardsize * sizeof(board[0]));
    }
    int r = rand()%boardsize;
    int c = rand()%boardsize;
    int tileToPlace = 1;
    while(tileToPlace < ((boardsize * boardsize)))
    {
      if(board[r][c] == 0)
      {
        board[r][c] = tileToPlace;
        tileToPlace++;
      }
      r = rand()%boardsize;
      c = rand()%boardsize;
    }
    for(r = 0; r < boardsize; r++)
    {
      for(c = 0; c < boardsize; c++)
      {
        if(board[r][c] == 0)
        {
          blankX = r;
          blankY = c;
        }
      }
    }
    printf("Setting up the game\n");
    displayBoard();
  }
/**
 * 
 * 
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
int main()
{
  srand(time(NULL));
  char input = ' ';
  int tile = 0;
  initialize();
  while(input != 'q')
  {
    if(wonGame() == 1)
    {
      printf("You won! Winning board shown below\n");
      displayBoard();
      printf("Setting up a new game\n");
      free(board);
      boardsize = 0;
      initialize();
    }
    printf("Menu: [n]ew, [s]ave, [l]oad, [p]rint, [m]ove, [q]uit? ");
    scanf(" %c", &input);
    if(input == 'n')
    {
      free(board);
      boardsize = 0;
      initialize();
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
