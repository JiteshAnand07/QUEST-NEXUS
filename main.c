#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

// defining no of cells and no of mines for mine sweeper
#define gridSize 9
#define minesNumber 8

// this function is used to add colors in our terminal
// takes a integer in input denoting the code for the color
void setConsoleColor(int color)
{
    // this hconsole is the pointer to our current output terminal
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // this sets  color to text
    // this function is provides in the window.h header file
    SetConsoleTextAttribute(hConsole, color);
}


//MINESWEEPER
// to place mines in the grid
void placeMines(int grid[gridSize][gridSize], int size, int mine)
{
    // this function will generate random seed for the srand function to generate random number
    srand(time(0));

    // running loop till all the mines are placed
    while (mine > 0)
    {
        // generating random coordinates
        int xCoord = rand() % size;
        int yCoord = rand() % size;

        // ensuring that no two mines gets placed on one block
        if (grid[xCoord][yCoord] == 0)
        {
            grid[xCoord][yCoord] = -1;
            mine--;
        }
    }
}

// to mark each cell of the grid
// we will give each cell a number represeting the no of mines in its neighbourhood
void markGrid(int grid[gridSize][gridSize], int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (grid[i][j] == -1)
                continue;
            int mines = 0;

            // checking for each cell around the current cell
            if (j + 1 < size && grid[i][j + 1] == -1)
                mines++;
            if (j - 1 >= 0 && grid[i][j - 1] == -1)
                mines++;
            if (i + 1 < size && grid[i + 1][j] == -1)
                mines++;
            if (i - 1 >= 0 && grid[i - 1][j] == -1)
                mines++;
            if (i - 1 >= 0 && j + 1 < size && grid[i - 1][j + 1] == -1)
                mines++;
            if (i - 1 >= 0 && j - 1 >= 0 && grid[i - 1][j - 1] == -1)
                mines++;
            if (i + 1 < size && j - 1 >= 0 && grid[i + 1][j - 1] == -1)
                mines++;
            if (i + 1 < size && j + 1 < size && grid[i + 1][j + 1] == -1)
                mines++;

            grid[i][j] = mines;
        }
    }
}

// for checking which cells are to be shown if the cell chosen by the user is a safe
// it reveals all the safe zones around the cell chosen and shows the border containing only numbers
// this is a recursive function
void checkGrid(int grid[gridSize][gridSize], int revealGrid[gridSize][gridSize], int size, int xCoord, int yCoord)
{
    // this function only executes if the cell number in input is valid
    if (xCoord >= 0 && xCoord < size && yCoord >= 0 && yCoord < size)
    {
        // stops if it founds a number greater than 0 and its not reveal till now
        if (grid[xCoord][yCoord] > 0 && revealGrid[xCoord][yCoord] == 0)
        {
            revealGrid[xCoord][yCoord] = 1;
            return;
        }

        // go to each cell with 0 and repeat the process for the adjacent cells
        if (grid[xCoord][yCoord] == 0 && revealGrid[xCoord][yCoord] == 0)
        {
            revealGrid[xCoord][yCoord] = 1;
            checkGrid(grid, revealGrid, size, xCoord, yCoord + 1);
            checkGrid(grid, revealGrid, size, xCoord, yCoord - 1);
            checkGrid(grid, revealGrid, size, xCoord + 1, yCoord);
            checkGrid(grid, revealGrid, size, xCoord - 1, yCoord);

            // for diagnols
            checkGrid(grid, revealGrid, size, xCoord + 1, yCoord + 1);
            checkGrid(grid, revealGrid, size, xCoord + 1, yCoord - 1);
            checkGrid(grid, revealGrid, size, xCoord - 1, yCoord + 1);
            checkGrid(grid, revealGrid, size, xCoord - 1, yCoord - 1);
        }
    }
}

// checking whether the user has won the game or not
int checkWin(int grid[gridSize][gridSize], int revealGrid[gridSize][gridSize], int size)
{
    // it counts number of mines and no of safezones that are revealed
    // if sum of both equals to total no of cells then the user has won the game
    int countMines = 0;
    int countSafeZonesRevealed = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (grid[i][j] == -1)
                countMines++;
            else if (revealGrid[i][j] == 1)
                countSafeZonesRevealed++;
        }
    }

    // returns 1 if won and 0 if not won till now
    if (countSafeZonesRevealed + countMines == size * size)
        return 1;
    else
        return 0;
}

// for printing grid
void printGrid(int grid[gridSize][gridSize], int revealGrid[gridSize][gridSize], int size)
{
    setConsoleColor(15);
    printf("       1  2  3  4  5  6  7  8  9  \n");
    printf("       v  v  v  v  v  v  v  v  v   \n");
    printf("     ----------------------------- \n");
    for (int i = 0; i < size; i++)
    {
        setConsoleColor(15);
        printf("%d > |  ", i + 1);
        for (int j = 0; j < size; j++)
        {
            // printing each cell based on whether its revealed according to reveal grid or not
            if (revealGrid[i][j] == 1)
            {
                if (grid[i][j] == -1)
                {
                    setConsoleColor(12); // Red for mines
                    printf("*  ");
                }
                else if (grid[i][j] == 0)
                {
                    setConsoleColor(10); // Green for safe cells
                    printf("o  ");
                }
                else
                {
                    setConsoleColor(14); // Yellow for numbers
                    printf("%d  ", grid[i][j]);
                }
            }
            else
            {
                setConsoleColor(8); // Gray for unrevealed cells
                printf("-  ");
            }
        }
        setConsoleColor(15);
        printf("|\n");
    }
    printf("     ----------------------------- \n");
}

