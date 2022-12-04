#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define unused(x) (void) x

typedef struct {
	size_t sz;
	char* data;
} string ;

#define MAX_STR_LEN 1000
string strings[MAX_STR_LEN];
size_t strings_count = 0;

void read_strings ( char* file_name )
{
	FILE* file = fopen ( file_name, "r" );
	if ( file == NULL ) {
		fprintf ( stderr, "Can't open %s: %s\n",
				file_name, strerror(errno) );
		exit ( 1 );
	}
	while ( true ) {
		strings[strings_count].data = (char*) malloc ( sizeof(char)*100 );
		if ( fscanf ( file, "%99s", strings[strings_count].data )
				== EOF ) {
			free ( strings[strings_count].data );
			strings[strings_count].data = NULL;
			break;
		}
		strings[strings_count].sz =
			strlen ( strings[strings_count].data );
		++ strings_count;
	}
	fclose ( file );
}

char match_character ( char* left, char* right )
{
	size_t len_left = strlen ( left ),
			len_right = strlen ( right );
	size_t matched_count = 0;
	char ans = 0;
	for ( size_t i = 0 ; i < len_left ; ++i ) {
		for ( size_t j = 0 ; j < len_right ; ++j ) {
			if ( left[i] == right[j] ) {
				if ( matched_count != 0 ) {
					if ( ans != left[i] )
						++ matched_count;
				} else {
					++ matched_count;
					ans = left[i];
				}
			}
		}
	}
	if ( matched_count != 1 ) {
		fprintf ( stderr, "More than one match in strings\n" );
		exit ( 1 );
	}
	return ans;
}

long long int process_the_strings ( void )
{
	for ( size_t i = 0 ; i < strings_count ; ++i ) {
		if ( strings[i].sz & (size_t)1 ) {
			fprintf ( stderr, "String size not even\n" );
			return -1;
		}
	}
	long long int ans = 0, ta;
	for ( size_t i = 0 ; i < strings_count ; ++i ) {
		size_t half = strings[i].sz / (size_t)2;
		char* left = (char*) malloc ( sizeof(char) * half + 1 );
		char* right = (char*) malloc ( sizeof(char) * half + 1 );
		memcpy ( left, strings[i].data, half ); left[half] = '\0';
		memcpy ( right, strings[i].data+half, half ); right[half] = '\0';
		char ch = match_character ( left, right );
		if ( ch >= 'a' && ch <= 'z' ) ta = ch - 'a' + 1;
		else if ( ch >= 'A' && ch <= 'Z' ) ta = ch - 'A' + 27;
		else {
			fprintf ( stderr, "Unknown character matched\n" );
			exit ( 1 );
		}
		ans += ta;
		free ( left ); free ( right );
	}
	return ans;
}

void destroy_strings ( void )
{
	for ( size_t i = 0 ; i < strings_count ; ++i ) {
		strings[i].sz = 0;
		if ( strings[i].data != NULL ) free ( strings[i].data );
		strings[i].data = NULL;
	}
	strings_count = 0;
}

int main ( int argc, char* argv[] )
{
	unused ( argc ); unused ( argv );

	read_strings ( "sample.txt" );
//	read_strings ( "input.txt" );
	long long int ans = process_the_strings ();
	printf ( "Ans is : %lld\n", ans );
	destroy_strings ();

	return 0;
}
