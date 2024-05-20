#include <stdio.h>
#include <unistd.h>
#include "prototypes.h"
#include <errno.h>
#include <stdlib.h>
#include <time.h>
int **board;
int blankX;
int blankY;
int boardSize;

/**
 * Checks if the entered size is valid, and if so makes a new board
 * @param size: size of the board
 * return: -1 if size is invalid, 0 otherwise
 */
int newBoard(int size)
{
  if(size > 10 || size < 2)
  {
    return -1;//size not valid
  }
  else
  {
    free(board);
    board = malloc((size) * sizeof(*board));
    for(int i = 0; i < (size); i++)
    {
      board[i] = malloc((size) * sizeof(board[0]));
    }
    int r, c;
    for(r = 0; r < size; r++)
    {
      for(c = 0; c < size; c++)
      {
        board[r][c] = 0;
      }
    }
    r = rand()%size;
    c = rand()%size;
    int tileToPlace = 0;
    while(tileToPlace < ((size * size)))
    {
      if(board[r][c] == 0)
      {
        board[r][c] = tileToPlace;
        tileToPlace++;
      }
      r = rand()%size;
      c = rand()%size;
    }
    for(r = 0; r < size; r++)
    {
      for(c = 0; c < size; c++)
      {
        if(board[r][c] == 0)
        {
          blankX = r;
          blankY = c;//store whatever tile was blank as the location of blank space
        }
      }
    }
    return 0;//successfully made a board
  }
}
/**
 * saves a game based on fileName provided
 * return: 0 if success -1 if fail
 */
int saveGame(char *fileName)
{
  FILE *fp;
  fp = fopen(("%s", fileName), "w");
  if(fp != NULL)
  {
    fprintf(fp, "%3d\n", boardSize);//first line indicates size of board
    int r, c = 0;
    for(r = 0; r < boardSize; r++)
    {
      for(c = 0; c < boardSize; c++)
      {
        fprintf(fp, "%3d ", board[r][c]);
      }
      fprintf(fp, "\n");
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
/**
 * loads a file based on fileName provided
 * return -1 if fail 0 if success
 */
int loadGame(char *fileName)
{
  FILE *fp;
  fp = fopen(("%s", fileName), "r");
  if(fp)
  {
    fscanf(fp, "%d", &boardSize);//first line will indicated board size
    //freeBoard(boardSize);
    free(board);
    int r, c = 0;
    board = malloc(boardSize * sizeof(*board));
    for(int i = 0; i < boardSize; i++)
    {
      board[i] = malloc(boardSize * sizeof(board[0]));
    }
    for(r = 0; r < boardSize; r++)
    {
      for(c = 0; c < boardSize; c++)
      {
        fscanf(fp, "%d", &board[r][c]);
        if(board[r][c] == 0)
        {
          blankX = r;
          blankY = c;
        }
      }
    }
    fclose(fp);
    return 0;
  }
  else
  {
    return -1;
  }
}
/**
 * sends every int in the array one by one to be read on client end
 * @param: open write pipe to client
 */
void sendBoard(int toClient)
{
  int r, c;
  for(r = 0; r < boardSize; r++)
  {
    for(c = 0; c < boardSize; c++)
    {
      write(toClient, &board[r][c], sizeof(int));
    }
  }
}
/**
 * Checks if the board is sorted from one to the last. ignores blank space
 * return: -1 if false, 0 if true
 */
int checkWinServerEnd()
{
  int r, c;
  int lastTile = 0;
  for(r = 0; r < boardSize; r++)
  {
    for(c = 0; c < boardSize; c++)
    {
      if(board[r][c] != 0)
      {
        if(board[r][c] - 1 != lastTile)
        {
          return -1;//return -1 if the current tile is not one greater than the last one
        }
        else
        {
          lastTile = board[r][c];
        }
      }
    }
  }
  return 0;//return 0 if win
}
/**
 * checks if the given tile can be moved
 * @param tile: the number associated with the tile to be moved
 */
int isMoveValid(int tile)
{
  if(blankX+1 < boardSize && board[blankX+1][blankY] == tile)//each statement refers to checking if the tile is adjacent to the blank space in a certain direction
  {
    return 1;//return an integer indicating which direction
  }
  else if(blankX-1 > -1 && board[blankX-1][blankY] == tile)
  {
    return 2;
  }
  else if(blankY+1 < boardSize && board[blankX][blankY+1] == tile)
  {
    return 3;
  }
  else if(blankY-1 > -1 && board[blankX][blankY-1] == tile)
  {
    return 4;
  }
  else
  {
    return 0;//if the tile isnt near the blank space
  }
}
/**
 * moves a tile if possible
 * @param tile to be moved
 * return 0 if successful, -1 if fail
 */
int moveTile(int tile)
{
  switch(isMoveValid(tile))
  {
    case 1:
      board[blankX+1][blankY] = 0;//set space with given tile to 0
      board[blankX][blankY] = tile;//set blank space to tile's value
      blankX++;//adjust blankX to reflect new blank space location
      return 0;//above comments apply to each case
    case 2:
      board[blankX-1][blankY] = 0;
      board[blankX][blankY] = tile;
      blankX--;
      return 0;
    case 3:
      board[blankX][blankY+1] = 0;
      board[blankX][blankY] = tile;
      blankY++;
      return 0;
    case 4:
      board[blankX][blankY-1] = 0;
      board[blankX][blankY] = tile;
      blankY--;
      return 0;
    case 0:
      return -1;//return -1 if fail
  }
}
