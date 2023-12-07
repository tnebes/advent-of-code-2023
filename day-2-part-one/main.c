#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define RED "red"
#define GREEN "green"
#define BLUE "blue"

#define RED_LENGTH 3
#define GREEN_LENGTH 5
#define BLUE_LENGTH 4

#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14

#define MAX_TURNS_IN_GAME 16
#define MAX_COLOURS_IN_TURN 3

typedef struct
{
   int red;
   int blue;
   int green;
} Turn;

typedef struct
{
   int id;
   Turn *turns;
} Game;

int isDigit(const char character)
{
   return (character >= '0' && character <= '9');
}

int isJunk(const char character)
{
   return (character == ' ' || character == ',');
}

int isEnd(const char character)
{
   return (character == ';' || character == '\0');
}

int getFirstIntFromInput(const char input[], const int start, const int inputLength)
{
   char foundDigits[4];
   int digitCounter = 0;
   int firstDigitFound = 0;
   for (int i = start; i < inputLength && digitCounter < 4; i++)
   {
      char currentCharacter = input[i];
      if (isDigit(currentCharacter))
      {
         firstDigitFound = 1;
         foundDigits[digitCounter++] = currentCharacter;
         continue;
      }
      if (firstDigitFound)
      {
         if (!isDigit(currentCharacter))
         {
            break;
         }
      }
   }

   int number = 0;
   for (int i = digitCounter - 1, multiplicant = 1; i >= 0; i--, multiplicant *= 10)
   {
      number += (foundDigits[i] - '0') * multiplicant;
   }
   return number;
}

int getPositionOfCharacter(const char input[], const int start, const int end, const char targetChar)
{
   for (int i = start; i < end; i++)
   {
      if (input[i] == targetChar)
      {
         return i;
      }
   }

   return -1;
}

int skipCharactersForColour(const char colour)
{
   switch (colour)
   {
   case 'r':
      return RED_LENGTH;
      break;
   case 'g':
      return GREEN_LENGTH;
      break;
   case 'b':
      return BLUE_LENGTH;
      break;
   default:
      printf("Unrecognized color %c", colour);
      exit(1);
   }
}

void updateTurnColour(Turn *turn, const char colour, const int numberOfThrows)
{
   switch (colour)
   {
   case 'r':
      turn->red = numberOfThrows;
      break;
   case 'g':
      turn->green = numberOfThrows;
      break;
   case 'b':
      turn->blue = numberOfThrows;
      break;
   default:
      printf("Unrecognized color %c", colour);
      break;
   }
}

int getTurnLength(const char *input, const int start)
{
   int currentCharacterPosition = start;
   int length = 1;
   char currentCharacter = input[currentCharacterPosition];

   while (currentCharacter != '\0')
   {
      length++;
      currentCharacter = input[++currentCharacterPosition];
   }

   return length;
}

char *getTurnInformation(const char *input, const int start)
{
   int turnSize = getTurnLength(input, start);

   char *turnsInformation = (char *)malloc((turnSize + 1));

   if (turnsInformation == NULL)
   {
      printf("Could not allocate memory for %s\n", input);
      exit(1);
   }

   for (int i = start, j = 0; j < turnSize; i++, j++)
   {
      turnsInformation[j] = input[i];
   }
   turnsInformation[turnSize] = '\0';

   return turnsInformation;
}

void parseTurns(Game *game, char *turnsInformation, int turnsLength)
{
   int parsedTurns = 0;
   int currentCharacterPosition = 0;
   char currentCharacter = turnsInformation[currentCharacterPosition];
   int numberOfThrows = 0;

   while (currentCharacter != '\0')
   {
      if (isJunk(currentCharacter))
      {
         currentCharacter = turnsInformation[++currentCharacterPosition];
         continue;
      }

      int foundColours = 0;

      while (1)
      {
         if (currentCharacterPosition >= turnsLength || foundColours >= MAX_COLOURS_IN_TURN || currentCharacter == '\0')
         {
            return;
         }
         if (isDigit(currentCharacter))
         {
            numberOfThrows = getFirstIntFromInput(turnsInformation, currentCharacterPosition, turnsLength);
            while (isDigit(currentCharacter))
            {
               currentCharacter = turnsInformation[++currentCharacterPosition];
            }
         }
         else
         {
            currentCharacter = turnsInformation[++currentCharacterPosition];
            continue;
         }

         while (isJunk(currentCharacter))
         {
            currentCharacter = turnsInformation[++currentCharacterPosition];
         }

         updateTurnColour(&game->turns[parsedTurns], currentCharacter, numberOfThrows);
         numberOfThrows = 0;
         foundColours++;
         currentCharacterPosition += skipCharactersForColour(currentCharacter);
         currentCharacter = turnsInformation[currentCharacterPosition];

         if (isEnd(currentCharacter))
         {
            parsedTurns++;
            foundColours = 0;
         }
         continue;
      }
   }
}

Game *initialiseGame()
{
   Game *game = malloc(sizeof(Game));

   if (game == NULL)
   {
      printf("Failed to allocate memory for Game\n");
      exit(1);
   }

   game->id = 0;
   game->turns = (Turn *)malloc(MAX_TURNS_IN_GAME * sizeof(Turn));
   for (int i = 0; i < MAX_TURNS_IN_GAME; i++)
   {
      Turn *turn = &(game->turns[i]);
      turn->red = 0;
      turn->blue = 0;
      turn->green = 0;
   }
   return game;
}

void deinitialiseGame(Game *game)
{
   free(game->turns);
   free(game);
}

int validateGame(Game *game)
{
   Turn *turns = game->turns;
   for (int i = 0; i < MAX_TURNS_IN_GAME; i++)
   {
      if (turns[i].red > MAX_RED || turns[i].blue > MAX_BLUE || turns[i].green > MAX_GREEN)
      {
         return 0;
      }
   }
   return 1;
}

int getIdFromPossibleGame(const char input[], const int inputLength)
{
   if (inputLength < 2)
   {
      return 0;
   }

   Game *game = initialiseGame();

   if (game->turns == NULL)
   {
      printf("Could not allocate memory for game.turns\n");
      exit(1);
   }

   game->id = getFirstIntFromInput(input, 0, inputLength);
   int currentPosition = getPositionOfCharacter(input, 0, inputLength, ':') + 1;
   if (currentPosition < 0)
   {
      printf("Input was invalid. %s", input);
      return 0;
   }

   char *turnsInformation = getTurnInformation(input, currentPosition);
   if (turnsInformation == NULL)
   {
      printf("Could not allocate memory for %s\n", input);
      exit(1);
   }

   parseTurns(game, turnsInformation, strlen(turnsInformation));
   free(turnsInformation);

   if (validateGame(game))
   {
      int gameId = game->id;
      deinitialiseGame(game);
      return gameId;
   }
   return 0;
}

int main(int argc, char *argv[])
{

   if (argc < 2)
   {
      printf("Usage: day2part1.exe path/to/input.txt\n");
      return 1;
   }

   char filePath[256];
   strncpy(filePath, argv[1], sizeof(filePath));
   filePath[sizeof(filePath) - 1] = '\0';

   FILE *file = fopen(filePath, "r");
   if (file == NULL)
   {
      printf("Failed to open file at %s\n", filePath);
      return 1;
   }

   int sum = 0;
   char line[1024];

   while (fgets(line, sizeof(line), file))
   {
      const int lineLength = strlen(line);
      sum += getIdFromPossibleGame(line, lineLength);
   }

   printf("Sum is: %d", sum);
   return 0;
}
