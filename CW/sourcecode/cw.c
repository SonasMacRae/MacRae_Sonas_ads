#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// Nodes for the doubly linked list to store the board states
// during run time
struct node
{
  char *board;
  struct node *previous;
  struct node *next;
};

// Nodes for single linked list, used for storing all board states
struct history
{
  char *board;
  struct history *next;
  char type;
};

// Used to point to a certain node, keeps track of the presented
// board state
struct node *pointer;
// Initialize the nodes for the linked lists
struct node *list;
struct history *historyNodes;

// Names for the players
char name1[10] = "Player 1";
char name2[10] = "Player 2";

// Initialize the functions that interact with the linked lists
int undo(struct node **list);
int redo(struct node **list);
void append(struct node **list, char* board);
void addToHistory(struct history **historyNodes, char* board, char type);
void deleteAfter();
void writeToFile(struct history **historyNodes, char* fileName, int bot);
void resetBoard(struct node **list, struct history **historyNodes);


// Used to append a node to the doubly linked list
void append(struct node **list, char* board)
{
  // Sets new node structs to the start of the list
  struct node *temp, *current = *list;

  // Checks if the list is empty, if so, add a new node to the
  // start of the list
  if (*list == NULL)
  {
    temp = (struct node*) malloc(sizeof(struct node) + 1);
    memset(temp, 0, sizeof(struct node) + 1);

    // Whenever a node is added to the doubly linked list, a copy
    // of the node is appended to the history list
    addToHistory(&historyNodes, "123456789", 'n');

    temp -> board = malloc(strlen(board) + 1);
    strcpy(temp -> board, "123456789");

    temp -> next = NULL;
    temp -> previous = NULL;

    *list = temp;

    // Updates the pointer node to the new node
    pointer = temp;
  }
  // This code is executed if the list is not empty
  struct node *tempAdd, *currentAdd = *list;

  tempAdd = (struct node*) malloc(sizeof(struct node) + 1);
  memset(tempAdd, 0, sizeof(struct node) + 1);

  tempAdd -> next = NULL;
  tempAdd -> previous = pointer;
  tempAdd -> previous -> next = tempAdd;

  tempAdd -> board = malloc(strlen(board) + 1);
  memset(tempAdd -> board, 0, 10);
  strcpy(tempAdd -> board, board);

  // Updates the pointer node to the new node
  pointer = tempAdd;
}


// Function to add a node to the history linked list, works similiarly
// to the append function
void addToHistory(struct history **historyNodes, char* board, char type)
{
  struct history *temp, *current = *historyNodes;

  if (*historyNodes == NULL)
  {
    temp = (struct history*) malloc(sizeof(struct history) + 1);
    memset(temp, 0, sizeof(struct history) + 1);
    temp -> next = NULL;

    temp -> board = malloc(strlen(board) + 1);
    memset(temp -> board, 0, 10);
    strcpy(temp -> board, board);

    temp -> type = type;

    *historyNodes = temp;
  }

  else
  {
    while(current -> next != NULL)
      current = current -> next;

    temp = (struct history*) malloc(sizeof(struct history) + 1);
    memset(temp, 0, sizeof(struct history) + 1);
    temp -> next = NULL;

    temp -> board = malloc(10);
    memset(temp -> board, 0, 10);
    strcpy(temp -> board, board);

    temp -> type = type;

    current -> next = temp;

    printf("size of board %zu\nsize of next %zu\nsize of type %zu\nsize of whole node %zu \n", sizeof(temp->board), sizeof(temp->next), sizeof(temp->type), sizeof(temp));
  }
}


// Function to undo the current board state, reverting to the
// previous board state
int undo(struct node **list)
{
  // If the previous node in the doubly linked list isn't empty
  // the pointer node points one index backwards in the list from
  // its current position
  if (pointer -> previous != NULL)
  {
    pointer = pointer -> previous;
    addToHistory(&historyNodes, pointer -> board, 'u');
    return 0;
  }

  printf("There is no state to undo back to\n");
  return 1;
}


