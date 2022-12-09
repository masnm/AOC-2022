#define _GNU_SOURCE
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define unused(x) (void) x;

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

long long int find_ans ( const char* input_file )
{
	FILE* input = open_file_for_read ( input_file );
	char* line = NULL; size_t len;
	if ( getline ( &line, &len, input ) == -1 ) {
		fprintf ( stderr, "Failed to read input file %s\n", strerror(errno) );
		exit ( 1 );
	}
	len = strlen ( line );
	size_t ans = -1;
	for ( size_t i = 13 ; i < len ; ++i ) {
		bool ok = true;
		for ( size_t l = i - 13 ; l <= i && ok ; ++l ) {
			for ( size_t m = i - 13 ; m <= i && ok ; ++m ) {
				if ( l == m ) continue;
				if ( line[l] == line[m] ) {
					ok = false;
				}
			}
		}
		if ( ok ) { ans = i; break; }
	}
	free ( line );
	close_file_properly ( input_file, input );
	return ans;
}

int main ( int argc, char* argv[] )
{
	unused ( argc ); unused ( argv );
//	const char* input_file = "sample1.txt";
//	const char* input_file = "sample2.txt";
//	const char* input_file = "sample3.txt";
//	const char* input_file = "sample4.txt";
//	const char* input_file = "sample5.txt";

	const char* input_file = "sample.txt";

	long long int ans = find_ans ( input_file );
	printf ( "Ans is : %lld\n", ans + 1 );

	return 0;
}

