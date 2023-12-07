#include <stdio.h>
#include <string.h>

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

typedef struct
{
   int red;
   int blue;
   int green;
} Turn;

typedef struct
{
   int id;
   Turn turns[];
} Game;

enum colour
{
   Red,
   Green,
   Blue
};

int isDigit(const char character)
{
   return (character >= '0' && character <= '9');
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

void updateTurnColor(Turn *turn, const char color, const int numberOfThrows)
{
   switch (color)
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
      printf("Unrecognized color %c", color);
      break;
   }
}

int getIdFromPossibleGame(const char input[], const int inputLength, const char *colours[], const int coloursSize)
{
   if (inputLength < 2)
   {
      return 0;
   }

   Game game;
   Turn turns[MAX_TURNS_IN_GAME];
   int currentCharacterPosition = 0;
   int currentTurn = 0;
   char currentCharacter = input[currentCharacterPosition];

   game.id = getFirstIntFromInput(input, 0, inputLength);
   int startPosition, currentPosition = getPositionOfCharacter(input, 0, inputLength, ':');
   if (startPosition < 0)
   {
      printf("Input was invalid. %s", input);
      return 0;
   }

   // while (currentCharacter != '\0')
   // {
   //    currentCharacter = input[currentPosition++];
   //    if (!isDigit(currentCharacter))
   //    {
   //       continue;
   //    }

   //    int numberOfThrows = getFirstIntFromInput(input, currentPosition, inputLength);
   //    int lengthOfIntInChars = 0; // set to 0 due to above currentPosition++

   //    {
   //       int temp = numberOfThrows;
   //       while (temp >= 10)
   //       {
   //          temp /= 10;
   //          lengthOfIntInChars++;
   //       }
   //    }

   //    currentPosition += lengthOfIntInChars + 1; // skipping to first letter of colour
   //    currentCharacter = input[currentPosition];

   //    Turn turn;

   //    while (!isEnd(currentCharacter))
   //    {
   //       updateTurnColor(&turn, currentCharacter, numberOfThrows);
   //    }
      

   // }

   

   // i need to capture the number of thrown cubes
   // i need to find which colour it is
   // until I reach ;
   // i need to store that turn into the game
   // until I reach '\0'

   return game.id;
}

int main(int argc, char *argv[])
{

   if (argc < 2)
   {
      printf("Usage: day2part1.exe path/to/input.txt\n");
      return 1;
   }

   const char *colours[] = {RED, GREEN, BLUE};
   const int coloursSize = 3;

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
      sum += getIdFromPossibleGame(line, lineLength, &colours, coloursSize);
   }

   printf("Sum is: %d", sum);
   return 0;
}
