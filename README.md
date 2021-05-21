# BrainFuckCompiler
A compiler written in C for the esoteric programming language Brainfuck.
## Usage
### You'll need to install gcc if you don't have it.
Run the compiler from the command line telling it what file do you want to compile, like as you would do with gcc.
#### Compiler flags (new in 1.1)
-l (size): specifies the memory block length (defaults to 32678)
-o (filename): specifies the name of the final binary (defaults to output.exe or output.out, depends on your OS)
-k: keep the generated .c file (defaults to no)
#### Example
bfc.exe [flags] main.bf
## How does it work
The compiler translates the brainfuck source into a .c file and compilates that.
