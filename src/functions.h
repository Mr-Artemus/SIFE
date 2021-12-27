#ifndef SIFE_FUNCTIONS_H
#define SIFE_FUNCTIONS_H

    #include <sys/stat.h>
    #include <dirent.h>
    #include <stddef.h>
    #include <string.h>
    #include <unistd.h>
    #include <stdio.h>

    extern int verboseFlag;

    int isRegularFile(const char *path);
    int isDirectory(const char *path);
    int countAllRegularFilesInDirectory(const char *path, int onlyWritable, int deep);
    void getAllRegularFilesInDirectory(const char *path, char paths[][MAX_PATH_LENGTH], size_t startAt, int onlyWritable, int deep);

#endif //SIFE_FUNCTIONS_H
