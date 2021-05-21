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
#include <unistd.h>
#include <string.h>

#include "dynamicArray.h"
#include "brainfuck.h"


// Detect the current os
#ifdef __unix__
	#define IS_WINDOWS 0
#else
	#define IS_WINDOWS 1
#endif

int main(int argc, char* argv[])
{
    DynamicArray file;

    FILE *sourceFile;
    FILE *compiledSource;

    // Flag to keep the temporary .c file
    int keepFile = 0;
    // How big is the memory block reserved to BrainFuck
    int memBlockLength = 32678;
    // Compiled binary filename
    char outputFilename[255] = "output";
    // Select the proper extension for the current OS
    char extension[5];
    if (IS_WINDOWS == 0)
    	strcpy(extension, ".out");
    else
    	strcpy(extension, ".exe");
    strcat(outputFilename, extension);

    // Argument parser
    int c;
    while ((c = getopt(argc, argv, "hkl:o:")) != -1)
    {
    	switch (c)
    	{
    		// -k: keep temp .c file
    		case 'k':
    			keepFile = 1;
    			break;
    		// -l (size): size of the memory block
    		case 'l':
    			memBlockLength = atoi(optarg);
    			break;
    		// -o (filename): compiled binary filename
    		case 'o':
    			strcpy(outputFilename, optarg);
    			break;
    		// -h or wrong syntax: print help
    		case 'h':
    		case '?':
    			printf("Usage: %s [-h -k -l (size) -o (filename)] filename.bf\n", argv[0]);
    			printf("\n-h\tPrints this help message\n");
    			printf("-l\tKeep the generated .c file (the file gets deleted otherwise)\n");
    			printf("-l\tSets the length of the memory block reserved to BrainFuck (default: 32768, the pointer starts at the middle)\n");
    			printf("-o\tSets the output filename (default: \"output\" without extension)\n");
    			return 1;
    	}
    }
    
    // Check if the argument number is wrong
    if (argc - optind != 1)
    {
    	printf("Too many arguments or missing source filepath. Use %s -h for help\n", argv[0]);
    	return 1;
    }

    sourceFile = fopen(argv[optind], "rb");
    if (sourceFile == NULL)
    {
    	printf("Unable to open %s. Make sure the file exists and that you have the required permissions.\n", argv[1]);
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
    if (compiledSource == NULL)
    {
    	printf("Unable to write to the current directory. Make sure you have write access to the folder.\n");
    	return 1;
    }
    fprintf(compiledSource, "#include <stdio.h>\nint main() {\nchar memory[%d] = {0};\nint pointer = %d;\n", memBlockLength, (int)memBlockLength/2);

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

    fprintf(compiledSource, "return 0;\n}");

    fclose(compiledSource);
    destroyArray(&file);

    char command[300] = "gcc temp.compiled.c -o ";
    strcat(command, outputFilename);

    // Try to open gcc and compile the translated source file
    if (system(command) != 0)
    {
        printf("\nAn error occourred. Maybe gcc isn't installed?\n");
        return 1;
    }
    else
        printf("\nCompilation was successful.\n");

    if (keepFile == 0)
    	remove("temp.compiled.c");

    return 0;
}