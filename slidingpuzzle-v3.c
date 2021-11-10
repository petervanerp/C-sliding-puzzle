#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include "prototypes.h"
#include "globalVars.h"
#include <stdlib.h>
#include <time.h>
//enum command{cmd_new, cmd_save, cmd_load, cmd_move, cmd_print, cmd_quit};
int main()
{
  srand(time(NULL));
  newBoard(4);
  boardSize = 4;
  //client to server pipe
  int clientToServer[2];
  pipe(clientToServer);
  //char *c2sBuffer = malloc(sizeof(char));
  char c2sBuffer[20];
  //server to client pipe
  int serverToClient[2];
  pipe(serverToClient);
  //char *s2cBuffer = malloc(sizeof(char));
  //char s2cBuffer[20];
  pid_t client = fork();
  int command = 0;
  int tileNumber = 0;
  int result;//returns results of various functions from server to client
  result = 0;
  if(client < 0)
  {//error
    exit(3);
  }
  else
  {
    while(1)
    {
      if(client == 0)
      {//client
        close(clientToServer[0]);
        close(serverToClient[1]);
        printf("Enter one of the following:\n1)New game\n2)Save game\n3)Load game\n4)Move tile\n5)Print board\n6)Quit\n");
        scanf("%d", &command);
        write(clientToServer[1], &command, sizeof(int));//first 4 bytes in will be the command type given as an int
        switch(command)
        {
          case 1://new
            printf("Enter size of new board. Must be at least 2 and no more than 10\n");
            scanf("%d", &boardSize);//there's no need to actually check the size entered since the server will just return -1 if it's below 2 or above 10
            write(clientToServer[1], &boardSize, sizeof(int));
            read(serverToClient[0], &result, sizeof(int));
            if(result == -1)
            {
              printf("Error creating board. You most likely entered the wrong size\n");
            }
            else
            {
              printf("Successfully created board\n");
            }
            break;
          case 2://save
            printf("Enter file name:\n");
            scanf("%s", c2sBuffer);
            write(clientToServer[1], &c2sBuffer, sizeof(&c2sBuffer));
            read(serverToClient[0], &result, sizeof(int));
            if(result == -1)
            {
              printf("Error saving file\n");
            }
            else
            {
              printf("Successfully saved file\n");
            }
            break;
          case 3://load
            printf("Enter file name:\n");
            scanf("%s", c2sBuffer);
            write(clientToServer[1], &c2sBuffer, sizeof(&c2sBuffer));
            read(serverToClient[0], &result, sizeof(int));
            if(result == -1)
            {
              printf("Error loading file\n");
            }
            else
            {
              printf("Successfully loaded file\n");
              read(serverToClient[0], &boardSize, sizeof(int));
            }
            break;
          case 4://move
            if(getMoveValidity(serverToClient[0], clientToServer[1]) == 0)
            {
              printf("Move successful\n");
            }
            else
            {
              printf("Move failed\n");
            }
            break;
          case 5://print
            readBoard(serverToClient[0]);
            break;
          case 6://quit
            printf("Shutting down\n");
            exit(0);
        }
        if(checkWin(serverToClient[0]) == 0)
        {
          boardSize = 0;
          while(boardSize > 10 || boardSize < 2)
          {
            printf("Enter size of new board. Must be at least 2 and no more than 10\n");
            scanf("%d", &boardSize);
            write(clientToServer[1], &boardSize, sizeof(int));
          }
        }
      }
      else
      {//server
        close(clientToServer[1]);
        close(serverToClient[0]);
        int winStatus = 0;//store win status here to send to client at the end of loop
        read(clientToServer[0], &command, sizeof(int));
        switch(command)
        {
          case 1://new
            read(clientToServer[0], &boardSize, sizeof(int));//next int will always be size
            result = newBoard(boardSize);
            write(serverToClient[1], &result, sizeof(int));
            break;
          case 2://save
            read(clientToServer[0], &c2sBuffer, sizeof(&c2sBuffer));
            result = saveGame(c2sBuffer);
            write(serverToClient[1], &result, sizeof(int));
            break;
          case 3://load
            read(clientToServer[0], &c2sBuffer, sizeof(&c2sBuffer));
            result = loadGame(c2sBuffer);
            write(serverToClient[1], &result, sizeof(int));
            write(serverToClient[1], &boardSize, sizeof(int));//update boardsize for client
            break;
          case 4://move
            read(clientToServer[0], &command, sizeof(int));
            result = moveTile(command);
            write(serverToClient[1], &result, sizeof(int));
            break;
          case 5://print
            sendBoard(serverToClient[1]);//sends the entire board as a series of integers to be printed
            break;
          case 6://quit
            result = -1;
            write(serverToClient[1], &result, sizeof(int));
            break;
        }
      winStatus = checkWinServerEnd();
      write(serverToClient[1], &winStatus, sizeof(int));//write win status
      if(winStatus == 0)
      {
        boardSize == 0;
        while(newBoard(boardSize) != 0)
        {
          read(clientToServer[0], &boardSize, sizeof(int));
        }
      }
      }
    }
  }
}
