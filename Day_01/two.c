#define _GNU_SOURCE
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long int string_to_lli ( char* line )
{
	long long int ans = 0;
	size_t len = strlen ( line );
	for ( size_t i = 0 ; i < len ; ++i ) {
		if ( line[i] < '0' || line[i] > '9' )
			break;
		ans *= 10;
		ans += (int)(line[i] - '0');
	}
	return ans;
}

long long int ar[10000], sz = 0;

int less ( const void* a, const void* b )
{
	if ( *(long long int*)a < *(long long int*)b ) return -1;
	else return 1;
}

int main ( int argc, char* argv[] )
{
	(void) argc; (void) argv;
	FILE* input = fopen ( "sample.txt", "r" );
//	FILE* input = fopen ( "input.txt", "r" );
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	long long int mx = 0;
	while ( (nread = getline ( &line, &len, input ) ) != -1 ) {
		if ( strcmp ( line, "\n" ) == 0 ) {
			ar[sz++] = mx;
			mx = 0;
		} else {
			mx += string_to_lli ( line );
		}
	}
	ar[sz++] = mx;
	qsort ( ar, sz, sizeof(long long int), less );
	long long int ans = 0;
	for ( long long int i = 1 ; i < 4 ; ++i )
		ans += ar[sz-i];
	printf ( "Ans is : %lld\n", ans );

	free ( line );
	fclose ( input );

	return 0;
}

