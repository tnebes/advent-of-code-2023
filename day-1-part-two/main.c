/*
  Your calculation isn't quite right. It looks like some of the digits are actually spelled out with letters: one, two, three, four, five, six, seven, eight, and nine also count as valid "digits".

   Equipped with this new information, you now need to find the real first and last digit on each line.

   What is the sum of all of the calibration values?
*/

#include <stdio.h>
#include <string.h>

#define ONE "one"
#define TWO "two"
#define THREE "three"
#define FOUR "four"
#define FIVE "five"
#define SIX "six"
#define SEVEN "seven"
#define EIGHT "eight"
#define NINE "nine"
#define MINIMUM_NUMBER_LENGTH_IN_CHARACTERS 3

int getLineLength(const char line[])
{
   int length = 0;
   while (line[length] != '\0')
   {
      length++;
   }
   return length;
}

int isDigit(const char character)
{
   return (character >= '0' && character <= '9' ? 1 : 0);
}

char getCharFromWordFromString(const char *input, const int start, const int size, const char *matchers[], const int numberOfMatchers)
{
   int currentPosition = start;

   for (int i = 0; i < numberOfMatchers; i++)
   {
      const char *currentMatcher = matchers[i];
      const int matcherLength = getLineLength(currentMatcher);
      int matcherCharacterPointer = 0;
      int matchedCharacter = 0;
      for (int j = start; j < size; j++)
      {
         char currentInputCharacter = input[j];
         char currentMatcherCharacter = currentMatcher[matcherCharacterPointer];
         if (currentInputCharacter == currentMatcherCharacter)
         {
            matchedCharacter++;
            matcherCharacterPointer++;
            if (matchedCharacter == matcherLength)
            {
               return (char)((i + 1) + '0');
            }
         }
         else
         {
            matcherCharacterPointer = 0;
            break;
         }
      }
   }

   return '\0';
}

int concatenateChars(const char input[])
{
   return (input[0] - '0') * 10 + (input[1] - '0');
}

int getIntFromLine(const char *input, const int size, const char *matchers[], const int numberOfMatchers)
{
   int selectedCharacterPosition = 0;
   int firstDigitFound = 0;
   char currentCharacter;
   char foundDigits[2];

   while (selectedCharacterPosition < size)
   {
      currentCharacter = input[selectedCharacterPosition];

      if (isDigit(currentCharacter))
      {
         if (firstDigitFound)
         {
            foundDigits[1] = currentCharacter;
         }
         else
         {
            foundDigits[0] = currentCharacter;
            foundDigits[1] = currentCharacter;
            firstDigitFound = 1;
         }
         selectedCharacterPosition++;
         continue;
      }

      char currentCharacter = getCharFromWordFromString(input, selectedCharacterPosition, size, matchers, numberOfMatchers);
      if (isDigit(currentCharacter))
      {
         if (firstDigitFound)
         {
            foundDigits[1] = currentCharacter;
         }
         else
         {
            foundDigits[0] = currentCharacter;
            foundDigits[1] = currentCharacter;
            firstDigitFound = 1;
         }
      }

      selectedCharacterPosition++;
      continue;
   }
   return concatenateChars(foundDigits);
}

int main(int argc, char *argv[])
{
   if (argc < 2)
   {
      printf("Usage: day2.exe path/to/input.txt\n");
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

   const char *digitsAsWords[] = {ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};
   const int numberOfDigits = (sizeof(digitsAsWords) / sizeof(digitsAsWords[0]));
   int sum = 0;
   char line[1024];

   while (fgets(line, sizeof(line), file))
   {
      const int lineLength = getLineLength(line);
      sum += getIntFromLine(line, lineLength, digitsAsWords, numberOfDigits);
   }

   printf("End sum is %d\n", sum);
   fclose(file);
   return 0;
}