void mineSweeper()
{
    printf("\n");
    // displaying the instructions
    printf("HOW TO PLAY:\n");

    printf("-----------------------------------------------------------------\n");

    printf("1. The goal is to uncover all safe cells without hitting a mine.\n");
    printf("2. Numbers indicate how many mines are adjacent to a cell.\n");
    printf("3. Be strategic! Avoid cells with mines (*), or it's game over.\n");
    printf("4. Enter your move as: ROW COLUMN (e.g., 3 4).\n");
    printf("5. Clear all safe cells to win the game. Best of luck!\n");

    printf("-----------------------------------------------------------------\n");
    setConsoleColor(15);

    setConsoleColor(5);
    printf("\nLet's get started! Here's your grid:\n\n");
    setConsoleColor(15);

    // defining two matrices
    // grid        - for storing the mines and cells
    // reveal grid - for storing which cell is revealed and which is not
    int grid[gridSize][gridSize] = {0}, revealGrid[gridSize][gridSize] = {0};

    placeMines(grid, gridSize, minesNumber);

    markGrid(grid, gridSize);

    printGrid(grid, revealGrid, gridSize);

    // main gameloop
    while (1)
    {
        // inputing user choice
        int x, y;
        setConsoleColor(10);
        printf("\nEnter your move (ROW COLUMN): ");
        setConsoleColor(15);

        setConsoleColor(14);
        scanf("%d %d", &x, &y);
        printf("\n");
        setConsoleColor(15);
        x -= 1;
        y -= 1;

        // checking the user input
        if (!(x >= 0 && x < gridSize && y >= 0 && y < gridSize))
        {
            setConsoleColor(12);
            printf("\nInvalid move! Stay within the grid.\n");
            setConsoleColor(15);
            continue;
        }

        // if the cell input by user has bomb then game is over
        if (grid[x][y] == -1)
        {
            setConsoleColor(12);
            printf("\nBOOM! You hit a mine. Game Over.\n");
            printf("Revealing the whole grid!!");
            printf("\n");
            printf("\n");
            setConsoleColor(15);
            for (int i = 0; i < gridSize; i++)
                for (int j = 0; j < gridSize; j++)
                    revealGrid[i][j] = 1;
            printGrid(grid, revealGrid, gridSize);
            break;
        }

        // or reveal the grid automatically
        checkGrid(grid, revealGrid, gridSize, x, y);
        printGrid(grid, revealGrid, gridSize);

        // checking whether the user has won or not
        if (checkWin(grid, revealGrid, gridSize))
        {
            setConsoleColor(10);
            printf("\nCongratulations! You cleared the board!\n");
            setConsoleColor(15);
            break;
        }
    }
    setConsoleColor(11);
    printf("--------------------------------------------------------------");
    printf("\nThanks for playing!\n");
    setConsoleColor(15);
}



//================================================================================================================================================================================

 // HANGMAN

#define MAX_WORDS 100


const char *words[MAX_WORDS] = { "Apple", "Chair", "Table", "Dance", "Beach", "Clock", "Drink", "Light", "Grass", "House", "Sleep", "Peace", "Smile", "Plant", "Stone", "Tiger",
                                 "Bread", "Earth", "Money", "Flame", "Drink", "Water", "Magic", "Music", "Dream", "Clear", "Cloud", "Beach", "Swing", "Crown", "Ghost", "Lemon",
                                 "Block", "Reach", "Storm", "Mango", "Spice", "Space", "Field", "Grape", "Chair", "Arrow", "Wrist", "Storm", "Peach", "Thumb", "Jumpy", "Flora",
                                 "Green", "Quick", "Prize", "Slice", "Fresh", "Skirt", "Power", "Grant", "Laugh", "Skies", "Pizza", "Mount", "Trick", "Penne", "Steam", "Blaze",
                                 "Beast", "Craft", "Draft", "Drill", "Hilly", "Flask", "Class", "Plumb", "Creak", "Swing", "Stack", "Jolly", "Boast", "Charm", "Paint", "Punch",
                                 "Knock", "Track", "Prize", "Blaze", "Sharp", "Front", "Shine", "Sweat", "Roast", "Foldy", "Veins", "Pleat", "Badge", "Shade", "Curve", "Route",
                                };

const char *hangmanStages[] = {"  +---+\n  |   |\n      |\n      |\n      |\n      |\n=========",
                               "  +---+\n  |   |\n  O   |\n      |\n      |\n      |\n=========",
                               "  +---+\n  |   |\n  O   |\n  |   |\n      |\n      |\n=========",
                               "  +---+\n  |   |\n  O   |\n /|   |\n      |\n      |\n=========",
                               "  +---+\n  |   |\n  O   |\n /|\\  |\n      |\n      |\n=========",
                               "  +---+\n  |   |\n  O   |\n /|\\  |\n /    |\n      |\n=========",
                               "  +---+\n  |   |\n  O   |\n /|\\  |\n / \\  |\n      |\n=========" };


