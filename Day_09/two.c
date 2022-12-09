#include <assert.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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

typedef struct {
	int16_t row;
	int16_t col;
} pair ;

#define MAX_LEN 1000
bool grid[MAX_LEN][MAX_LEN];

void fix_tail ( pair* head, pair* tail )
{
	if ( head->row == tail->row && head->col == tail->col ) return;
	int16_t rd = head->row - tail->row, cd = head->col - tail->col;
	if ( abs ( rd ) < 2 && abs ( cd ) < 2 ) return;
	if ( head->row == tail->row ) {
		int16_t diff = head->col - tail->col;
		if ( diff > 1 || diff < -1 ) {
			diff = ( diff / abs(diff) );
			tail->col += diff;
		}
	} else if ( head->col == tail->col ) {
		int16_t diff = head->row - tail->row;
		if ( diff > 1 || diff < -1 ) {
			diff = ( diff / abs(diff) );
			tail->row += diff;
		}
	} else {
		int16_t diff = ( rd / abs(rd) );
		tail->row += diff;
		diff = ( cd / abs(cd) );
		tail->col += diff;
	}
}

void simulate_instructions ( const char* input_file )
{
	FILE* input = open_file_for_read ( input_file );
	pair head = { MAX_LEN/2, MAX_LEN/2 }, tail[9], change;
	for ( size_t i = 0 ; i < 9 ; ++i ) tail[i] = head;
	char dir; size_t count;
	while ( fscanf ( input, "%c %zu\n", &dir, &count ) != EOF ) {
		switch ( dir ) {
			case 'U':
				change = (pair) { .row = 0, .col = -1 };
				break;
			case 'D':
				change = (pair) { .row = 0, .col = 1 };
				break;
			case 'L':
				change = (pair) { .row = -1, .col = 0 };
				break;
			case 'R':
				change = (pair) { .row = 1, .col = 0 };
				break;
			default:
				fprintf ( stderr, "Unknown Direction found\n" );
				break;
		}
		for ( size_t i = 0 ; i < count ; ++i ) {
			head.row += change.row; head.col += change.col;
			fix_tail ( &head, &tail[0] );
			for ( size_t j = 0 ; j < 8 ; ++j )
				fix_tail ( &tail[j], &tail[j+1] );
			grid[tail[8].row][tail[8].col] = true;
		}
	}
	close_file_properly ( input_file, input );
}

size_t get_ans ( void )
{
	size_t ans = 0;
	for ( uint16_t i = 0 ; i < MAX_LEN ; ++i )
		for ( uint16_t j = 0 ; j < MAX_LEN ; ++j )
			if ( grid[i][j] ) ++ans;
	return ans;
}

int main ( int argc, char* argv[] )
{
	unused ( argc ); unused ( argv );
	const char* input_file = "sample.txt";
//	const char* input_file = "input.txt";
	simulate_instructions ( input_file );
	size_t ans = get_ans ();
	printf ( "Ans is : %zu\n", ans );

	return 0;
}

