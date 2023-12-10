#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINES 1024

enum CharacterType
{
   EMPTY_SPACE,
   DIGIT,
   SYMBOL
};

typedef struct
{
   int length;
   int *numbers;
   int *symbols;
} Line;

int isDigit(const char character)
{
   return (character >= '0' && character <= '9');
}

int isEmptySpace(const char character)
{
   return character == '.';
}

int isSymbol(const char character)
{
   return (!isEmptySpace(character) && !isDigit(character));
}

int isEnd(const char character)
{
   return (character == '\0' || character == '\n');
}

int classifyCharacter(const char character)
{
   if (isEmptySpace(character))
   {
      return EMPTY_SPACE;
   }
   if (isDigit(character))
   {
      return DIGIT;
   }
   if (isSymbol(character))
   {
      return SYMBOL;
   }
   printf("Character %c cannot be identified.\n", character);
   exit(1);
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

Line *initialiseLine(const int lineLength)
{
   Line *line = malloc(sizeof(Line));

   if (line == NULL)
   {
      printf("Failed to allocate memory for Line\n");
      exit(1);
   }

   line->length = lineLength;
   line->numbers = malloc(sizeof(int) * lineLength);
   line->symbols = malloc(sizeof(int) * lineLength);

   for (int i = 0; i < lineLength; i++)
   {
      line->numbers[i] = 0;
      line->symbols[i] = 0;
   }

   return line;
}

void freeLine(Line *line)
{
   free(line->numbers);
   free(line->symbols);
   free(line);
}

void freeLines(Line *lines[], const int numberOfLines)
{
   for (int i = 0; i < numberOfLines; i++)
   {
      freeLine(lines[i]);
   }
}

int getLengthOfNumber(int number)
{
   int counter = 1;
   while (number >= 10)
   {
      number /= 10;
      counter++;
   }
   return counter;
}

void populateLine(Line *line, const char *input)
{
   const int inputLength = line->length;
   int currentCharacterPosition = 0;
   char currentCharacter = input[currentCharacterPosition];
   while (!isEnd(currentCharacter))
   {
      switch (classifyCharacter(currentCharacter))
      {
         int number;
         int numberLength;
         int targetPosition;
      case EMPTY_SPACE:
         currentCharacter = input[++currentCharacterPosition];
         break;
      case DIGIT:
         number = getFirstIntFromInput(input, currentCharacterPosition, inputLength);
         numberLength = getLengthOfNumber(number);
         targetPosition = currentCharacterPosition + numberLength;
         while (currentCharacterPosition < targetPosition)
         {
            line->numbers[currentCharacterPosition++] = number;
         }
         currentCharacter = input[currentCharacterPosition];
         break;
      case SYMBOL:
         line->symbols[currentCharacterPosition++] = 1;
         currentCharacter = input[currentCharacterPosition];
         break;
      default:
         printf("Could not classify character %c\n", currentCharacter);
         exit(1);
      }
   }
}

int hasSymbolHorizontally(const int position, const Line *currentLine)
{
   if (currentLine->symbols[position])
   {
      return 1;
   }
   if (position > 0 && currentLine->symbols[position - 1])
   {
      return 1;
   }
   if (position < currentLine->length - 1 && currentLine->symbols[position + 1])
   {
      return 1;
   }
   return 0;
}

int hasSymbolVertically(const int position, Line *aboveLine, Line *currentLine, Line *belowLine)
{
   if (currentLine->symbols[position])
   {
      return 1;
   }
   if (aboveLine != NULL && aboveLine->symbols[position])
   {
      return 1;
   }
   if (belowLine != NULL && belowLine->symbols[position])
   {
      return 1;
   }
   return 0;
}

int hasSymbolAround(const int position, Line *aboveLine, Line *currentLine, Line *belowLine)
{
   // up horizontally
   if (aboveLine != NULL && hasSymbolHorizontally(position, aboveLine))
   {
      return 1;
   }
   // down horizontally
   if (belowLine != NULL && hasSymbolHorizontally(position, belowLine))
   {
      return 1;
   }
   // left vertically
   if (position > 0 && hasSymbolVertically(position - 1, aboveLine, currentLine, belowLine))
   {
      return 1;
   }
   // right vertically
   if (position < currentLine->length - 1 && hasSymbolVertically(position + 1, aboveLine, currentLine, belowLine))
   {
      return 1;
   }
   return 0;
}

int getSumFromLines(Line *lines[], const int numberOfLines)
{
   int sum = 0;
   return sum;
}

Line *parseLine(const char *input, const int inputLength)
{
   Line *line = initialiseLine(inputLength);
   populateLine(line, input);
   return line;
}

int main(int argc, char *argv[])
{

   if (argc < 2)
   {
      printf("Usage: d3p2.exe path/to/input.txt\n");
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

   char line[1024];
   Line *lines[MAX_LINES] = {NULL};
   int counter = 0;

   while (fgets(line, sizeof(line), file))
   {
      const int lineLength = strlen(line);
      lines[counter++] = parseLine(line, lineLength);
   }

   int sum = getSumFromLines(lines, counter);

   printf("Sum is: %d", sum);
   freeLines(lines, counter);
   return 0;
}