// Works the same as undo, but it points one index forward
int redo(struct node **list)
{
  if (pointer -> next != NULL)
  {
    pointer = pointer -> next;
    addToHistory(&historyNodes, pointer -> board, 'r');
    return 0;
  }

  printf("There is no state to redo forward to\n");
  return 1;
}


// Used to delete all the nodes infront of the pointer node
// in the doubly linked list
// BROKEN --------------------
void deleteAfter()
{
  if (pointer -> next != NULL)
  {
    struct node *next, *current = pointer -> next;

    while (current != NULL)
    {
      next = current->next;
      free(current);
      current = next;
    }

    pointer -> next = NULL;
  }
}


// Prints all of the board states from the history linked list to the file
// as well as information stored about each board state
void writeToFile(struct history **historyNodes, char* fileName, int bot)
{
  struct history *temp = *historyNodes;

  // Opens the file
  FILE * fp;
  fp = fopen("output.txt⁩","a");

  // If the player played against Florp (the bot), that is written to the file
  if (bot == 0)
    fprintf(fp, "&^%s^vs^%s^%s\n", name1, "Florp", fileName);

  // Else the 2 player names are printed along with the name of the file
  else
    fprintf(fp, "&^%s^vs^%s^%s\n", name1, name2, fileName);

  // Loop to the end of the history linked list printing each board state
  // with the information stored about each board state
  while (temp -> next != NULL)
  {
    fprintf(fp, "%c %s ", temp -> type, temp -> board);
    temp = temp -> next;
  }

  fprintf(fp, "%c %s \n", temp -> type, temp -> board);
  fclose(fp);
}


// Function to empty the history linked list and the doubly
// linked list, so a new game can be played without restarting
// the program
void resetBoard(struct node **list, struct history **historyNodes)
{
  struct node *currentNode = *list;
  struct node *nextNode;

  // Empty the doubly linked list
  while (currentNode != NULL)
  {
   nextNode = currentNode -> next;
   free(currentNode);
   currentNode = nextNode;
  }

  *list = NULL;

  struct history *currentHistory = *historyNodes;
  struct history *nextHistory;

  // Empty the history linked list
  while (currentHistory != NULL)
  {
   nextHistory = currentHistory -> next;
   free(currentHistory);
   currentHistory = nextHistory;
  }

  *historyNodes = NULL;
}


// Initialize functions for the game aspect
void Help();
void PrintSlow(char* input);
void Delay(int speed);
void RemoveNewline(char *str);
int Bot();
int Check();
void RenderBoard();
int Move(char player, int position, char type);
void ChooseNames();
void RunGameSinglePlayer();
void RunGameMultiPlayer();
void ReplayGame();
int StartGame();


// Prints helpful information about the game
void Help()
{
  PrintSlow("This game is 'Tic-tac-toe', also known as 'noughts and crosses', players take turn ");
  printf("\n");
  PrintSlow("to place their assigned symbol on the board, an example board is displayed below: ");
  printf("\n\n");
  RenderBoard();
  PrintSlow("The aim of this game is to place 3 symbols in a row, either horizontally, vertically ");
  printf("\n");
  PrintSlow("or diagonally, squares with symbols can not be overwritten. ");
  printf("\n\n");
  PrintSlow("Blorp is the bot who was created entirely to play this game at the will of the users, ");
  printf("\n");
  PrintSlow("Blorp is infact very enthusiastic and loves his job. ");
  printf("\n");
}


// Prints strings slowly, this is for style
void PrintSlow(char* input)
{
  for (int i = 0; i < strlen(input); i++)
  {
    printf("%c", input[i]);
    fflush(stdout);
    Delay(40000);
  }
}

void Delay(int speed)
{
  clock_t startTime = clock();

  while(clock() < startTime + speed)
    ;
}


// Removes newline characters from strings
void RemoveNewline(char *str)
{
  int len = strlen(str);

  if (len > 0 && str[len - 1] == '\n')
    str[len - 1] = '\0';
}


