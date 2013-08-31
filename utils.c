#include <stdlib.h>
#include <string.h>

#include "utils.h"

FILECHECK_t utils_validate_filename( const char *filename ) {
        int length = 0;
        size_t EXT_LENGTH = 6;
        int cmp = -1;
        FILECHECK_t result = FILE_OK;

        length = strlen( filename );

        if ( length > EXT_LENGTH ) {
            cmp = strcmp( filename + (length - EXT_LENGTH), ".xwell" );
        }

        if ( cmp != 0 ) {
            result = BAD_EXTENSION;
        }

        return result;
}

void utils_error( const char *msg ) {
    printf( "ERROR: %s\n", msg );
}

void utils_abort( const char *msg ) {
    utils_error( msg );
    exit( EXIT_FAILURE );
}

void utils_expected( const char *msg ) {
    char msgbf[ 50 ] = "\0";
    sprintf( msgbf, "Expected - '%s'", msg );
    
    utils_abort( msgbf );
}

void utils_print_file( FILE *fptr ) {
    char ch;

    if ( fptr != NULL ) {
        while( ( ch = fgetc( fptr ) ) != EOF ) {
            printf( "%c", ch );
        }
    } else {
        utils_error( "File Content DNE" );
    }
}
