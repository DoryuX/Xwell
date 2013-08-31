#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"
#include "parse.h"

FILE *g_fptr = NULL;

int main( int argc, char *argv[] ) {
    char *filename = NULL;
    char *msg = NULL;

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
                msg = "Expected a file in the format of '< filename >.xwell'";
                utils_error( msg, strlen( msg ) );
                break;
            default:
                msg = "Unknown FILECHECK_t Error";
                utils_error( msg, strlen( msg ) );
        }
    } else {
        char msgbf[ 100 ] = "\0";
        size_t n = sprintf( msgbf, "Usage - %s %s", argv[ 0 ], "< filename >.xwell" );
        utils_error( msgbf, n );
    }

    msg = NULL;
    filename = NULL;

    exit( EXIT_SUCCESS );
}
