#ifndef PARSE_H
#define PARSE_H

extern void parse_emit( const char *msg );
extern void parse_emit_ln( const char *msg );

extern void parse_get_char( void );
extern void parse_get_name( void );
extern void parse_get_num( void );

extern int parse_is_addop( const char c );

extern void parse_match( const char s );
extern void parse_init( const char *filename );

extern void parse_expression( void );
extern void parse_ident( void );
extern void parse_assignment( void );
extern void parse_factor( void );
extern void parse_term( void );

extern void parse_multiply( void );
extern void parse_divide( void );
extern void parse_add( void );
extern void parse_subtract( void );

#endif
