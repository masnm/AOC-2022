#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define unused(x) (void) x

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

uint16_t row = 0, col = 0;
uint8_t heights[1000][1000];

void read_input_from_file ( const char* input_file )
{
	FILE* input = open_file_for_read ( input_file );
	char* line = NULL; size_t len; uint16_t cl = -1;
	while ( getline ( &line, &len, input ) != -1 ) {
		cl = strlen ( line ) - 1;
		for ( uint16_t i = 0 ; i < cl ; ++i )
			heights[row][i] = (uint8_t)(line[i] - '0');
		++row;
	}
	col = cl; assert ( cl != -1 );
	free ( line );
	close_file_properly ( input_file, input );
}

uint64_t get_view_score ( uint16_t _r, uint16_t _c )
{
	if ( _r == 0 || _c == 0 || _r == row - 1 || _c == col - 1 ) return 0;
	uint64_t a, b, c, d;
	int16_t i;
	for ( i = _r - 1 ; i >= 0 ; --i )
		if ( heights[i][_c] >= heights[_r][_c] )
			break;
	if ( i == -1 ) a = _r;
	else a = _r - i;
	for ( i = _r + 1 ; i < row ; ++i )
		if ( heights[i][_c] >= heights[_r][_c] )
			break;
	if ( i == row ) b = i - _r - 1;
	else b = i - _r;

	for ( i = _c - 1 ; i >= 0 ; --i )
		if ( heights[_r][i] >= heights[_r][_c] )
			break;
	if ( i == -1 ) c = _c;
	else c = _c - i;

	for ( i = _c + 1 ; i < col ; ++i )
		if ( heights[_r][i] >= heights[_r][_c] )
			break;
	if ( i == col ) d = i - _c - 1;
	else d = i - _c;

	return a * b * c * d;
}

uint64_t get_ans ( void )
{
	uint64_t ans = 0, ta;
	for ( uint16_t r = 0 ; r < row ; ++r )
		for ( uint16_t c = 0 ; c < col ; ++c ) {
			ta = get_view_score ( r, c );
			if ( ta > ans ) ans = ta;
		}
	return ans;
}

void print_input ( void )
{
	for ( uint16_t i = 0 ; i < row ; ++i ) {
		for ( uint16_t j = 0 ; j < col ; ++j ) {
			printf ( "%d", heights[i][j] );
		}
		puts ( "" );
	}
}

int main ( int argc, char ** argv )
{
	unused ( argc ); unused ( argv );
	const char* file_name = "sample.txt";
	//const char* file_name = "input.txt";
	read_input_from_file ( file_name );
	uint64_t ans = get_ans ();
	printf ( "Ans is : %zu\n", ans );

	return 0;
}