void Hangman() {
// Select a random word from the words array
  //printf("")
 int randomIndex = rand() % MAX_WORDS;
 char *word = (char *)words[randomIndex];
 char guess[2];
 int attempts = 6, correct = 0;
 char guessed[20] = {0};
 setConsoleColor(10); // Green for welcome message
 printf("\n\nWelcome to hangman!\n");
 printf("INSTRUCTIONS : \n");
 printf("The player must guess one letter at a time. \n");
 printf("The player has a limited number of incorrect guesses (here 6). \n");
 printf("The player wins by revealing the entire word. \n");
 printf("The player loses if they make 6 incorrect guesses before the word is fully revealed.\n");

 while(attempts > 0) {
   setConsoleColor(14); // Yellow for hangman stage
   printf("%s\n", hangmanStages[6-attempts]);  // Display the current stage of the hangman
   printf("Word: ");
   setConsoleColor(15); // White for the word display
   for(int i = 0; i < strlen(word); i++){
      if(guessed[i]) {
         printf("%c ", word[i]);
      } else {
         printf("_ ");
      }
   }
   printf("\n");
   setConsoleColor(12); // Red for attempts left message
   printf("\nAttempts left: %d\n", attempts);
   setConsoleColor(11); // Cyan for input prompt
   printf("Enter a letter: ");
   setConsoleColor(7); // Reset to white for user input
   scanf(" %c", &guess[0]);
   system("cls");
   setConsoleColor(10);
    printf("$$\\   $$\\  $$$$$$\\  $$\\   $$\\  $$$$$$\\        $$\\      $$\\  $$$$$$\\  $$\\   $$\\ \n");
    printf("$$ |  $$ |$$  __$$\\ $$$\\  $$ |$$  __$$\\       $$$\\    $$$ |$$  __$$\\ $$$\\  $$ | \n");
    printf("$$ |  $$ |$$ /  $$ |$$$$\\ $$ |$$ /  \\__|      $$$$\\  $$$$ |$$ /  $$ |$$$$\\ $$ | \n");
    printf("$$$$$$$$ |$$$$$$$$ |$$ $$\\$$ |$$ |$$$$\\       $$\\$$\\$$ $$ |$$$$$$$$ |$$ $$\\$$ | \n");
    printf("$$  __$$ |$$  __$$ |$$ \\$$$$ |$$ |\\_$$ |      $$ \\$$$  $$ |$$  __$$ |$$ \\$$$$ | \n");
    printf("$$ |  $$ |$$ |  $$ |$$ |\\$$$ |$$ |  $$ |      $$ |\\$  /$$ |$$ |  $$ |$$ |\\$$$ | \n");
    printf("$$ |  $$ |$$ |  $$ |$$ | \\$$ |\\$$$$$$  |      $$ | \\_/ $$ |$$ |  $$ |$$ | \\$$ | \n");
    printf("\\__|  \\__|\\__|  \\__|\\__|  \\__| \\______/       \\__|     \\__|\\__|  \\__|\\__|  \\__| \n");

   int found = 0;
   for(int i=0; i<strlen(word); i++){
      if(word[i] == guess[0]) {
         guessed[i] = 1;
         found = 1;
      }
   }
   if(!found) {
      attempts--;
      setConsoleColor(12); // Red for wrong guess message
      printf("Wrong guess!\n");
      setConsoleColor(7); // Reset to white
   }
   correct = 1;
   for(int i=0; i<strlen(word); i++) {
       if(!guessed[i]) {
          correct = 0;
          break;
       }
   }
   if(correct) {
      setConsoleColor(10); // Green for correct guess message
      printf("Congratulations! You've guessed the word: %s\n", word);
      setConsoleColor(7); // Reset to white
     // gamebreaker();
      return;
   }
 }
 // Final stage (game over)
 setConsoleColor(12); // Red for game over message
 printf("%s\n", hangmanStages[6]); // Display the final stage
 printf("Game Over! The word was: %s\n", word);
 setConsoleColor(7); // Reset to white
 //gamebreaker();
 }


//=================================================================================================================================================================================

//ROCK PAPER SCISSOR

// Function to get the player's choice
int getPlayerChoice() {
    int choice;
    setConsoleColor(11);
    printf("\n\nWelcome to Rock Paper Scissor Game \n");
    printf("Rules : \n ");
    printf("Rock crushes Scissors (Rock wins).\n");
    printf("Scissors cuts Paper (Scissors wins).\n");
    printf("Paper covers Rock (Paper wins).\n");
    printf("Enter your choice (1 for Rock, 2 for Paper, 3 for Scissors): \n");
    scanf("%d", &choice);
    return choice;
}

// Function to get the computer's choice
int getComputerChoice() {
    return rand() % 3 + 1;  // Random number between 1 and 3
}

// Function to display the choice as text
void displayChoice(int choice) {
    switch(choice) {
        case 1: printf("Rock\n"); break;
        case 2: printf("Paper\n"); break;
        case 3: printf("Scissors\n"); break;
        default: printf("Invalid choice\n"); break;
    }
}

// Function to determine the winner
void determineWinner(int playerChoice, int computerChoice, int* playerScore, int* computerScore) {
    if (playerChoice == computerChoice) {
        setConsoleColor(14);
        printf("It's a tie!\n");
    } else if ((playerChoice == 1 && computerChoice == 3) ||  // Rock beats Scissors
               (playerChoice == 2 && computerChoice == 1) ||  // Paper beats Rock
               (playerChoice == 3 && computerChoice == 2)) {  // Scissors beats Paper
        setConsoleColor(10);
        printf("You win this round!\n");
        (*playerScore)++;
    } else {
        setConsoleColor(12);
        printf("Computer wins this round!\n");
        (*computerScore)++;
    }
}

// Function to display the scores
void displayScores(int playerScore, int computerScore) {
    setConsoleColor(1);
    printf("\nScore - You: %d, Computer: %d\n", playerScore, computerScore);
}

// Main game function with multiple rounds
void RockPaperScissors() {
    srand(time(NULL));  // Seed the random number generator
    int playerChoice, computerChoice;
    int playerScore = 0, computerScore = 0;
    char playAgain;

    do {
        // Get the player's choice
        playerChoice = getPlayerChoice();

        // Check if the player's choice is valid
        if (playerChoice < 1 || playerChoice > 3) {
            setConsoleColor(12);
            printf("Invalid choice! Please enter a number between 1 and 3.\n");
            continue;  // Skip the rest of the loop and ask again
        }

        // Get the computer's choice
        computerChoice = getComputerChoice();

        // Display choices
        setConsoleColor(15);
        printf("\nYou chose: ");
        displayChoice(playerChoice);
        printf("Computer chose: ");
        setConsoleColor(15);
        displayChoice(computerChoice);

        // Determine and display the winner
        determineWinner(playerChoice, computerChoice, &playerScore, &computerScore);

        // Display scores
        displayScores(playerScore, computerScore);

        // Ask if the player wants to play again
        setConsoleColor(11);
        printf("\nDo you want to play another round? (Y/N): ");
        scanf(" %c", &playAgain);  // Use " %c" to consume the leftover newline character from previous input
        playAgain = (playAgain == 'Y' || playAgain == 'y') ? 'Y' : 'N';  // Normalize input
        printf("\n");
    } while (playAgain == 'Y');

    // Final score display after the game ends
    setConsoleColor(1);
    printf("\nFinal Score - You: %d, Computer: %d\n", playerScore, computerScore);
    if (playerScore > computerScore) {
        setConsoleColor(10);
        printf("Congratulations, you win the game!\n");
    } else if (playerScore < computerScore) {
        setConsoleColor(12);
        printf("Computer wins the game! Better luck next time.\n");
    } else {
        setConsoleColor(14);
        printf("The game ends in a tie!\n");
    }
}


 //=============================================================================================================================================================================

 //TIC TAC TOE

 #define X 'X'
#define O 'O'
char board[3][3]; // 3x3 board for the game

