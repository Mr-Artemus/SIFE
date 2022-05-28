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
// `onlyWritable` : 0 => all files
//                  1 => only writable files
//                  2 => only non-writable files
int countAllRegularFilesInDirectory(const char *path, int onlyWritable, int deep) {
    int count = 0;

    // If the path is a directory
    if(isDirectory(path)) {
        // Path base
        char newPathBase[MAX_PATH_LENGTH];
        strcpy(newPathBase, path);
        // Add a '/' at the end of the path if not present
        if(newPathBase[strlen(newPathBase)-1] != '/') strcat(newPathBase, "/");

        struct dirent * entry;

        // Open directory stream;
        DIR *dirp = opendir(path);

        // Iterate on each entries
        while ((entry = readdir(dirp)) != NULL) {
            // Pass the iteration if it's the current or parent directory
            if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

            // If the entry is a regular file
            if (entry->d_type == DT_REG) {
                if(onlyWritable > 0) {
                    char newPath[MAX_PATH_LENGTH];
                    strcpy(newPath, newPathBase);
                    strcat(newPath, entry->d_name);
                    if(access(newPath, W_OK) == 0) {
                        // Increment if the file is writable and the `onlyWritable` is set to 1 (only writable files)
                        if (onlyWritable == 1) count++;
                    } else {
                        // Increment if the file is writable and the `onlyWritable` is set to 2 (only non-writable files)
                        if (onlyWritable == 2) count++;
                    }
                } else {
                    // Increment the number of files
                    count++;
                }

            // If the entry is a directory
            } else if(entry->d_type == DT_DIR && deep == 1) {
                char newPath[MAX_PATH_LENGTH];
                strcpy(newPath, newPathBase);
                strcat(newPath, entry->d_name);
                // Call the same function with the folder as parameter
                count += countAllRegularFilesInDirectory(newPath, onlyWritable, 1);
            }
        }

        // Close directory stream
        if(closedir(dirp) != 0 && verboseFlag) printf("Cannot close the directory stream for %s\n", path);

    } else if (isRegularFile(path)) {
        // If the path is a simple file, return 1
        count = 1;
    }

    return count;
}

// put all path to a passed array
// `onlyWritable` : 0 => all files
//                  1 => only writable files
//                  2 => only non-writable files
void getAllRegularFilesInDirectory(const char *path, char paths[][MAX_PATH_LENGTH], size_t startAt, int onlyWritable, int deep) {
    // Iterator to start the output array at a specific location
    size_t iterator = startAt;

    // If the path is a directory
    if(isDirectory(path)) {
        // Path base
        char newPathBase[MAX_PATH_LENGTH];
        strcpy(newPathBase, path);
        // Add a '/' at the end of the path if not present
        if(newPathBase[strlen(newPathBase)-1] != '/') strcat(newPathBase, "/");


        struct dirent * entry;

        // Open directory stream;
        DIR *dirp = opendir(path);

        // Iterate on each entries
        while ((entry = readdir(dirp)) != NULL) {
            // Pass the iteration if it's the current or parent directory
            if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

            // If the entry is a regular file
            if (entry->d_type == DT_REG) {
                if(onlyWritable > 0) {
                    char newPath[MAX_PATH_LENGTH];
                    strcpy(newPath, newPathBase);
                    strcat(newPath, entry->d_name);
                    if(access(newPath, W_OK) == 0) {
                        // Append if the file is writable and the `onlyWritable` is set to 1 (only writable files)
                        if (onlyWritable == 1) strcpy(paths[iterator], newPath), iterator++;
                    } else {
                        // Append if the file is writable and the `onlyWritable` is set to 2 (only non-writable files)
                        if (onlyWritable == 2) strcpy(paths[iterator], newPath), iterator++;
                    }
                } else {
                    // Append the file
                    strcpy(paths[iterator], newPathBase);
                    strcat(paths[iterator], entry->d_name);
                    iterator++;
                }

            // If the entry is a directory
            } else if(entry->d_type == DT_DIR && deep == 1) {
                char dirPath[MAX_PATH_LENGTH];
                strcpy(dirPath, newPathBase);
                strcat(dirPath, entry->d_name);

                // Call the same function with the folder as parameter
                getAllRegularFilesInDirectory(dirPath, paths, iterator, onlyWritable, 1);
                iterator += countAllRegularFilesInDirectory(dirPath, onlyWritable, 1);
            }
        }

        // Close directory stream
        if(closedir(dirp) != 0 && verboseFlag) printf("Cannot close the directory stream for %s\n", path);

    } else if (isRegularFile(path)) {
        // If the path is a simple file
        strcpy(paths[iterator], path);
    }
}