// The bot examines the board and returns an int that corresponds to
// a square on the game board in which it will make its play
int Bot()
{
  // Fetches the current board state from the pointer node
  char *board = pointer -> board;

  // 2D array of all of all of the horizontals, verticals and diagonals of the board
  int disp[8][3] = {{0, 1, 2},{3, 4, 5},{6, 7, 8},{0, 3, 6},{1, 4, 7},{2, 5, 8},{0, 4, 8},{2, 4, 6}};
  int highScore = 0;
  int xCounter, oCounter, blankCounter, score, tempInt = 0;

  // loops through every array in the 2D array
  for (int i = 0; i < 8; i++)
  {
    oCounter = 0;
    xCounter = 0;
    score = 0;
    blankCounter = 0;

    // Gives each array in the 2D array a score
    for (int j = 0; j < 3; j++)
    {
      if (board[disp[i][j]] == 'X')
        xCounter++;

      else if (board[disp[i][j]] == 'O')
        oCounter++;

      else
        blankCounter++;
    }

    if (oCounter == 2 && blankCounter == 1)
      score = 5;

    else if (xCounter == 2 && blankCounter == 1)
      score = 4;

    else if (oCounter == 1 && xCounter == 0 && blankCounter == 2)
      score = 3;

    else if (xCounter == 1 && oCounter == 1 && blankCounter == 1)
      score = 2;

    else if (blankCounter == 0)
      score = 0;

    else
      score = 1;

    if (score > highScore)
    {
      highScore = score;
      tempInt = i;
    }
  }

  int flag = 1;

  // Chooses an index from the highest scoring array
  while (flag != 0)
  {
    int s = rand() % 3;

    if (board[disp[tempInt][s]] != 'X' && board[disp[tempInt][s]] != 'O')
    {
      flag = 0;

      int finalInt = board[disp[tempInt][s]] - '0';

      PrintSlow("The bot chose square ");

      printf(" %d\n", finalInt);

      return finalInt;
    }
  }

  return 1;
}


// Prints the board that the pointer node is pointing to
void RenderBoard()
{
  char* board;

  // If the pointer is NULL, create an empty board
  if (pointer == NULL)
  {
    pointer = (struct node*) malloc(sizeof(struct node));
    memset(pointer, 0, sizeof(struct node));
    pointer -> board = "123456789";
  }

  board = pointer -> board;

  // Print the board
  for (int i = 0; i < strlen(board); i++)
  {
    if (board[i] == 'O')
      printf("O  ");

    else if (board[i] == 'X')
      printf("X  ");

    else
      printf("%d  ", i + 1);

    if (((i + 1) % 3) == 0)
      printf("\n\n");
  }

  printf("\n");
}


// Checks the current board state to see if a player has won the game
int Check()
{
  int index = 0;

  char *board = pointer -> board;
  int counter = 0;

  for(int i = 0; i < 9; i++)
  {
    if (board[i] == 'X' || board[i] == 'O')
      counter++;
  }

  if(counter == 9)
  {
    printf("The game ended as a draw\n");
    return 0;
  }

  for(int i = 0; i < 3; i++)
  {
    // Check for Horizontal
    if ((board[index] == board[index + 1]) && (board[index] == board[index + 2]))
    {
      printf("%c wins \n", board[index]);
      return 0;
    }
    // Check for Vertical
    if ((board[i] == board[i + 3]) && (board[i] == board[i + 6]))
    {
      printf("%c wins \n", board[i]);
      return 0;
    }

    index += 3;
  }
  // Check for diagonal
  if ((board[0] == board[4] && board[0] == board[8]) || (board[2] == board[4] && board[2] == board[6]))
  {
    printf("%c wins \n", board[4]);
    return 0;
  }

  return 1;
}


