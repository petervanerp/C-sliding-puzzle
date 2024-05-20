A text-based puzzle game made in C. Demonstrates use of basic C programming fundamentals. Features include:
- The ability to create N by N puzzles for N from 2 to 10
- The ability to save and load these puzzle configurations
- The user is tasked with solving a puzzle by sorting the tiles so that the first tile is in the top left, and the last is in the top right. They do this by moving one tile at a time
- When the player wins, they are asked if they want to create another puzzle, and if so, how large they want that puzzle to be
- Implements a simulated client-server setup using pipes. The "server" records the user's moves and the state of the puzzle, and checks if the player has won. The "client" handles user-side functions like input and displaying the puzzle

To compile and run:
- run 'make -f makefile' in the same directory that the files are located in
- run 'slidingpuzzle-v3'

After creating a new puzzle, you will need to print it out by entering '5', as the game will not automatically display the puzzle as soon as you create it. 
