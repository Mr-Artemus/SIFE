#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <time.h>
#include <ftw.h>

#include "console.h"
#include "functions.h"
#include "wipe.h"

// Version
#define VERSION "0.1.0"


// flag to check if the file(s) need to be deleted or not
int keepFilesAfterOverwriting = 0;
// flag to check if verbose mode need to be activated
int verboseFlag = 0;

// Array of all available algorithms
static algorithm methodsList[12] = {
        ALGO_SIMPLE_0_PASS,
        ALGO_SIMPLE_1_PASS,
        ALGO_SIMPLE_01_PASS,
        ALGO_SIMPLE_RANDOM_PASS,
        ALGO_GOST_R_50739_95_FIRST,
        ALGO_GOST_R_50739_95_SECOND,
        ALGO_HMG_IS5,
        ALGO_HMG_IS5_ENHANCED,
        ALGO_RCMP_TSSIT_OPS_II,
        ALGO_AFSSI_5020,
        ALGO_AR_380_19,
        ALGO_BRUCE_SCHNEIER
};

// Variable to store the selected method to wipe
static int method;

int main(int argc, char **argv) {
    // random seed
    srand(time(NULL)); // NOLINT(cert-msc51-cpp)

    // Check if there are 1 parameter or more
    if(argc >= 2) {
        int opt;

        // options definition
        static struct option long_options[] = {
            {"verbose", no_argument,        &verboseFlag,                1},
            {"keep",    no_argument,        &keepFilesAfterOverwriting,  1},

            {"help",    no_argument,        NULL,                   'h'},
            {"method",  required_argument,  NULL,                   'm'},

            {0,0,0,0} // To close the loop
        };

        // disable getopt error in console
        opterr = 0;

        // for each options
        while ((opt = getopt_long(argc, argv, "hm:", long_options, NULL)) != -1) {
            // which option
            switch(opt) {
                // print help message
                case 'h':
                    printf("S.I.F.E. version %s\n", VERSION);
                    printf("\nUSAGE: %s [options] file...\n", argv[0]);
                    printf("\nOPTIONS:\n");
                    printf("-h | --help         Show the help message\n");
                    printf("-m | --method       Select wiping method\n");
                    printf("--verbose           Active verbose mode\n");
                    printf("--keep              Keep file(s) after overwriting\n");
                    printf("\n");
                    exit(EXIT_SUCCESS);

                case 'm':
                    // get method from user choice
                    method = atoi(optarg);
                    // If the option has a value that is between 0 and 12
                    if(optarg && method > 0 && method <= 12) {
                        // Decrement to correspond to the array key
                        method -= 1;
                    } else {
                        printf("Invalid method argument, please retry the command without any argument to get the list of all methods\n");
                        exit(EXIT_FAILURE);
                    }
                    break;

                case '?':
                    // If the 'method' option does not has a value
                    if (optopt == 'm') {
                        // Show all available methods
                        printf("List of all usable methods :\n");
                        printf(" 1 - Simple pass of 0b00000000\n");
                        printf(" 2 - Simple pass of 0b11111111\n");
                        printf(" 3 - Simple pass of 0b01010101\n");
                        printf(" 4 - Simple pass of pseudo-random byte\n");
                        printf(" 5 - GOST-R-50739-95 first version\n");
                        printf(" 6 - GOST-R-50739-95 second version\n");
                        printf(" 7 - HMG IS5\n");
                        printf(" 8 - HMG IS5 Enhanced\n");
                        printf(" 9 - RCMP TSSIT OPS-II\n");
                        printf("10 - AFSSI-5020\n");
                        printf("11 - AR 380-19\n");
                        printf("12 - Bruce Schneier Algorithm\n");

                        exit(EXIT_FAILURE);
                    } else {
                        // unknown option
                        printf("Unknown option %s.\n", argv[optind -1]);
                        printf("See %s --help for more information\n", argv[0]);
                        exit(EXIT_FAILURE);
                    }

                default:
                    break;
            }
        }

        if(verboseFlag) printf("Starting program...\n");

        // count all files from all paths
        size_t numberOfFiles = 0;
        size_t numberOfFilesWritable = 0;
        size_t listCount[argc - optind];
        for(int i = optind; i < argc; i++) {
            // get the count of file writable and not writable
            int found               = countAllRegularFilesInDirectory(argv[i], 0, 1);
            int foundWritable       = countAllRegularFilesInDirectory(argv[i], 1, 1);

            numberOfFiles           += found;
            numberOfFilesWritable   += foundWritable;
            listCount[i - optind]    = numberOfFilesWritable;

            // If verbose mod, show the discovered files for each asked path
            if(verboseFlag) printf("For argument \"%s\", found %d file%s and %d is not writable\n", argv[i], found, found > 1 ? "s" : "", found - foundWritable);
        }

        // Show the number of files detected
        printf("%zu file%s detected\n", numberOfFiles, numberOfFiles > 1 ? "s" : "");
        // If there are file that is no writable, show an alert
        if(numberOfFiles > numberOfFilesWritable) {
            printf("But only %zu file%s is writable\n", numberOfFilesWritable, numberOfFilesWritable > 1 ? "s" : "");
            printf(COLOR_RED "Please make sure that all files are writable, otherwise the program will not be effective !\n" COLOR_RESET);

            if(verboseFlag) {
                // show all non-writable files
                char a[numberOfFiles - numberOfFilesWritable][MAX_PATH_LENGTH];
                getAllRegularFilesInDirectory(argv[optind], a, 0, 2, 1);
                for (int i = 0; i < numberOfFiles - numberOfFilesWritable; i++) {
                    printf("The file \"%s\" is not writable\n", a[i]);
                }
            }

            printf("\n\n");
        }


        // Now, we'll get all paths
        size_t it = 0;
        // Create an array of strings that contain the number of files discovered
        char paths[numberOfFilesWritable][MAX_PATH_LENGTH];
        // for each argument
        for(int i = optind; i < argc; i++) {
            // get all files paths
            getAllRegularFilesInDirectory(argv[i], paths, it, 1, 1);

            if(verboseFlag) {
                for (size_t j = it; j < (it + listCount[i - optind]); j++) {
                    printf("For argument \"%s\", found file \"%s\"\n", argv[i], paths[j]);
                }
            }

            // Increment the iterator
            it += listCount[i - optind];
        }

        if(numberOfFilesWritable > 0) {
            // Wipe files
            wipeFiles(paths, numberOfFilesWritable, methodsList[method]);

            // Delete dirs
            if(keepFilesAfterOverwriting == 0) {
                for (int i = optind; i < argc; i++) {
                    if (isDirectory(argv[i])) {
                        if (verboseFlag) printf("Trying to delete folder of argument \"%s\"\n", argv[i]);

                        // try to remove dir and his left content if writable
                        nftw(argv[i], wipeDir, 32, FTW_DEPTH | FTW_PHYS);
                    }
                }
            }
        }
    } else {
        // show usage
        printf("USAGE: %s [options] file...\nSee %s --help for more information\n", argv[0], argv[0]);
    }

    return EXIT_SUCCESS;
}
