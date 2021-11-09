#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "prototypes.h"
#include "globalVars.h"
void passCommand(int toServer)
{
  int command;
  printf("Enter a command\n1)New game\n2)Save game\n3)Load game\n4)Print board\n5)Move tile\n6)Quit\n");
  scanf("%d", &command);
  write(toServer, &command, sizeof(int));
}
void printBoard(int **board)
{
  int r, c;
  for(r = 0; r < boardSize; r++)
  {
    for(c = 0; c < boardSize; c++)
    {
      printf("%3d ", board[r][c]);
    }
    printf("\n");
  }
}
void readBoard(int fromServer)
{
  int nextTile;
  int r = 0;
  int c = 0;
  for(r = 0; r <= boardSize; r++)
  {
    for(c = 0; c <= boardSize; c++)
    {
      read(fromServer, &nextTile, sizeof(int));
      printf("%3d", nextTile);
    }
    printf("\n");
  }
}
