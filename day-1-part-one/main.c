/*
  On each line, the calibration value can be found by combining the first digit and the last digit (in that order) to form a single two-digit number.
*/

#include <stdio.h>
#include <string.h>

int getCharacterAsInt(char character)
{
   return (character >= '0' && character <= '9' ? 1 : 0);
}

int getLineLength(const char line[])
{
   int length = 0;
   while (line[length] != '\0')
   {
      length++;
   }
   return length;
}

int main(int argc, char *argv[])
{
   if (argc < 2)
   {
      printf("Usage: day1.exe path/to/input.txt\n");
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
      char lineConcatenation[2];
      int endPosition = getLineLength(line) - 1;

      int startPosition = 0;

      while (startPosition < endPosition)
      {
         const char currentCharacter = line[startPosition];
         if (getCharacterAsInt(currentCharacter))
         {
            lineConcatenation[0] = currentCharacter;
            break;
         }
         startPosition++;
      }

      while (endPosition >= startPosition)
      {
         const char currentCharacter = line[endPosition];
         if (getCharacterAsInt(currentCharacter))
         {
            lineConcatenation[1] = currentCharacter;
            break;
         }
         endPosition--;
      }

      int lineSum = (lineConcatenation[0] - '0') * 10 + (lineConcatenation[1] - '0');
      sum += lineSum;
   }

   printf("End sum is %d\n", sum);
   fclose(file);
   return 0;
}
