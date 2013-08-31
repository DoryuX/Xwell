#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "utils.h"
#include "parse.h"

FILE *g_fptr = NULL;

int main( int argc, char *argv[] ) {
    char *filename = NULL;

    if ( argc == 2 ) {
        filename = argv[ 1 ];

        switch ( utils_validate_filename( filename ) ) {
            case FILE_OK:
                printf( "Input File: %s\n", filename );
                
                parse_init( filename );
                parse_assignment();

                fclose( g_fptr );

                break;
            case BAD_EXTENSION:
                utils_error( "Expected a file in the format of '< filename >.xwell'" );
                break;
            default:
                utils_error( "Unknown FILECHECK_t Error" );
        }
    } else {
        char msgbf[ 50 ] = "\0";
        sprintf( msgbf, "Usage - %s %s", argv[ 0 ], "< filename >.xwell" );

        utils_error( msgbf );
    }

    filename = NULL;

    exit( EXIT_SUCCESS );
}
