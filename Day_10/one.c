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

int64_t considerations[6] = { 20, 60, 100, 140, 180, 220 };
int64_t X = 1, cycle = 1, final_ans = 0;

int64_t cycle_in_considerations ( void )
{
	for ( size_t i = 0 ; i < 6 ; ++i )
		if ( considerations[i] == cycle )
			return considerations[i];
	return 0;
}

void noop_operation ( void )
{
	final_ans += cycle_in_considerations() * X;
	++cycle;
}

void addx_operation ( int32_t count )
{
	final_ans += cycle_in_considerations() * X;
	++cycle;
	final_ans += cycle_in_considerations() * X;
	++cycle;
	X += count;
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
	}
	free ( line );
	close_file_properly ( input_file, input );
}

int main ( int argc, char* argv[] )
{
	unused ( argc ); unused ( argv );
	const char* input_file = "sample.txt";
//	const char* input_file = "input.txt";
	read_and_execure_instructions ( input_file );
	printf ( "Ans is : %ld\n", final_ans );

	return 0;
}