// Used when a move has been played
int Move(char player, int position, char type)
{
  char board[10];

  // If the square inputted has been marked, return false
  if (pointer -> board[position - 1] == 'O' || pointer -> board[position - 1] == 'X')
    return 1;

  if (position > 9 || position < 0)
    return 1;


  // Update the board
  for (int i = 0; i <= 8; i++)
  {
    if (i == position - 1)
    {
      board[i] = player;
    }
    else
    {
      board[i] = pointer -> board[i];
    }
  }

  board[9] = '\0';

  // Checks if a move has been done after the undo function
  // has been called, this deleted all the old nodes in the list
  // after the current node, stops multiple paths from being created
  if (pointer -> next != NULL)
    deleteAfter();

  // Append the new board to the doubly linked list and to
  // the history list
  append(&list, board);
  addToHistory(&historyNodes, board, type);

  return 0;
}


// Updates the names for the players
void ChooseNames()
{
  char temp[20];

  printf("Enter the name for player 1: ");
  fgets(temp, 20, stdin);
  RemoveNewline(temp);

  strcpy(name1, temp);

  printf("Enter the name for player 2: ");
  fgets(temp, 20, stdin);
  RemoveNewline(temp);

  strcpy(name2, temp);
}


// When 2 players play against each other this function is called
void RunGameMultiPlayer()
{
  char temp[10];
  int counter = 2;
  char tempPlayer = 'X';
  char tempPlayerName[20];

  list = NULL;

  // Loop until the game is over
  while(Check() != 0)
  {
    // Prints the board to the console
    RenderBoard();

    // The counter is used to keep track of whose turn it is,
    // when the counter is even it's player X's turn, odd for O's
    if (counter % 2 == 0)
    {
      tempPlayer = 'X';
      strcpy(tempPlayerName, name1);
    }
    else
    {
      tempPlayer = 'O';
      strcpy(tempPlayerName, name2);
    }

    printf("It is %s's (%c) turn, choose a square: ", tempPlayerName, tempPlayer);
    fgets(temp, 10, stdin);
    RemoveNewline(temp);

    if (strcmp (temp, "") == 0)
    {
      printf("Enter a number from 1 to 9\n");
      continue;
    }

    // Calls the undo function
    if (strcmp(temp, "undo") == 0)
    {
      if(undo(&list) == 1)
        continue;
      counter++;
      continue;
    }

    if (strcmp(temp, "redo") == 0)
    {
      if(redo(&list) == 1)
        continue;
      counter++;
      continue;
    }

    int flag = 0;
    for (int i = 0; i < strlen(temp); i++)
    {
      if(!isdigit(temp[i]))
        flag = 1;
    }

    if (flag == 1)
    {
      printf("Enter a valid square number\n");
      continue;
    }

    // Call the move function
    if (Move(tempPlayer, atoi(temp), tempPlayer) == 1)
    {
      printf("Enter a valid square number\n");
      continue;
    }

    // Increment the counter so the next player has their turn
    counter++;
  }
}


void RunGameSinglePlayer()
{
  PrintSlow("Hello there, I am Blorp the Tic-tac-toe playing robot and it is my job ");
  printf("\n");
  PrintSlow("to play this game. ");
  printf(" (press the enter key to continue) \n");
  getchar();

  int counter = 2;

  char temp[10];

  list = NULL;

  while(Check() != 0)
  {
    RenderBoard();

    if (counter % 2 == 0)
    {
      printf("Choose a square: ");
      fgets(temp, 10, stdin);
      RemoveNewline(temp);

      if (strcmp (temp, "") == 0)
      {
        printf("Enter a number from 1 to 9\n");
        continue;
      }

      if (strcmp(temp, "undo") == 0)
      {
        if(undo(&list) == 1)
          continue;
        counter++;
        continue;
      }

      if (strcmp(temp, "redo") == 0)
      {
        if(redo(&list) == 1)
          continue;
        counter++;
        continue;
      }

      int flag = 0;
      for (int i = 0; i < strlen(temp); i++)
      {
        if(!isdigit(temp[i]))
          flag = 1;
      }

      if (flag == 1)
      {
        printf("Enter a valid square number\n");
        continue;
      }

      if (Move('X', atoi(temp), 'X') == 1)
      {
        printf("Enter a valid square number\n");
        continue;
      }
    }

    else
      Move('O', Bot(), 'O');

    counter++;
  }
}


