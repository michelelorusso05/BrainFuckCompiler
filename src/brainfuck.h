/*
    brainfuck.h
    Lorusso Michele - 2021
    This header file contains the translated instructions that will be written to the generated .c file by the compiler.
*/

// The eight character accepted in the BrainFuck language; anything else is considered as comment
const char validCharacters[] = "+-<>[].,";
    
// Translated C file syntax
const char add[] = "memory[pointer]++;\n";
const char subtract[] = "memory[pointer]--;\n";
const char pointerLeft[] = "pointer--;\n";
const char pointerRight[] = "pointer++;\n";
const char beginLoop[] = "while (memory[pointer] != 0) {\n";
const char endLoop[] = "}\n";
const char printChar[] = "printf(\"%c\", memory[pointer]);\n";
const char getChar[] = "scanf(\"%c\", &memory[pointer]);\n";