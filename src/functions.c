#include "functions.h"

// check if the path passed in argument points to a regular file
int isRegularFile(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

// check if the path passed in argument points to a regular file
int isDirectory(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

// Return the numbers of files from a directory and subdirectories
int countAllRegularFilesInDirectory(const char *path, int onlyWritable, int deep) {
    int count = 0;

    if(isDirectory(path)) {
        // path base
        char newPathBase[MAX_PATH_LENGTH];
        strcpy(newPathBase, path);
        if(newPathBase[strlen(newPathBase)-1] != '/') strcat(newPathBase, "/");

        struct dirent * entry;

        // open directory stream;
        DIR *dirp = opendir(path);

        // iterate on each entries
        while ((entry = readdir(dirp)) != NULL) {
            if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

            // if the entry is a regular file
            if (entry->d_type == DT_REG) {
                if(onlyWritable > 0) {
                    char newPath[MAX_PATH_LENGTH];
                    strcpy(newPath, newPathBase);
                    strcat(newPath, entry->d_name);
                    if(access(newPath, W_OK) == 0) {
                        if (onlyWritable == 1) count++;
                    } else {
                        if (onlyWritable == 2) count++;
                    }
                } else {
                    count++;
                }

            // if the entry is a directory
            } else if(entry->d_type == DT_DIR && deep == 1) {
                char newPath[MAX_PATH_LENGTH];
                strcpy(newPath, newPathBase);
                strcat(newPath, entry->d_name);
                count += countAllRegularFilesInDirectory(newPath, onlyWritable, 1);
            }
        }

        // close directory stream
        if(closedir(dirp) != 0 && verboseFlag) printf("Cannot close the directory stream for %s\n", path);

    } else if (isRegularFile(path)) {
        count = 1;
    }

    return count;
}

void getAllRegularFilesInDirectory(const char *path, char paths[][MAX_PATH_LENGTH], size_t startAt, int onlyWritable, int deep) {
    size_t iterator = startAt;

    if(isDirectory(path)) {
        // path base
        char newPathBase[MAX_PATH_LENGTH];
        strcpy(newPathBase, path);
        if(newPathBase[strlen(newPathBase)-1] != '/') strcat(newPathBase, "/");


        struct dirent * entry;

        // open directory stream;
        DIR *dirp = opendir(path);

        // iterate on each entries
        while ((entry = readdir(dirp)) != NULL) {
            if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

            // if the entry is a regular file
            if (entry->d_type == DT_REG) {
                if(onlyWritable > 0) {
                    char newPath[MAX_PATH_LENGTH];
                    strcpy(newPath, newPathBase);
                    strcat(newPath, entry->d_name);
                    if(access(newPath, W_OK) == 0) {
                        if (onlyWritable == 1) strcpy(paths[iterator], newPath), iterator++;
                    } else {
                        if (onlyWritable == 2) strcpy(paths[iterator], newPath), iterator++;
                    }
                } else {
                    strcpy(paths[iterator], newPathBase);
                    strcat(paths[iterator], entry->d_name);
                    iterator++;
                }

            // if the entry is a directory
            } else if(entry->d_type == DT_DIR && deep == 1) {
                char dirPath[MAX_PATH_LENGTH];
                strcpy(dirPath, newPathBase);
                strcat(dirPath, entry->d_name);

                getAllRegularFilesInDirectory(dirPath, paths, iterator, onlyWritable, 1);
                iterator += countAllRegularFilesInDirectory(dirPath, onlyWritable, 1);
            }
        }

        // close directory stream
        if(closedir(dirp) != 0 && verboseFlag) printf("Cannot close the directory stream for %s\n", path);

    } else if (isRegularFile(path)) {
        strcpy(paths[iterator], path);
    }
}