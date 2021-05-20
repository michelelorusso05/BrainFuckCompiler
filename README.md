# BrainFuckCompiler
A compiler written in C for the esoteric programming language Brainfuck.
## Usage
### You'll need to install gcc if you don't have it.
Run the compiler from the command line telling it what file do you want to compile, like as you would do with gcc.
#### Example
bfc.exe main.bf
## How does it work
The compiler translates the brainfuck source into a .c file and compilates that.
## Current limitations
The memory, is limited to 32KB, with the pointer starting at the middle.
(I mean, 32KB is enough for a program written in this language, right?)