int player1Wins = 0, player2Wins = 0, playerWins = 0, playerGWins = 0, computerWins = 0, computerGWins = 0, draws = 0, draw = 0, drawss = 0; // Score variables

// Function to initialize the board with empty spaces
void initializeBoard() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }
}

// Function to print the current state of the board
void printBoard(int mode) {
    system("cls"); // Clear screen
    setConsoleColor(5);
    printf("\n\nTic-Tac-Toe Game\n");
    printf("--------------------------------------------\n");
    printf("Players take turns to place their mark (X or O) on a 3x3 grid.\n");
    printf("The player who first gets 3 marks in a row (horizontally, vertically, or diagonally) wins.\n");
    printf("To make a move, enter a row column , for eg. 1 2 to place your mark on the grid\n");
    setConsoleColor(15);
    // Print scorecard
    if (mode == 1) {
        // Multiplayer score
        printf("Score: Player 1 - %d | Player 2 - %d | Draws - %d\n", player1Wins, player2Wins, draws);
    }
    if (mode == 2) {
        // Single-player score
        printf("Score: Player - %d | Computer - %d | Draw - %d\n", playerWins, computerWins, draw);
    }
    if (mode == 3) {
        // God Mode score
        printf("Score: Player-G - %d | Computer-G - %d | Draws - %d\n", playerGWins, computerGWins, drawss);
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf(" %c ", board[i][j]);
            if (j < 2) printf("|");
        }
        printf("\n");
        if (i < 2) {
            printf("---|---|---\n");
        }
    }
    printf("\n");
}

// Function to check if the current player has won
int check_win(char player) {
    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
            (board[0][i] == player && board[1][i] == player && board[2][i] == player)) {
            return 1;
        }
    }

    // Check diagonals
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player)) {
        return 1;
    }

    return 0; // No winner yet
}

// Function to check if the board is full
int isBoardFull() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                return 0; // Board is not full
            }
        }
    }
    return 1; // Board is full
}

// Function to make the player's move
void makePlayerMove(char player) {
    int row, col;
    int validMove = 0;

    // Loop until a valid move is made
    while (!validMove) {
        setConsoleColor(10);
        printf("Your turn! Enter row and column (1, 2, 3): ");
        scanf("%d %d", &row, &col);
        row--; col--;  //converting to zero based
        // Check if the move is within bounds and the cell is empty
        if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == ' ') {
            board[row][col] = player;
            validMove = 1; // Valid move
        } else {
            setConsoleColor(12);
            printf("Invalid move, try again.\n");
        }
    }
}

// Function to make the computer's move
void makeComputerMove() {
    int row, col;
    int validMove = 0;

    // Loop until a valid move is made
    while (!validMove) {
        // Randomly choose an empty spot on the board for the computer's move
        row = rand() % 3;
        col = rand() % 3;

        if (board[row][col] == ' ') {
            board[row][col] = O;
            validMove = 1;
        }
    }

    //printf("Computer chooses row %d, column %d\n", row, col);
}

void GOD_Move() {
  // 1. Play for Immediate win
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == ' ') {
        board[i][j] = O;
        if (check_win(O)) {
          return;
        }
        board[i][j] = ' ';
      }
    }
  }

  // 2. Play for Immediate Block
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == ' ') {
        board[i][j] = X;
        if (check_win(X)) {
          board[i][j] = O;
          return;
        }
        board[i][j] = ' ';
      }
    }
  }

  // 3. Play Center if available
    if (board[1][1] == ' ') {
      board[1][1] = O;
      return;
    }

    // 4. Play Corner if available
    int corner[4][2] = {{0, 0},{0, 2},{2, 0},{2, 2}};
    for (int i = 0; i < 4; i++) {
      if (board[corner[i][0]][corner[i][1]] == ' ') {
        board[corner[i][0]][corner[i][1]] = O;
        return;
      }
    }

   // 5. Play first available move
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == ' ') {
        board[i][j] = O;
        return;
      }
    }
  }
}


