Points of interest:

(1)
dirent.h that is used is windows compatible. For some reason, the dirent.h that I tried to include pre-installed in the GCC was not working as I intended to be so I had to get it here: https://github.com/tronkko/dirent/tree/master (Please do not attempt to use #include <dirent.h> because that calls the GCC dirent.h, not the one that I got from "tronkko".)

(2)
File system has been fixed. Categorized each files as source code, headers, or input/output (bin)

(3)
The compiled .exe should be in the bin but I couldn't figure out how to automatically compile a file into /lexeme/bin so for the meantime, if you wish to compile cleanly, I recommend doing:

If you're in the root:
cd PREVIOUS_DIRECTORIES/Cmin/lexical/source && gcc lexer.c -o "../bin/lexeme.exe" && cd.. && cd bin && lexeme.exe

If you're inside Cmin:
cd lexical/source ; gcc lexer.c -o "../bin/lexeme.exe" ; cd.. ; cd bin ; .\lexeme.exe

(If you just compile it on vscode, the file will spawn in lexical/source directory not in the bin.)

(The previous directories are of course, the directories where you likely put where your files are)

(4)
The tokenized file is also in /lexical/bin. The file extension shall be .lxmf.

(5)
Added scan.c in lexical/source for scanning the first file name it sees that has an extension of .min. Additionally, the path where the .cmin file could be read can be edited in set.h in /lexical/headers in variable workPath

(6)
In set.h, there are a few changes for some variable names:

CONST -> CONST_
INT -> INT_
TRUE -> TRUE_
FALSE -> FALSE_

This is to prevent conflict with dirent.h's macros.



