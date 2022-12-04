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
	union {
		long long int first;
		long long int F;
	};
	union {
		long long int second;
		long long int S;
	};
} pair ;

typedef struct {
	union {
		pair first;
		pair F;
	};
	union {
		pair second;
		pair S;
	};
} p_pair ;

#define MAX_PAIRS 10000
p_pair pairs[MAX_PAIRS];
size_t pairs_count = 0;

void read_inputs_from_file ( const char* input_file )
{
	FILE* input = fopen ( input_file, "r" );
	if ( input == NULL ) {
		fprintf ( stderr, "Failed to open %s cause %s\n",
				input_file, strerror ( errno ) );
		exit ( 1 );
	}
	while ( true ) {
		int res = fscanf ( input, "%lld-%lld,%lld-%lld\n",
				&pairs[pairs_count].F.F, &pairs[pairs_count].F.S,
				&pairs[pairs_count].S.F, &pairs[pairs_count].S.S );
		if ( res == EOF ) break;
		if ( pairs_count >= MAX_PAIRS ) {
			fprintf ( stderr, "Cant fit into pairs array\n" );
			exit ( 1 );
		}
		//printf ( "%lld %lld %lld %lld\n",
		//		pairs[pairs_count].F.F, pairs[pairs_count].F.S,
		//		pairs[pairs_count].S.F, pairs[pairs_count].S.S );
		++pairs_count;
	}
	int res = fclose ( input );
	if ( res != 0 ) {
		fprintf ( stderr, "Failed to close %s cause %s\n",
				input_file, strerror ( errno ) );
		exit ( 1 );
	}
}

bool inside ( pair a, pair b )
{
	return ( a.F >= b.F && a.S <= b.S );
}

long long int process_data ( void )
{
	long long int ans = 0;
	for ( size_t i = 0 ; i < pairs_count ; ++i ) {
		if ( inside ( pairs[i].F, pairs[i].S )
				|| inside ( pairs[i].S, pairs[i].F ) )
			++ ans;
	}
	return ans;
}

int main ( int argc, char* argv[] )
{
	unused ( argc ); unused ( argv );

//	const char* input_file_name = "input.txt";
	const char* input_file_name = "sample.txt";
	read_inputs_from_file ( input_file_name );
	long long int ans = process_data ();
	printf ( "Ans is : %lld\n", ans );

	return 0;
}

