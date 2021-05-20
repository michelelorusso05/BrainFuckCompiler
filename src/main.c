/*
    main.c
    Lorusso Michele - 2021
    This is the main file. It contains the majority of the code of the compiler.

    The compiler opens a .bf file, ignores all the characters that aren't one of the 
    eight commands and translates each remaining character into their C counterparts.
    Then, the resulting .c file is compiled resulting in an executable file.
*/

#include <stdio.h>
#include <stdlib.h>

#include "dynamicArray.h"
#include "brainfuck.h"

int main(int argc, char* argv[])
{
    DynamicArray file;

    FILE *sourceFile;
    FILE *compiledSource;
    
    
    if (argc == 2)
    {
        sourceFile = fopen(argv[1], "rb");
        // Unable to open the file
        if (sourceFile == NULL)
        {
            printf("Unable to open %s. Make sure the file exists and that you have the required permissions.\n", argv[1]);
            return 1;
        }
    }
    // Too many or none arguments
    else
    {
        printf("USAGE: %s filename.bf\n", argv[0]);
        return 1;
    }

    initArray(&file, 2048);
    char currentChar;
    int openLoops = 0;

    // Load all the characters into a dynamic array
    while (!feof(sourceFile))
    {
        fscanf(sourceFile, "%c", &currentChar);
        insertElement(&file, currentChar);
    }

    // Set the last character as null (null-terminator)
    file.array[file.used - 1] = '\0';
    fclose(sourceFile);

    // Open a new .c file
    compiledSource = fopen("temp.compiled.c", "w");
    fprintf(compiledSource, "%s", beginning);

    // For each character, write its C counterpart to the .c file
    for (int i = 0; i < file.used; i++)
    {
        switch (file.array[i])
        {
            case '+':
                fprintf(compiledSource, "%s", add);
                break;
            case '-':
                fprintf(compiledSource, "%s", subtract);
                break;
            case '<':
                fprintf(compiledSource, "%s", pointerLeft);
                break;
            case '>':
                fprintf(compiledSource, "%s", pointerRight);
                break;
            case '[':
                fprintf(compiledSource, "%s", beginLoop);
                openLoops++;
                break;
            case ']':
                fprintf(compiledSource, "%s", endLoop);
                openLoops--;
                break;
            case '.':
                fprintf(compiledSource, "%s", printChar);
                break;
            case ',':
                fprintf(compiledSource, "%s", getChar);
                break;
        }
    }

    // The only possible syntax error in this language is an unmatched [ or ], so only this check is needed
    if (openLoops != 0)
    {
        printf("\nSYNTAX ERROR: Number of opened loops doesn't match with the number of closed loops. Maybe you missed a square bracket?\n");
        fclose(compiledSource);
        destroyArray(&file);
        return 1;
    }

    fprintf(compiledSource, "%s", ending);

    fclose(compiledSource);
    destroyArray(&file);

    // Try to open gcc and compile the translated source file
    if (system("gcc temp.compiled.c -o output") != 0)
    {
        printf("\nAn error occourred. Maybe gcc isn't installed?\n");
        return 1;
    }
    else
        printf("\nCompilation was successful.\n");

    return 0;
}