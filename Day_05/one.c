#define _GNU_SOURCE
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define unused(x) (void)x

#define MAX_STACK_CAP 200
typedef struct {
	size_t index;
	size_t size;
	char data[MAX_STACK_CAP];
} stack ;
stack stacks[100];
size_t stacks_count = 0;

void stacks_push ( size_t index, const char ch )
{
	assert ( stacks[index].size < MAX_STACK_CAP - 1 );
	stacks[index].data[stacks[index].size] = ch;
	++ stacks[index].size;
}

char stacks_pop ( size_t index )
{
	assert ( stacks[index].size > 0 );
	return stacks[index].data[--stacks[index].size];
}

void send_stacks_data ( size_t count, size_t st_ind, size_t en_ind )
{
	-- st_ind; -- en_ind;
	for ( size_t i = 0 ; i < count ; ++i )
		stacks_push ( en_ind, stacks_pop ( st_ind ) );
}

FILE* open_file_for_read ( const char* input_file )
{
	FILE* input = fopen ( input_file, "r" );
	if ( input == NULL ) {
		fprintf ( stderr, "Can't open %s cause %s\n",
				input_file, strerror ( errno ) );
		exit ( 1 );
	}
	return input;
}

void close_file_properly ( const char* input_file, FILE* input )
{
	if ( fclose ( input ) != 0 ) {
		fprintf ( stderr, "Can't close %s cause %s\n",
				input_file, strerror ( errno ) );
		exit ( 1 );
	}
}

void get_stacks_count ( const char* input_file )
{
	FILE* input = open_file_for_read ( input_file );
	char* line = NULL;
	size_t len;
	while ( getline ( &line, &len, input ) != -1 ) {
		if ( strcmp ( line, "\n" ) == 0 ) {
			fprintf ( stderr, "Skipped the stacks_count\n" );
			break;
		}
		size_t length = strlen ( line );
		bool this_is_that = true;
		for ( size_t i = 0 ; i < length ; ++i ) {
			if ( line[i] == '[' || line[i] == ']' ) {
				this_is_that = false;
				break;
			}
		}
		if ( this_is_that ) {
			for ( size_t i = 0 ; i < length ; ++i ) {
				size_t this_val = (size_t) line[i];
				if ( this_val > stacks_count )
					stacks_count = this_val;
			}
			stacks_count -= (size_t) '0';
			break;
		}
	}
	free ( line );
	close_file_properly ( input_file, input );
}

void strrev ( char* data, size_t size )
{
	for ( size_t i = 0 ; i < size/2 ; ++i ) {
		char t = data[i];
		data[i] = data[size-1-i];
		data[size-1-i] = t;
	}
}

void populate_the_stacks ( const char* input_file )
{
	FILE* input = open_file_for_read ( input_file );
	char* line = NULL;
	size_t len;
	while ( getline ( &line, &len, input ) != -1 ) {
		if ( strcmp ( line, "\n" ) == 0 ) break;
		size_t length = strlen ( line );
		char prev = 0;
		for ( size_t i = 0 ; i < length ; ++i ) {
			if ( prev == '[' ) {
				stacks_push ( i/4, line[i] );
			}
			prev = line[i];
		}
	}
	for ( size_t i = 0 ; i < stacks_count ; ++i )
		strrev ( stacks[i].data, stacks[i].size );
	free ( line );
	close_file_properly ( input_file, input );
}

void print_stacks ( void )
{
	for ( size_t i = 0 ; i < stacks_count ; ++i ) {
		fprintf ( stderr, "Stack %zu : ", i );
		for ( size_t j = 0 ; j < stacks[i].size ; ++j )
			fprintf ( stderr, "%c ", stacks[i].data[j] );
		fprintf ( stderr, "\n" );
	}
}

void simulate_operations ( const char* input_file )
{
	FILE* input = open_file_for_read ( input_file );
	char* line = NULL;
	size_t len;
	while ( getline ( &line, &len, input ) != -1 )
		if ( strcmp ( line, "\n" ) == 0 ) break;
	while ( getline ( &line, &len, input ) != -1 ) {
		size_t count, st_ind, en_ind;
		sscanf ( line, "move %zu from %zu to %zu\n",
				&count, &st_ind, &en_ind );
		send_stacks_data ( count, st_ind, en_ind );
	}
	free ( line );
	close_file_properly ( input_file, input );
}

int main ( int argc, char* argv[] )
{
	unused ( argc ); unused ( argv );
	const char* input_file = "sample.txt";
//	const char* input_file = "input.txt";
	get_stacks_count ( input_file );
	populate_the_stacks ( input_file );
	simulate_operations ( input_file );
	//fprintf ( stderr, "SC : %zu\n", stacks_count );
	//print_stacks ();
	printf ( "Ans is : " );
	for ( size_t i = 0 ; i < stacks_count ; ++i )
		printf ( "%c", stacks[i].data[stacks[i].size-1] );

	return 0;
}

