#include "..\header\set.h"

void symbolTableSearch(){
    struct dirent **fileToBeAnalyzed;
    int fileCount = scandir("../../lexical/bin", &fileToBeAnalyzed, NULL, alphasort);
    int tokenFileExists = 0;
    for (int i = 0; i < fileCount; i++) {
        if (!strcmp(fileToBeAnalyzed[i]->d_name, "symbol_table.txt")) {
            if (!tokenFileExists) {
                sprintf(fileName, "%s%s", symtblPath, fileToBeAnalyzed[i]->d_name);
                tokenFileExists = 1;
                continue;
            } else {
                debugVal = 0;
                break;
            }   
        }
    }
}


/*
identifier = 123;
if(marcos)

return;
break;

*/
    
