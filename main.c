#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static unsigned int g_row = 1;
static unsigned int g_col = 0;

const char NEWLINE = '\n';
const char CARRIAGE_RETURN = '\r';
const char TAB = '\t';
const char SPACE = ' ';
const char PLUS = '+';
const char MINUS = '-';
const char MULTIPLY = '*';
const char DIVIDE = '/';
const char OPEN_PARENS = '(';
const char CLOSED_PARENS = ')';
const char EQUALS = '=';

FILE* g_fptr = NULL;
static char g_look;

typedef enum {
    BAD_EXTENSION,
    FILE_OK
} FILECHECK_t;

FILECHECK_t ValidateFilename( const char* filename );

void Error( const char* msg );
void Abort( const char* msg );
void Expected( const char* msg );

void ReadFile( FILE* fptr );

void Emit( const char* msg );
void EmitLn( const char* msg );

void GetChar( void );
void GetName( void );
void GetNum( void );

int IsAddOp( const char* c );

void Match( const char* s );
void Init( const char* filename );

void Expression( void );
void Ident( void );
void Assignment( void );
void Factor( void );
void Term( void );

void Multiply( void );
void Divide( void );
void Add( void );
void Subtract( void );

int main( int argc, char* argv[] ) {
    char* filename = NULL;

    if ( argc == 2 ) {
        filename = argv[ 1 ];

        switch ( ValidateFilename( filename ) ) {
            case FILE_OK:
                printf( "Input File: %s\n", filename );
                
                Init( filename );
                Assignment();

                fclose( g_fptr );

                break;
            case BAD_EXTENSION:
                Error( "Expected a file in the format of '< filename >.max'" );
                break;
            default:
                Error( "Unknown FILECHECK_t Error" );
        }
    } else {
        char msgbf[ 50 ];
        sprintf( msgbf, "Usage - %s %s", argv[ 0 ], "< filename >.max" );

        Error( msgbf );
    }

    filename = NULL;

    exit( EXIT_SUCCESS );
}

FILECHECK_t ValidateFilename( const char* filename ) {
        int length = 0;
        int cmp = -1;
        FILECHECK_t result = FILE_OK;

        length = strlen( filename );

        if ( length > 4 ) {
            cmp = strcmp( filename + (length - 4), ".max" );
        }

        if ( cmp != 0 ) {
            result = BAD_EXTENSION;
        }

        return result;
}

void Error( const char* msg ) {
    printf( "ERROR: %s\n", msg );
}

void Abort( const char* msg ) {
    Error( msg );
    exit( EXIT_FAILURE );
}

void Expected( const char* msg ) {
    char msgbf[ 50 ];
    sprintf( msgbf, "Expected - '%s'", msg );
    
    Abort( msgbf );
}

void ReadFile( FILE* fptr ) {
    char ch;

    if ( fptr != NULL ) {
        while( ( ch = fgetc( fptr ) ) != EOF ) {
            printf( "%c", ch );
        }
    } else {
        Error( "File Content DNE" );
    }
}

void Emit( const char* str ) {
    printf( "[ %d, %d ]: %s", g_row, g_col, str );
}

void EmitLn( const char* str ) {
    char msgbf[ 50 ];
    sprintf( msgbf, "%s\n", str );

    Emit( msgbf );
}

void GetChar( void ) {
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
                EmitLn( &g_look );
            } else {
                EmitLn( "EOF" );
            }
        } else {
            if ( g_look == SPACE ) {
                EmitLn( "SPACE" );
            } else if ( g_look == TAB ) {
                EmitLn( "TAB" );
            } else if ( g_look == NEWLINE ) {
                EmitLn( "NEWLINE" );
            } else if ( g_look == CARRIAGE_RETURN ) {
                EmitLn( "CARRIAGE_RETURN" );
            }

        }
    } while ( isspace( g_look ) && g_look != EOF );
}

int IsAddOp( const char* c ) {
    return ( *c == PLUS || *c == MINUS );
}

void Match( const char* s ) {
    if ( g_look == *s ) {
        GetChar();
    } else {
        Expected( s );
    }
}

void GetName( void ) {
    if ( !isalpha( g_look ) ) {
        Expected( "Name" );
    }

    GetChar();
}

void GetNum( void ) {
    if ( !isdigit( g_look ) ) {
        Expected( "Integer" );
    }

    GetChar();
}

void Init( const char* filename ) {
    g_fptr = fopen( filename, "r" );

    if ( g_fptr == NULL ) {
        Abort( "Failed to open file." );
    }

    GetChar();
}

void Expression( void ) {
    if ( IsAddOp( &g_look ) ) {
        // Add zero in front to subtract.
        EmitLn( "Negate Value." );
    } else {
        Term();
    }

    while ( IsAddOp( &g_look ) ) {
        if ( g_look == PLUS ) {
            Add();
        } else if ( g_look == MINUS ) {
            Subtract();
        }
    }
}

void Ident( void ) {
    GetName();

    if ( g_look == OPEN_PARENS ) {
        Match( &OPEN_PARENS );
        Match( &CLOSED_PARENS );
    }
}

void Assignment( void ) {
    GetName();

    Match( &EQUALS );
    Expression();
}

void Factor( void ) {
    if ( g_look == OPEN_PARENS ) {
        Match( &OPEN_PARENS );
        Expression();
        Match( &CLOSED_PARENS );
    } else if ( isalpha( g_look ) ) {
        Ident();    
    }else {
        GetNum();
    }
}

void Multiply( void ) {
    Match( &MULTIPLY );
    Factor();
}

void Divide( void ) {
    Match( &DIVIDE );
    Factor();
}

void Term( void ) {
    Factor();

    while ( g_look == MULTIPLY || g_look == DIVIDE ) {
        if ( g_look == MULTIPLY ) {
            Multiply();
        } else if ( g_look == DIVIDE ) {
            Divide();
        } 
    }
}

void Add( void ) {
    Match( &PLUS );
    Term();
}

void Subtract( void ) {
    Match( &MINUS );

    Term();
}

