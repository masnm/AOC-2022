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

long long int max ( long long int a, long long int b )
{
	return (a > b ? a : b);
}

int main ( int argc, char* argv[] )
{
	(void) argc; (void) argv;
	FILE* input = fopen ( "sample.txt", "r" );
//	FILE* input = fopen ( "input.txt", "r" );
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	long long int mx = 0, total_mx = 0;
	while ( (nread = getline ( &line, &len, input ) ) != -1 ) {
		if ( strcmp ( line, "\n" ) == 0 ) {
			total_mx = max ( total_mx, mx );
			mx = 0;
		} else {
			mx += string_to_lli ( line );
		}
	}
	total_mx = max ( total_mx, mx );
	printf ( "Ans is : %lld\n", total_mx );

	free ( line );
	fclose ( input );

	return 0;
}

