#include "parse.h"

#include <string.h>
#include <ctype.h>

#include "ident.h"
#include "utils.h"

static size_t g_row = 1;
static size_t g_col = 0;
static char g_look;

extern FILE *g_fptr;

void parse_emit( const char* str ) {
    printf( "[ %zd, %zd ]: %s", g_row, g_col, str );
}

void parse_emit_ln( const char* str ) {
    char msgbf[ 50 ];
    sprintf( msgbf, "%s\n", str );

    parse_emit( msgbf );
}

void parse_get_char( void ) {
    do {
        g_look = fgetc( g_fptr );

        if ( g_look != NEWLINE ) {
            ++g_col; 
        } else {
            g_col = 0; 
            ++g_row;
        }

        if ( !isspace( g_look ) ) {
            if ( g_look != EOF ) {
                parse_emit_ln( &g_look );
            } else {
                parse_emit_ln( "EOF" );
            }
        } else {
            if ( g_look == SPACE ) {
                parse_emit_ln( "SPACE" );
            } else if ( g_look == TAB ) {
                parse_emit_ln( "TAB" );
            } else if ( g_look == NEWLINE ) {
                parse_emit_ln( "NEWLINE" );
            } else if ( g_look == CARRIAGE_RETURN ) {
                parse_emit_ln( "CARRIAGE_RETURN" );
            }

        }
    } while ( isspace( g_look ) && g_look != EOF );
}

int parse_is_addop( const char c ) {
    return ( c == PLUS || c == MINUS );
}

void parse_match( const char s ) {
    if ( g_look == s ) {
        parse_get_char();
    } else {
        utils_expected( &s );
    }
}

void parse_get_name( void ) {
    if ( !isalpha( g_look ) ) {
        utils_expected( "Name" );
    }

    parse_get_char();
}

void parse_get_num( void ) {
    if ( !isdigit( g_look ) ) {
        utils_expected( "Integer" );
    }

    parse_get_char();
}

void parse_init( const char* filename ) {
    g_fptr = fopen( filename, "r" );

    if ( g_fptr == NULL ) {
        char *msg = "Failed to open file."; 
        utils_abort( msg, strlen( msg ) );
    }

    parse_get_char();
}

void parse_expression( void ) {
    if ( parse_is_addop( g_look ) ) {
        // Add zero in front to subtract.
        parse_emit_ln( "Negate Value." );
    } else {
        parse_term();
    }

    while ( parse_is_addop( g_look ) ) {
        if ( g_look == PLUS ) {
            parse_add();
        } else if ( g_look == MINUS ) {
            parse_subtract();
        }
    }
}

void parse_ident( void ) {
    parse_get_name();

    if ( g_look == OPEN_PARENS ) {
        parse_match( OPEN_PARENS );
        parse_match( CLOSED_PARENS );
    }
}

void parse_assignment( void ) {
    parse_get_name();

    parse_match( EQUALS );
    parse_expression();
}

void parse_factor( void ) {
    if ( g_look == OPEN_PARENS ) {
        parse_match( OPEN_PARENS );
        parse_expression();
        parse_match( CLOSED_PARENS );
    } else if ( isalpha( g_look ) ) {
        parse_ident();    
    }else {
        parse_get_num();
    }
}

void parse_multiply( void ) {
    parse_match( MULTIPLY );
    parse_factor();
}

void parse_divide( void ) {
    parse_match( DIVIDE );
    parse_factor();
}

void parse_term( void ) {
    parse_factor();

    while ( g_look == MULTIPLY || g_look == DIVIDE ) {
        if ( g_look == MULTIPLY ) {
            parse_multiply();
        } else if ( g_look == DIVIDE ) {
            parse_divide();
        } 
    }
}

void parse_add( void ) {
    parse_match( PLUS );
    parse_term();
}

void parse_subtract( void ) {
    parse_match( MINUS );

    parse_term();
}

