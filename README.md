# Cmin Lexical Analyzer

Cmin is a minimalistic version of the C language designed to modernize the C programming experience. It was conceptualized to simplify the steep learning curve of C by eliminating manual pointer manipulation for referencing, automating memory processes, and introducing modern features like string interpolation and a dedicated `string` data type.

This repository contains the Cmin Lexical Analyzer, which reads Cmin source code and tokenizes it into a symbol table for further stages of compilation.

## Features

* **File Support:** Exclusively processes files with the `.cmin` extension.
* **Identifier Recognition:** Supports strict, case-sensitive identifiers following the standard `[a-zA-Z_][a-zA-Z0-9_]*` regular expression.


* **Keywords and Reserved Words:** Recognizes Cmin-specific keywords (e.g., `if`, `else`, `while`, `try`, `catch`, `anon`) and reserved data types/modifiers (e.g., `int`, `string`, `bool`, `struct`, `global`, `typedef`).


* **Operators:** Tokenizes an extensive set of arithmetic, assignment, logical, bitwise, and comparison operators.


* **Flexible Formatting:** Adopts a free-field format. Whitespaces (spaces, tabs, newlines) are ignored after separating tokens.


* **Optional Semicolons:** Semicolons are not required for statement termination and are safely ignored by the lexer.


* **Comments:** Supports both standard C-style single-line (`//`) and multi-line block (`/* ... */`) comments.



## Requirements

To compile and run this lexical analyzer, you need a standard C compiler installed on your system, such as:

* GCC (GNU Compiler Collection)
* Clang

## Installation and Usage

**1. Compile the Lexical Analyzer**
Open your terminal and compile the C source file using your preferred compiler:

```bash
gcc cmin_lexer.c -o cmin_lexer

```

**2. Create a Cmin Source File**
Create a sample file named `program.cmin` with valid Cmin code:

```c
/* Sample Cmin Program */
(sum + 47) / total

```

**3. Run the Analyzer**
Execute the compiled program and pass your `.cmin` file as an argument:

```bash
./cmin_lexer program.cmin

```

**4. View the Output**
The analyzer will process the input and automatically generate an output file named `program.txt` in the same directory. This text file will contain the generated Symbol Table with the parsed tokens and their corresponding lexemes.

## Project Authors

This proposal was authored by Group 2 from the Polytechnic University of the Philippines: Rheinz Owen D. Alpon, Jermaine Angelo G. Falcutila, Julian D. Jose, Charles Samuell M. Lopez, Kian Phillip T. Marin, Mariel M. Oliveros, and Kurt Recel N. Romares.