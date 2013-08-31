#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

typedef enum {
    BAD_EXTENSION,
    FILE_OK
} FILECHECK_t;

extern FILECHECK_t utils_validate_filename( const char *filename );

extern void utils_error( const char *msg );
extern void utils_abort( const char *msg );
extern void utils_expected( const char *msg );

extern void utils_print_file( FILE *fptr );

#endif
