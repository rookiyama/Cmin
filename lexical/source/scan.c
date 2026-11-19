#ifndef _SET
#define _SET
#include "../header/set.h"
#endif

#include <string.h>
#include "../header/dirent.h"

static int parseExt(const struct dirent* dir){
    if(!dir) return 0;
    if(dir->d_type == DT_REG) {
        const char *ext = strrchr(dir->d_name,'.');
        if((!ext) || (ext == dir->d_name)) {
            return 0;
        } else {
            if(ext[0] == '.' &&
               ext[1] == 'c' &&
               ext[2] == 'm' &&
               ext[3] == 'i' &&
               ext[4] == 'n')
                return 1;
            else {
                debugVal = 0;
                return 0;
            }
        }
    }
    return 0;
}

void fileSearch(){
    struct dirent **fileToBeAnalyzed;
    int fileCount = scandir("../../programs", &fileToBeAnalyzed, parseExt, alphasort);
    if (fileCount < 0)
        *fileName = *"\0";
    else {
        short length = strlen(fileToBeAnalyzed[0]->d_name);
        sprintf(fileName, "%s%s", workPath, fileToBeAnalyzed[0]->d_name);
        free(fileToBeAnalyzed[0]);
        free(fileToBeAnalyzed);
    }
}