// Used to replay a specific game
void ReplayGame()
{
  char buffer[255];
  int counter = 1;

  FILE * fp;
  fp = fopen("output.txt⁩","r");

  // Loop through the file line by line
  while(fgets(buffer, 255, (FILE*) fp))
  {
    if (buffer[0] == '&')
    {
      char stringArray[5][30];
      const char s[2] = "^";
      char *split = strtok(buffer, s);

      int tempCounter = 0;

      while (split != NULL)
      {
        strcpy(stringArray[tempCounter], split);
        split = strtok(NULL, s);
        tempCounter++;
      }

      printf("%d -> ", counter);

      for (int i = 1; i < 5; i++)
        printf("%s ", stringArray[i]);

      printf("\n");

      counter++;
    }
  }

  rewind(fp);

  int number;
  printf("\n\nEnter a Number corresponding to the game you would like to replay\n\n");
  scanf("%d", &number);

  int lineCounter = 0;

  while(fgets(buffer, 255, (FILE*) fp))
  {
    if (lineCounter == ((number * 2) - 1))
    {
      char space[2] = " ";
      char *tempString = strtok(buffer, space);
      char type;
      char prevBoard[10] = "987654321";
      char currentBoard[10] = "123456789";

      pointer = (struct node*) malloc(sizeof(struct node));

      while(tempString != NULL)
      {
        if (strlen(tempString) == 1)
        {
          type = tempString[0];

          tempString = strtok(NULL, space);
          continue;
        }

        strcpy(prevBoard, currentBoard);
        pointer -> board = tempString;

        strcpy(currentBoard, tempString);
        tempString = strtok(NULL, space);

        if (type == 'X' || type == 'O')
        {
          for (int i = 0; i < 8; i++)
          {
            if (currentBoard[i] != prevBoard[i])
              printf("%c made a move on square %d.\n", type, i + 1);
          }
        }

        else if (type == 'u')
          printf("An undo was made\n");

        else if (type == 'r')
          printf("A redo was made\n");

        RenderBoard();
        Delay(1000000);
      }

      break;
    }

    else
      lineCounter++;
  }

  fclose(fp);
}


// This represents the main menu
int StartGame()
{
  char temp[10];

  printf("\n\nWelcome to the game, this is the main menu\n\n");

  printf("1 - Single player\n2 - Multi-player\n3 - Choose names\n4 - replay games\n5 - Help\n6 - Quit ");
  fgets(temp, 10, stdin);

  printf("\n\n");

  switch(atoi(temp))
  {
    case 1:
      RenderBoard();
      RunGameSinglePlayer();
      RenderBoard();
      printf("Enter a file name: (enter 0 to generate a default name) ");
      fgets(temp, 10, stdin);
      RemoveNewline(temp);
      writeToFile(&historyNodes, temp, 0);
      return 2;

    case 2:
      RenderBoard();
      RunGameMultiPlayer();
      printf("Enter a file name: (enter 0 to generate a default name) ");
      fgets(temp, 10, stdin);
      RemoveNewline(temp);
      writeToFile(&historyNodes, temp, 1);
      return 2;

    case 3:
      ChooseNames();
      return 0;

    case 4:
      ReplayGame();
      return 0;

    case 5:
      Help();
      return 0;

    case 6:
      return 1;

    default:
      printf("Command not recognised.\n");
      return 0;
  }

  return 0;
}

int main()
{
  int flag = 0;

  while (flag != 1)
  {
    flag = StartGame();

    if (flag == 2)
      resetBoard(&list, &historyNodes);

    pointer = NULL;
  }
}

// What needs to be worked on ---->

// Allow the user to reset the saved data, delete what is in the file
// When user prints a blank input
