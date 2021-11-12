#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "prototypes.h"
#include "globalVars.h"

/**
 * Checks with server to see if the player won.
 * The server will probably already be creating a board as soon as it write an int indicating victory
 * @param fromServer: open read pipe to get win status from
 * return depends on what server returns. Normally this is always -1 if no win, 0 if win
 */
int checkWin(int fromServer)
{
  int winStatus = 0;
  read(fromServer, &winStatus, sizeof(int));
  return winStatus;
}
/**
 * reads board from server
 * @param fromServer: open read pipe
 */
void readBoard(int fromServer)
{
  int nextTile;
  int r = 0;
  int c = 0;
  for(r = 0; r < boardSize; r++)
  {
    for(c = 0; c < boardSize; c++)
    {
      read(fromServer, &nextTile, sizeof(int));
      printf("%3d", nextTile);
    }
    printf("\n");
  }
}
/**
 * takes a number and sends to server. server moves tiles based on this number and sends back
 * an integer indicating success
 * @param fromServer: open read pipe from server
 * @param toServer: open write pipe to server
 * return -1 if fail, 0 if success. SHOULD be the same as moveTile(tile);
 */
int getMoveValidity(int fromServer, int toServer)
{
  int tile;
  int validity;
  printf("Enter tile number\n");
  scanf("%d", &tile);
  write(toServer, &tile, sizeof(int));
  read(fromServer, &validity, sizeof(int));
  return validity;//should be the same as moveTile(tile)
}
