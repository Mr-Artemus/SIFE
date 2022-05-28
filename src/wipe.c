#include "wipe.h"

// write methods
unsigned char _write0       (void) { return 0b00000000; }
unsigned char _write1       (void) { return 0b11111111; }
unsigned char _write01      (void) { return 0b01010101; }
unsigned char _writeRandom  (void) { return rand();     } // NOLINT(cert-msc50-cpp)

// check methods
int _checkNone  (unsigned char byte) { return 1;                  }
int _check0     (unsigned char byte) { return byte == 0b00000000; }
int _check1     (unsigned char byte) { return byte == 0b11111111; }
int _check01    (unsigned char byte) { return byte == 0b01010101; }

// wipe method
void wipeFiles(char paths[][MAX_PATH_LENGTH], size_t pathsLength, algorithm algorithm) {
    printf("Starting the wiping sequence of %zu file%s with method : %s\n", pathsLength, pathsLength > 1 ? "s" : "", algorithm.name);
    // for each path
    for (size_t i = 0; i < pathsLength; i++) {
        // check if is writable again by security
        if(access(paths[i], W_OK) == 0) {
            if(verboseFlag) printf(CLEAR_LINE "Opening file \"%s\"\n", paths[i]);
            // Open file stream (r+b ==> read + write at the top of the file in binary)
            FILE *fp = fopen(paths[i], "r+b");
            if(fp == NULL) {
                // If an error as occurred while opening the file
                if(verboseFlag) printf(CLEAR_LINE COLOR_RED "Cannot open file \"%s\"" COLOR_RESET, paths[i]);
            } else {
                // get file size
                fseek(fp, 0, SEEK_END);
                unsigned long int fileSize = ftell(fp);
                unsigned long int fileSizeDouble = fileSize * 2; // The double will be in use for the verification sequence

                if(verboseFlag) printf(CLEAR_LINE "File size: %zu bytes\n", fileSize);

                // variables
                int passCounter;
                unsigned long int offsetCounter;
                int lastPercent = 0;
                int percent = 0;
                int errorOnWriteMethod = 0;
                unsigned char byte;

                // for each pass sequence
                for(passCounter = 0; passCounter < algorithm.size; passCounter++) {
                    if(verboseFlag) printf(CLEAR_LINE "Starting pass %d of %d\n", passCounter + 1, algorithm.size);

                    // return on the top of the file
                    fseek(fp, 0, SEEK_SET);

                    // for each byte of the file
                    for(offsetCounter = 1; offsetCounter <= fileSizeDouble; offsetCounter++) {
                        // Calculate the percentage of advancement and convert it to int instead of float
                        percent = (int) roundf(((((float) offsetCounter / (float) fileSizeDouble) * ((float) passCounter + 1))/(float) algorithm.size) * 100.0f);

                        // If the percentage is higher that the previous one
                        if(percent > lastPercent) {
                            lastPercent = percent;
                            printf(CLEAR_LINE "[file %zu/%zu] [%3d%%] Wiping file: Pass %d of %d", i + 1, pathsLength, percent, passCounter + 1, algorithm.size);
                            fflush(stdout);
                        }


                        if(offsetCounter <= fileSize) { // If it's under the half of the iterator (writing part)
                            // write a byte to the file and move cursor to the next one
                            fprintf(fp, "%c", algorithm.writes[passCounter]());

                            // if write is finished, then return to the top of the file for the verification
                            if(offsetCounter == fileSize) {
                                // return on the top of the file
                                fseek(fp, 0, SEEK_SET);

                                if(verboseFlag) printf(CLEAR_LINE "End of writing for this pass, now place for verification\n");
                            }
                        } else { // If it's after the half of the iterator (verification part)
                            fread(&byte, 1, 1, fp);
                            if(algorithm.checks[passCounter](byte) != 1 && errorOnWriteMethod == 0) {
                                errorOnWriteMethod = 1;
                                printf(CLEAR_LINE "An error has occurred while wiping the file \"%s\"\nOne or more bytes were not rewritten as they should have been.\nPlease verify that the file is editable and that it is not being used by another process.\n", paths[i]);
                                break;
                            }
                        }
                    }
                }

                printf(CLEAR_LINE "[file %zu/%zu] [100%%] Wiping file: Pass %d of %d\n", i + 1, pathsLength, passCounter, algorithm.size);
                fflush(stdout);

                if(verboseFlag) printf("End for this file\n");

                // Close the file stream
                if(fclose(fp) == 0) {
                    if(verboseFlag) printf("Successfully close the stream\n");
                } else {
                    if(verboseFlag) printf("Cannot close the stream\n");
                }

                // If the `--keep` parameter as not been passed
                if(keepFilesAfterOverwriting == 0) {
                    // Try to remove the file
                    if(remove(paths[i]) == 0) {
                        if(verboseFlag) printf("Successfully delete the file from your system\n");
                    } else {
                        printf("An error has occurred, cannot remove from your system the file \"%s\"\nPlease verify that the file is editable and that it is not being used by another process.\n", paths[i]);
                    }
                }
            }
        } else {
            if(verboseFlag) printf(COLOR_RED "Cannot write file \"%s\" while it should be" COLOR_RESET, paths[i]);
        }
    }
}

// A function that is passed to `nftw`to remove all files in a directory then delete the directory
int wipeDir(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf) {
    if(S_ISREG(sb->st_mode) && access(fpath, W_OK) == 0) { // If it's a writable regular file
        if(verboseFlag) printf("Found a writable file : %s\n", fpath);

        if(remove(fpath) == 0) {
            if(verboseFlag) printf("Successfully deleted it\n");
        } else {
            if(verboseFlag) printf("Cannot delete it\n");
        }
    } else if(S_ISDIR(sb->st_mode)) { // If it's a directory
        if(verboseFlag) printf("Found a directory : %s\n", fpath);

        if(rmdir(fpath) == 0) {
            if(verboseFlag) printf("Successfully deleted it\n");
        } else {
            if(verboseFlag) printf("Cannot delete it. It may not be empty\n");
        }
    } else if(S_ISLNK(sb->st_mode)) { // If it's a symbolic link
        if(verboseFlag) printf("Found a symbolic link : %s\n", fpath);

        if(unlink(fpath) == 0) {
            if(verboseFlag) printf("Successfully unlinked it\n");
        } else {
            if(verboseFlag) printf("Cannot unlinked it\n");
        }
    }

    return 0;
}