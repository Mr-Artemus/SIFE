#ifndef SIFE_WIPE_H
#define SIFE_WIPE_H

    extern int keepFilesAfterOverwriting;
    extern int verboseFlag;

    #include <stdlib.h>
    #include <unistd.h>
    #include <stdio.h>
    #include <math.h>
    #include <ftw.h>

    #include "console.h"
    #include "functions.h"


    // function's pointer type
    typedef unsigned char   (*func_w)(void);
    typedef int             (*func_c)(unsigned char);

    // write methods
    unsigned char _write0       (void); // NOLINT(bugprone-reserved-identifier)
    unsigned char _write1       (void); // NOLINT(bugprone-reserved-identifier)
    unsigned char _write01      (void); // NOLINT(bugprone-reserved-identifier)
    unsigned char _writeRandom  (void); // NOLINT(bugprone-reserved-identifier)

    // check methods
    int _checkNone  (unsigned char byte); // NOLINT(bugprone-reserved-identifier)
    int _check0     (unsigned char byte); // NOLINT(bugprone-reserved-identifier)
    int _check1     (unsigned char byte); // NOLINT(bugprone-reserved-identifier)
    int _check01    (unsigned char byte); // NOLINT(bugprone-reserved-identifier)

    // algorithms structure
    typedef struct {
        char name[50];
        int size;
        func_w writes[16];
        func_c checks[16];
    } algorithm;

    // Define all algorithms
    #define ALGO_SIMPLE_0_PASS              ((algorithm) { "Simple pass of 0b00000000",  1, { &_write0 }, { &_check0 } })
    #define ALGO_SIMPLE_1_PASS              ((algorithm) { "Simple pass of 0b11111111",  1, { &_write1 }, { &_check1 } })
    #define ALGO_SIMPLE_01_PASS             ((algorithm) { "Simple pass of 0b01010101",  1, { &_write01 }, { &_check01 } })
    #define ALGO_SIMPLE_RANDOM_PASS         ((algorithm) { "Simple pass of pseudo-random byte", 1, { &_writeRandom }, { &_checkNone } })

    #define ALGO_GOST_R_50739_95_FIRST      ((algorithm) { "GOST-R-50739-95 first version",   2, { &_write0, &_writeRandom }, { &_checkNone, &_checkNone } })
    #define ALGO_GOST_R_50739_95_SECOND     ((algorithm) { "GOST-R-50739-95 second version",  1, { &_writeRandom }, { &_checkNone } })

    #define ALGO_HMG_IS5                    ((algorithm) { "HMG IS5",           2, { &_write0, &_writeRandom }, { &_check0, &_checkNone } })
    #define ALGO_HMG_IS5_ENHANCED           ((algorithm) { "HMG IS5 Enhanced",  3, { &_write0, &_write1, &_writeRandom }, { &_check0, &_check1, &_checkNone } })

    #define ALGO_RCMP_TSSIT_OPS_II          ((algorithm) { "RCMP TSSIT OPS-II", 7, { &_write0, &_write1, &_write0, &_write1, &_write0, &_write1, &_writeRandom }, { &_check0, &_check1, &_check0, &_check1, &_check0, &_check1, &_checkNone } })

    #define ALGO_AFSSI_5020                 ((algorithm) { "AFSSI-5020",  3, { &_write0, &_write1, &_writeRandom }, { &_check0, &_check1, &_checkNone } })

    #define ALGO_AR_380_19                  ((algorithm) { "AR 380-19",   3, { &_writeRandom, &_write1, &_write0 }, { &_checkNone, &_check1, &_check0 } })

    #define ALGO_BRUCE_SCHNEIER             ((algorithm) { "Bruce Schneier Algorithm", 7, { &_write0, &_write1, &_writeRandom, &_writeRandom, &_writeRandom, &_writeRandom, &_writeRandom }, { &_check0, &_check1, &_checkNone, &_checkNone, &_checkNone, &_checkNone, &_checkNone } })




    // wipe method
    void wipeFiles(char paths[][MAX_PATH_LENGTH], size_t pathsLength, algorithm algorithm);
    int wipeDir(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf);


#endif //SIFE_WIPE_H
