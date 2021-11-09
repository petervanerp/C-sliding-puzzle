#include <stdio.h>
#include <unistd.h>
#include "prototypes.h"
//#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
int **board;
int blankX;
int blankY;
int boardSize;
int readCommand(int fromClient, int size)
{
  int nread;
  int buffer;
  while(1)
  {
    nread = read(fromClient, &buffer, size);
    switch(nread)
    {
      case -1:
        if(errno == EAGAIN)
        {
          sleep(1);
          break;
        }
        else
        {
          close(fromClient);
          exit(0);
        }
      case 0:
        exit(0);
      default:
        return buffer;
    }
  }
}
/**
 * Checks if the entered size is valid, and if so makes a new board
 * Time permitting I will use a better algorithm than randomly picking tiles
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
    board = malloc((size+1) * sizeof(*board));
    for(int i = 0; i < (size+1); i++)
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
int loadGame(char *fileName)
{
  return -1;
}

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
