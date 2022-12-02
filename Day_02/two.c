#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define Unused(x) (void) x;

typedef enum {
	O_Rock = 'A',
	O_Paper = 'B',
	O_Scissors = 'C',
} Other ;

typedef enum {
	M_Rock = 'X',
	M_Paper = 'Y',
	M_Scissors = 'Z',
} Mine ;

long long int outcome ( char mine, char their )
{
	Mine m = mine; Other o = their;
	if ( (int)m - ( (int)o + (int)('X' - 'A') ) == 0 ) {
		return 3LL;
	}
	if ( m == M_Rock && o == O_Scissors ) return 6LL;
	if ( m == M_Paper && o == O_Rock ) return 6LL;
	if ( m == M_Scissors && o == O_Paper ) return 6LL;
	return 0LL;
}

Mine get_accurate_choice ( char other, char mine )
{
	Other o = other;
	char ch;
	switch ( mine ) {
		case 'X':
			ch = (char)((int)other - 1);
			if ( ch < 'A' ) ch = 'C';
			return (char)(ch + 'X' - 'A');
			break;
		case 'Y':
			return (char)(o + 'X' - 'A');
			break;
		case 'Z':
			ch = (char)((int)other + 1);
			if ( ch > 'C' ) ch = 'A';
			return (char)(ch + 'X' - 'A');
			break;
		default:
			fprintf ( stderr, "Unknown choice\n" );
			exit ( 1 );
			break;
	}
}

int main ( int argc, char* argv[] )
{
	Unused ( argc ); Unused ( argv );
	const char* input_file_name = "input.txt";
//	const char* input_file_name = "sample.txt";
	FILE* input = fopen ( input_file_name, "r" );
	if ( input == NULL ) {
		fprintf ( stderr, "Can't open file: %s\n", strerror ( errno ) );
		exit ( 1 );
	}

	char their, mine;
	long long int score = 0;
	while ( fscanf ( input, "%c %c ", &their, &mine ) != EOF ) {
		mine = get_accurate_choice ( their, mine );
		switch ( mine ) {
			case M_Rock:
				score += 1;
				break;
			case M_Paper:
				score += 2;
				break;
			case M_Scissors:
				score += 3;
				break;
			default:
				fprintf ( stderr, "Unknown choice found\n" );
				exit ( 1 );
				break;
		}
		score += outcome ( mine, their );
	}
	printf ( "Total Score is : %lld\n", score );

	fclose ( input );

	return 0;
}

