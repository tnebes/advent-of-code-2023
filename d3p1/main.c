#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
   }

   printf("Sum is: %d", sum);
   return 0;
}
