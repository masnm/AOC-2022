#define _GNU_SOURCE
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#define unused(x) (void) x
#define MAX_LINE_LEN 105

FILE* open_file_properly ( const char* input_file )
{
	FILE* input = fopen ( input_file, "r" );
	if ( input == NULL ) {
		fprintf ( stderr, "Can't open %s : %s\n",
				input_file, strerror ( errno ) );
		exit ( 1 );
	}
	return input;
}

void close_file_properly ( const char* input_file, FILE* input )
{
	if ( fclose ( input ) != 0 ) {
		fprintf ( stderr, "Can't close %s : %s\n",
				input_file, strerror ( errno ) );
		exit ( 1 );
	}
}

const int32_t SCR_LEN = 240;
char *sprite, *screen;
int64_t considerations[6] = { 20, 60, 100, 140, 180, 220 };
int64_t X = 0, cycle = 0, shift = 0;

int64_t cycle_in_considerations ( void )
{
	for ( size_t i = 0 ; i < 6 ; ++i )
		if ( considerations[i] == cycle )
			return considerations[i];
	return 0;
}

void noop_operation ( void )
{
	if ( sprite[cycle] == '#' ) screen[cycle + shift] = '#';
	else screen[cycle + shift] = '.';
	++cycle;
	if ( cycle >= 40 ) { cycle -= 40; shift += 40; }
}

void addx_operation ( int32_t count )
{
	if ( sprite[cycle] == '#' ) screen[cycle + shift] = '#';
	else screen[cycle + shift] = '.';
	++cycle;
	if ( cycle >= 40 ) { cycle -= 40; shift += 40; }
	if ( sprite[cycle] == '#' ) screen[cycle + shift] = '#';
	else screen[cycle + shift] = '.';
	++cycle;
	if ( cycle >= 40 ) { cycle -= 40; shift += 40; }
	X += count;
	for ( int32_t i = 0 ; i < SCR_LEN ; ++i ) sprite[i] = '.';
	for ( int64_t i = X ; i < X + 3 ; ++i )
		if ( i >= 0 && i < SCR_LEN  )
			sprite[i] = '#';
}

void read_and_execure_instructions ( const char* input_file )
{
	FILE* input = open_file_properly ( input_file );
	char *line = NULL, instruction[MAX_LINE_LEN];
	const char *ins_add = "addx", *ins_nop = "noop";
	int32_t count; size_t len;
	while ( getline ( &line, &len, input ) != EOF ) {
		sscanf ( line, "%s %d\n", instruction, &count );
		if ( strcmp ( instruction, ins_nop ) == 0 ) {
			noop_operation ();
		} else if ( strcmp ( instruction, ins_add ) == 0 ) {
			addx_operation ( count );
		} else {
			fprintf ( stderr, "Wrong instruction found\n" );
		}
		if ( cycle > 40 ) break;
	}
	free ( line );
	close_file_properly ( input_file, input );
}

int main ( int argc, char* argv[] )
{
	unused ( argc ); unused ( argv );
	const char* input_file = "sample.txt";
//	const char* input_file = "input.txt";
	screen = (char*) malloc ( sizeof ( char ) * SCR_LEN );
	sprite = (char*) malloc ( sizeof ( char ) * SCR_LEN );
	for ( int32_t i = 0 ; i < SCR_LEN ; ++i ) {
		screen[i] = sprite[i] = '.';
	}
	sprite[0] = sprite[1] = sprite[2] = '#';
	read_and_execure_instructions ( input_file );
	for ( int32_t i = 0 ; i < SCR_LEN ; i += 40 ) {
		for ( int32_t j = i ; j < i + 40 ; ++j ) {
			if ( screen[j] == '.' ) screen[j] = ' ';
			printf ( "%c", screen[j] );
		}
		puts ( "" );
	}
	free ( sprite );
	free ( screen );

	return 0;
}