// Function to handle multiplayer mode (two players)
void multiplayerMode() {
    int gameOver = 0;
    char currentPlayer = rand() % 2 == 0 ? X : O;
    while (!gameOver) {
        printBoard(1); // Display the updated board
        makePlayerMove(currentPlayer); // Get player's move
        printBoard(1); // Display the updated board

        // Check if the current player has won
        if (check_win(currentPlayer)) {
            setConsoleColor(2);
            printf("Player %c wins!\n", currentPlayer);
            if (currentPlayer == X) {
                player1Wins++;
            } else {
                player2Wins++;
            }
            gameOver = 1;
        } else if (isBoardFull()) {
            setConsoleColor(14);
            printf("It's a draw!\n");
            draws++;
            gameOver = 1;
        } else {
            // Switch to the next player
            currentPlayer = (currentPlayer == X) ? O : X;
           //currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
    }
}

// Function to handle single-player mode (player vs computer)
void singlePlayerMode() {
    int gameOver = 0;
    srand(time(0)); // Initialize random number generator for computer moves
    char currentPlayer = rand() % 2 == 0 ? X : O;
        printBoard(2); // Display the updated board
        while (!gameOver) {
            if (currentPlayer == X) {
               makePlayerMove(X);   // Get player's move
               printBoard(2);   // Display the updated board
               // Check if the player has won
               if (check_win(X)) {
                   playerWins++;
                   printBoard(2);
                   setConsoleColor(2);
                   printf("Congratulations! You win!!\n");
                   gameOver = 1;
                   // break;
                 }
              currentPlayer = O;
              }
           else { // Computer's turn
               makeComputerMove();  // Get computer's move
               printBoard(2);    // Display the updated board
               // Check if the computer has won
               if (check_win(O)) {
                  computerWins++;
                  printBoard(2);
                  setConsoleColor(4);
                  printf("Computer wins! Better luck next time.\n");
                  gameOver = 1;
                  //break;
                 }
               currentPlayer = X;
              }

            if (isBoardFull()) {
                draw++;
                printBoard(2);
                setConsoleColor(14);
                printf("It's a draw!\n");
                gameOver = 1;
                //break;
               }
            }
}

// Function to handle GOD mode (player vs computer)
void GOD_Mode() {
    int gameOver = 0;
    srand(time(0)); // Initialize random number generator for computer moves
    char currentPlayer = rand() % 2 == 0 ? X : O;
        printBoard(3); // Display the updated board
        while (!gameOver) {
            if (currentPlayer == X) {
               makePlayerMove(X);   // Get player's move
               printBoard(3);   // Display the updated board
               // Check if the player has won
               if (check_win(X)) {
                   playerGWins++;
                   printBoard(3);
                   setConsoleColor(2);
                   printf("Congratulations! You win!!\n");
                   gameOver = 1;
                   // break;
                 }
              currentPlayer = O;
              }
           else { // Computer's turn
               GOD_Move();  // Get computer's move
               printBoard(3);  // Display the updated board

               // Check if the computer has won
               if (check_win(O)) {
                  computerGWins++;
                  printBoard(3);
                  setConsoleColor(4);
                  printf("Computer wins! Better luck next time.\n");
                  gameOver = 1;
                  //break;
                 }
               currentPlayer = X;
              }

            if (isBoardFull()) {
                drawss++;
                printBoard(3);
                setConsoleColor(14);
                printf("It's a draw!\n");
                gameOver = 1;
                //break;
               }
            }
        }

// Function to ask the player if they want to play again
int playAgain() {
    char choice;
    setConsoleColor(5);
    printf("Do you want to play again? (Y/N): ");
    scanf(" %c", &choice); // Leading space before %c to consume any leftover newline character
    if (choice == 'Y' || choice == 'y') {
        initializeBoard(); // Reset the board
        return 1;
    }
    return 0;
}

// Main function to control the game
int TicTacToe() {
    int mode, continueGame = 1;
    while (continueGame) {
        // Ask the user to choose between multiplayer, single-player or God Mode
        setConsoleColor(13);
        printf("Welcome to Tic-Tac-Toe!\n");
        printf("Select game mode:\n");
        printf("1. Multiplayer\n");
        printf("2. Single-player (vs Computer)\n");
        printf("3. God Mode (Impossible to Win)\n");
        setConsoleColor(9);
        printf("Enter your choice (1, 2, or 3): ");
        scanf("%d", &mode);

        // Initialize the board for a new game
        initializeBoard();

        if (mode == 1) {
            multiplayerMode(); // Two-player mode
        } else if (mode == 2) {
            singlePlayerMode(); // Single-player mode
        } else if (mode == 3) {
             GOD_Mode(); // God Mode
        } else {
            setConsoleColor(12);
            printf("Invalid choice! Please select 1, 2, or 3.\n");
            continue;
        }

        // Ask if the players want to play again
        continueGame = playAgain();
    }
    setConsoleColor(15);
    printf("Thanks for playing!\n");

}


//=============================================================================================================================================================================

//CASINO
#define NUMBERS_ON_WHEEL 37
#define START_BALANCE 1000

// Function to place bets
void place_bet(int* bet_amt, int* bet_type, int* bet_choice, int* balance) {
    setConsoleColor(9);
    printf("Enter your bet amount (1-%d): ", *balance);
    scanf("%d", bet_amt);
    while (*bet_amt < 1 || *bet_amt > *balance) {
        setConsoleColor(12);
        printf("Invalid bet amount. Please bet between 1 and %d: ", *balance);
        scanf("%d", bet_amt);
    }
    setConsoleColor(10);
    printf("Select bet type:\n");
    printf("1. Bet on a single number (0-36)\n");        // Single number bet (35:1 payout)
    printf("2. Bet on Red/Black\n");                     // Red or Black bet (2:1 payout)
    printf("3. Bet on Even/Odd\n");                      // Odd or Even bet (2:1 payout)
    printf("4. Bet on High/Low (1-18 / 19-36)\n");       // High or low bet (2:1 payout)
    printf("5. Bet on Dozens (1-12, 13-24, 25-36)\n");   // Dozens bet (2:1 payout)
    printf("6. Exit\n");
    printf("Choose your option: ");
    scanf("%d", bet_type);

    switch (*bet_type) {
        case 1: // Bet on a number
            setConsoleColor(13);
            printf("Enter your number to bet on (0-36): ");
            scanf("%d", bet_choice);
            while (*bet_choice < 0 || *bet_choice > 36) {
                setConsoleColor(4);
                printf("Invalid number. Please choose a number between 0 and 36: ");
                scanf("%d", bet_choice);
            }
            break;
        case 2: // Bet on Red/Black
            setConsoleColor(13);
            printf("Enter your color bet (1 for Red, 2 for Black): ");
            scanf("%d", bet_choice);
            while (*bet_choice != 1 && *bet_choice != 2) {
                setConsoleColor(4);
                printf("Invalid choice. Enter 1 for Red or 2 for Black: ");
                scanf("%d", bet_choice);
            }
            break;
        case 3: // Bet on Even/Odd
            setConsoleColor(13);
            printf("Enter your bet (1 for Even, 2 for Odd): ");
            scanf("%d", bet_choice);
            while (*bet_choice != 1 && *bet_choice != 2) {
                setConsoleColor(4);
                printf("Invalid choice. Enter 1 for Even or 2 for Odd: ");
                scanf("%d", bet_choice);
            }
            break;
        case 4: // Bet on High/Low
            setConsoleColor(13);
            printf("Enter your bet (1 for Low 1-18, 2 for High 19-36): ");
            scanf("%d", bet_choice);
            while (*bet_choice != 1 && *bet_choice != 2) {
                setConsoleColor(4);
                printf("Invalid choice. Enter 1 for Low (1-18) or 2 for High (19-36): ");
                scanf("%d", bet_choice);
            }
            break;
        case 5: // Bet on Dozens (1-12, 13-24, 25-36)
            setConsoleColor(13);
            printf("Enter your dozen bet (1 for 1-12, 2 for 13-24, 3 for 25-36): ");
            scanf("%d", bet_choice);
            while (*bet_choice < 1 || *bet_choice > 3) {
                printf("Invalid choice. Enter 1 for 1-12, 2 for 13-24, or 3 for 25-36: ");
                scanf("%d", bet_choice);
            }
            break;
        case 6:
            setConsoleColor(13);
            printf("Exiting....");
                exit(0);
        default:
            setConsoleColor(13);
            printf("Invalid bet type! Exiting.\n");
            exit(0);
    }
}

// Function to simulate the roulette wheel spin
int spin_wheel() {
    return rand() % NUMBERS_ON_WHEEL; // Random number between 0 and 36
}

// Function to display the player's balance
void disp_balance(int balance) {
    setConsoleColor(1);
    printf("\nYour current balance: %d\n", balance);
}

// Function to determine if a number is red
int is_red(int number, int* red_numbers) {
    for (int i = 0; i < 18; i++) {
        if (red_numbers[i] == number) {
            return 1; // Red
        }
    }
    return 0; // Black
}

// Function to generate the red and black numbers dynamically
void generate_red_black(int* red_numbers, int* black_numbers) {
    int numbers[NUMBERS_ON_WHEEL] = {0};
    int red_count = 0, black_count = 0;

    while (red_count < 18) {
        int num = rand() % NUMBERS_ON_WHEEL;
        if (num != 0 && !numbers[num]) {
            red_numbers[red_count++] = num;
            numbers[num] = 1;
        }
    }

    for (int i = 1; i < NUMBERS_ON_WHEEL; i++) {
        if (!numbers[i]) {
            black_numbers[black_count++] = i;
        }
    }
}

// Function to evaluate the result of the spin
int result(int spin_result, int bet_type, int bet_choice, int* red_numbers, int* black_numbers) {
    switch (bet_type) {
        case 1: // Bet on a number
            return spin_result == bet_choice;
        case 2: // Bet on Red/Black
            return (is_red(spin_result, red_numbers) && bet_choice == 1) || (!is_red(spin_result, red_numbers) && bet_choice == 2);
        case 3: // Bet on Even/Odd
            return (spin_result % 2 == 0 && bet_choice == 1) || (spin_result % 2 != 0 && bet_choice == 2);
        case 4: // Bet on High/Low
            return (bet_choice == 1 && spin_result >= 1 && spin_result <= 18) ||
                   (bet_choice == 2 && spin_result >= 19 && spin_result <= 36);
        case 5: // Bet on Dozens
            return (bet_choice == 1 && spin_result >= 1 && spin_result <= 12) ||
                   (bet_choice == 2 && spin_result >= 13 && spin_result <= 24) ||
                   (bet_choice == 3 && spin_result >= 25 && spin_result <= 36);
        default:
            return 0; // No win
    }
}

// Function to display game results
void print_results(int spin_result, int bet_type, int bet_choice, int bet_amt, int* balance, int* red_numbers, int* black_numbers) {
    int win = result(spin_result, bet_type, bet_choice, red_numbers, black_numbers);

    if (win) {
        setConsoleColor(10);
        printf("Congratulations! You won!\n");
        int payout = 0;
        switch (bet_type) {
            case 1: payout = 35;
                    break; // Single number pays 35:1
            case 2: case 3: case 4: case 5: case 6: payout = 2;
                    break; // Other bets pay 2:1
        }
        int winnings = bet_amt * payout;
        setConsoleColor(10);
        printf("You earned %d units!\n", winnings);
        *balance += winnings;
    } else {
        setConsoleColor(12);
        printf("Sorry, you lost. Better luck next time!\n");
    }
}
int Casino_Roulette() {
    srand(time(0)); // Seed the random number generator for spin and color determination

    int balance = START_BALANCE;
    int bet_amt, bet_type, bet_choice, spin_result;
    int cont_playing = 1;
    int red_numbers[18], black_numbers[18];
    setConsoleColor(5);
    printf("\n----- Welcome to Advanced European Roulette -----\n");

    generate_red_black(red_numbers, black_numbers); // Generate red and black numbers dynamically

    while (cont_playing) {
        disp_balance(balance);

        if (balance <= 0) {
            setConsoleColor(12);
            printf("You don't have enough balance to continue playing!\n");
            break;
        }

        place_bet(&bet_amt, &bet_type, &bet_choice, &balance);

        // Check if the player can afford the bet
        if (bet_amt > balance) {
            setConsoleColor(12);
            printf("You cannot afford this bet. Please enter a valid amount.\n");
            continue;
        }

        // Deduct the bet amount from the balance
        balance -= bet_amt;

        // Spin the roulette wheel
        spin_result = spin_wheel();
        setConsoleColor(14);
        printf("The wheel spun and landed on: %d", spin_result);

        // Determine the color of the spin result
        if (is_red(spin_result, red_numbers)) {
            printf(" (Red)\n");
        } else {
            printf(" (Black)\n");
        }

        // Evaluate the result and print the outcome
        print_results(spin_result, bet_type, bet_choice, bet_amt, &balance, red_numbers, black_numbers);

        // Ask if the player wants to continue playing
        setConsoleColor(6);
        printf("Do you want to continue playing? (1 for Yes, 0 for No): ");
        scanf("%d", &cont_playing);
    }
}

 //=============================================================================================================================================================================

void printPattern3() {
    setConsoleColor(11);
    printf("  /$$$$$$  /$$   /$$ /$$$$$$$$  /$$$$$$  /$$$$$$$$       /$$   /$$ /$$$$$$$$ /$$   /$$ /$$   /$$  /$$$$$$ \n");
    printf(" /$$__  $$| $$  | $$| $$_____/ /$$__  $$|__  $$__/      | $$$ | $$| $$_____/| $$  / $$| $$  | $$ /$$__  $$\n");
    printf("| $$  \\ $$| $$  | $$| $$      | $$  \\__/   | $$         | $$$$| $$| $$      |  $$/ $$/| $$  | $$| $$  \\__/\n");
    printf("| $$  | $$| $$  | $$| $$$$$   |  $$$$$$    | $$         | $$ $$ $$| $$$$$    \\  $$$$/ | $$  | $$|  $$$$$$ \n");
    printf("| $$  | $$| $$  | $$| $$__/    \\____  $$   | $$         | $$  $$$$| $$__/     >$$  $$ | $$  | $$ \\____  $$\n");
    printf("| $$/$$ $$| $$  | $$| $$       /$$  \\ $$   | $$         | $$\\  $$$| $$       /$$/\\  $$| $$  | $$ /$$  \\ $$\n");
    printf("|  $$$$$$/|  $$$$$$/| $$$$$$$$|  $$$$$$/   | $$         | $$ \\  $$| $$$$$$$$| $$  \\ $$|  $$$$$$/|  $$$$$$/\n");
    printf(" \\____ $$$ \\______/ |________/ \\______/    |__/         |__/  \\__/|________/|__/  |__/ \\______/  \\______/ \n");
    printf("      \\__/                                                                                                 \n");
}

void printPattern() {
    setConsoleColor(11);
    printf("$$\\      $$\\ $$$$$$\\ $$\\   $$\\ $$$$$$$$\\        $$$$$$\\  $$\\      $$\\ $$$$$$$$\\ $$$$$$$$\\ $$$$$$$\\  $$$$$$$$\\ $$$$$$$\\  \n");
    printf("$$$\\    $$$ |\\_$$  _|$$$\\  $$ |$$  _____|      $$  __$$\\ $$ | $\\  $$ |$$  _____|$$  _____|$$  __$$\\ $$  _____|$$  __$$\\ \n");
    printf("$$$$\\  $$$$ |  $$ |  $$$$\\ $$ |$$ |            $$ /  \\__|$$ |$$$\\ $$ |$$ |      $$ |      $$ |  $$ |$$ |      $$ |  $$ | \n");
    printf("$$\\$$\\$$ $$ |  $$ |  $$ $$\\$$ |$$$$$\\          \\$$$$$$\\  $$ $$ $$\\$$ |$$$$$\\    $$$$$$\\   $$$$$$$  |$$$$$\\    $$$$$$$  | \n");
    printf("$$ \\$$$  $$ |  $$ |  $$ \\$$$$ |$$  __|          \\____$$\\ $$$$  _$$$$ |$$  __|   $$  __|   $$  ____/ $$  __|   $$  __$$<  \n");
    printf("$$ |\\$  /$$ |  $$ |  $$ |\\$$$ |$$ |            $$\\   $$ |$$$  / \\$$$ |$$ |      $$ |      $$ |      $$ |      $$ |  $$ | \n");
    printf("$$ | \\_/ $$ |$$$$$$\\ $$ | \\$$ |$$$$$$$$$\\      \\$$$$$$  |$$  /   \\$$ |$$$$$$$$\\ $$$$$$$$\  $$ |      $$$$$$$$\\ $$ |  $$ | \n");
    printf("\\__|     \\__|\\______|\\__|  \\__|\\________|       \\______/ \\__/     \\__|\\________|\\________|\\__|      \\________|\\__|  \\__| \n");
}

void printAsciiArt2() {
    setConsoleColor(11);
    printf("$$\\   $$\\  $$$$$$\\  $$\\   $$\\  $$$$$$\\        $$\\      $$\\  $$$$$$\\  $$\\   $$\\ \n");
    printf("$$ |  $$ |$$  __$$\\ $$$\\  $$ |$$  __$$\\       $$$\\    $$$ |$$  __$$\\ $$$\\  $$ | \n");
    printf("$$ |  $$ |$$ /  $$ |$$$$\\ $$ |$$ /  \\__|      $$$$\\  $$$$ |$$ /  $$ |$$$$\\ $$ | \n");
    printf("$$$$$$$$ |$$$$$$$$ |$$ $$\\$$ |$$ |$$$$\\       $$\\$$\\$$ $$ |$$$$$$$$ |$$ $$\\$$ | \n");
    printf("$$  __$$ |$$  __$$ |$$ \\$$$$ |$$ |\\_$$ |      $$ \\$$$  $$ |$$  __$$ |$$ \\$$$$ | \n");
    printf("$$ |  $$ |$$ |  $$ |$$ |\\$$$ |$$ |  $$ |      $$ |\\$  /$$ |$$ |  $$ |$$ |\\$$$ | \n");
    printf("$$ |  $$ |$$ |  $$ |$$ | \\$$ |\\$$$$$$  |      $$ | \\_/ $$ |$$ |  $$ |$$ | \\$$ | \n");
    printf("\\__|  \\__|\\__|  \\__|\\__|  \\__| \\______/       \\__|     \\__|\\__|  \\__|\\__|  \\__| \n");
}

void printAsciiArt() {
    setConsoleColor(11);
    printf("$$$$$$$\\   $$$$$$\\   $$$$$$\\  $$\\   $$\\       $$$$$$$\\   $$$$$$\\  $$$$$$$\\  $$$$$$$$\\ $$$$$$$\\         $$$$$$\\   $$$$$$\\  $$$$$$\\  $$$$$$\\   $$$$$$\\   $$$$$$\\  $$$$$$$\\  \n");
    printf("$$  __$$\\ $$  __$$\\ $$  __$$\\ $$ | $$  |      $$  __$$\\ $$  __$$\\ $$  __$$\\ $$  _____|$$  __$$\\       $$  __$$\\ $$  __$$\\ \\_$$  _|$$  __$$\\ $$  __$$\\ $$  __$$\\ $$  __$$\\ \n");
    printf("$$ |  $$ |$$ /  $$ |$$ /  \\__|$$ |$$  /       $$ |  $$ |$$ /  $$ |$$ |  $$ |$$ |      $$ |  $$ |      $$ /  \\__|$$ /  \\__|  $$ |  $$ /  \\__|$$ /  \\__|$$ /  $$ |$$ |  $$ | \n");
    printf("$$$$$$$  |$$ |  $$ |$$ |      $$$$$  /        $$$$$$$  |$$$$$$$$ |$$$$$$$  |$$$$$\\    $$$$$$$  |      \\$$$$$$\\  $$ |        $$ |  \\$$$$$$\\  \\$$$$$$\\  $$ |  $$ |$$$$$$$  | \n");
    printf("$$  __$$< $$ |  $$ |$$ |      $$  $$<         $$  ____/ $$  __$$ |$$  ____/ $$  __|   $$  __$$<        \\____$$\\ $$ |        $$ |   \\____$$\\  \\____$$\\ $$ |  $$ |$$  __$$< \n");
    printf("$$ |  $$ |$$ |  $$ |$$ |  $$\\ $$ |\\$$\\        $$ |      $$ |  $$ |$$ |      $$ |      $$ |  $$ |      $$\\   $$ |$$ |  $$\\   $$ |  $$\\   $$ |$$\\   $$ |$$ |  $$ |$$ |  $$ | \n");
    printf("$$ |  $$ | $$$$$$  |\\$$$$$$  |$$ | \\$$\\       $$ |      $$ |  $$ |$$ |      $$$$$$$$\\ $$ |  $$ |      \\$$$$$$  |\\$$$$$$  |$$$$$$$\\ \\$$$$$$  | $$$$$$  |$$ |  $$ | \n");
    printf("\\__|  \\__| \\______/  \\______/ \\__|  \\__|      \\__|      \\__|  \\__|\\__|      \\________|\\__|  \\__|       \\______/  \\______/ \\______| \\______/  \\______/  \\______/ \\__|  \\__| \n");
}


void printAsciiArt1() {
    setConsoleColor(11);
    printf("$$$$$$$$\\ $$$$$$\\  $$$$$$\\        $$$$$$$$\\  $$$$$$\\   $$$$$$\\        $$$$$$$$\\  $$$$$$\\  $$$$$$$$\\ \n");
    printf("\\__$$  __|\\_$$  _|$$  __$$\\       \\__$$  __|$$  __$$\\ $$  __$$\\       \\__$$  __|$$  __$$\\ $$  _____|\n");
    printf("   $$ |     $$ |  $$ /  \\__|         $$ |   $$ /  $$ |$$ /  \\__|         $$ |   $$ /  $$ |$$ |      \n");
    printf("   $$ |     $$ |  $$ |               $$ |   $$$$$$$$ |$$ |               $$ |   $$ |  $$ |$$$$$\\    \n");
    printf("   $$ |     $$ |  $$ |               $$ |   $$  __$$ |$$ |               $$ |   $$ |  $$ |$$  __|   \n");
    printf("   $$ |     $$ |  $$ |  $$\\          $$ |   $$ |  $$ |$$ |  $$\\          $$ |   $$ |  $$ |$$ |      \n");
    printf("   $$ |   $$$$$$\\ \\$$$$$$  |         $$ |   $$ |  $$ |\\$$$$$$  |         $$ |    $$$$$$  |$$$$$$$$$\\ \n");
    printf("   \\__|   \\______| \\______/          \\__|   \\__|  \\__| \\______/          \\__|    \\______/ \\________| \n");
}



void printPattern1() {
    setConsoleColor(11);
    printf(" $$$$$$\\   $$$$$$\\   $$$$$$\\  $$$$$$\\ $$\\   $$\\  $$$$$$\\                  $$$$$$$\\   $$$$$$\\  $$\\   $$\\ $$\\       $$$$$$$$\\ $$$$$$$$\\ $$$$$$$$\\ $$$$$$$$\\ \n");
    printf("$$  __$$\\ $$  __$$\\ $$  __$$\\ \\_$$  _|$$$\\  $$ |$$  __$$\\                 $$  __$$\\ $$  __$$\\ $$ |  $$ |$$ |      $$  _____|\\__$$  __|\\__$$  __|$$  _____|\n");
    printf("$$ /  \\__|$$ /  $$ |$$ /  \\__|  $$ |  $$$$\\ $$ |$$ /  $$ |      $$\\       $$ |  $$ |$$ /  $$ |$$ |  $$ |$$ |      $$ |         $$ |      $$ |   $$ |      \n");
    printf("$$ |      $$$$$$$$ |\\$$$$$$\\    $$ |  $$ $$\\$$ |$$ |  $$ |      \\__|      $$$$$$$  |$$ |  $$ |$$ |  $$ |$$ |      $$$$$\\       $$ |      $$ |   $$$$$\\    \n");
    printf("$$ |      $$  __$$ | \\____$$\\   $$ |  $$ \\$$$$ |$$ |  $$ |                $$  __$$< $$ |  $$ |$$ |  $$ |$$ |      $$  __|      $$ |      $$ |   $$  __|   \n");
    printf("$$ |  $$\\ $$ |  $$ |$$\\   $$ |  $$ |  $$ |\\$$$ |$$ |  $$ |      $$\\       $$ |  $$ |$$ |  $$ |$$ |  $$ |$$ |      $$ |         $$ |      $$ |   $$ |      \n");
    printf("\\$$$$$$  |$$ |  $$ |\\$$$$$$  |$$$$$$\\ $$ | \\$$ | $$$$$$  |      \\__|      $$ |  $$ | $$$$$$  |\\$$$$$$  |$$$$$$$$\\ $$$$$$$$\\    $$ |      $$ |   $$$$$$$$\\ \n");
    printf(" \\______/ \\__|  \\__| \\______/ \\______|\\__|  \\__| \\______/                 \\__|  \\__| \\______/  \\______/ \\________|\\________|   \\__|      \\__|   \\________|\n");
}


 int main() {
 srand(time(NULL));
 printPattern3();
 int choice;
 while (1) {
    setConsoleColor(5);
    printf("\n\nWelcome to the world of QUEST NEXUS : The Multiverse of Gaming Adventures \n");
    printf("Enter 1 to play MINESWEEPER \n");
    printf("Enter 2 to play HANGMAN \n");
    printf("Enter 3 to play ROCK PAPER SCISSOR \n");
    printf("Enter 4 to play TIC TAC TOE \n");
    printf("Enter 5 to play CASINO - ROULETTE \n");
    printf("Enter 6 to exit \n");
    setConsoleColor(10);
    printf("Enter your choice from 1 to 6 : \n");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
           printPattern();
           mineSweeper();
            break;
    case 2:
           printAsciiArt2();
            Hangman();
            break;
    case 3:
           printAsciiArt();
           RockPaperScissors();
            break;
    case 4:
           printAsciiArt1();
           TicTacToe();
            break;
    case 5:
           printPattern1();
            Casino_Roulette();
            break;
    case 6:
            return 0;
    default:
           setConsoleColor(12);
           printf("Invalid choice. Try again. \n");
    }
 }
 return 0;
}
