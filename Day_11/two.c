#define _GNU_SOURCE
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#define unused(x) (void) x

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

bool string_match ( char* line, size_t index, char* seg )
{
	size_t len = strlen ( seg );
	for ( size_t i = 0 ; i < len ; ++i )
		if ( line[i + index] != seg[i] )
			return false;
	return true;
}

uint64_t get_a_number ( char* line, size_t index )
{
	uint64_t ans = 0;
	while ( true ) {
		if ( line[index] < '0' || line[index] > '9' ) break;
		ans *= 10;
		ans += (uint64_t) ( line[index] - '0' );
		++index;
	}
	return ans;
}

uint64_t m_add ( uint64_t old, uint64_t val ) { return old + val; }
uint64_t m_mul ( uint64_t old, uint64_t val ) { return old * val; }
uint64_t m_sub ( uint64_t old, uint64_t val ) { return old - val; }
uint64_t m_div ( uint64_t old, uint64_t val ) { return old / val; }
typedef uint64_t (*operation) ( uint64_t old, uint64_t val );

#define MAX_ITEMS 205
typedef struct {
	uint64_t items[MAX_ITEMS];
	uint16_t items_count;
	bool val_is_old;
	uint64_t val;
	uint64_t test_val;
	uint16_t true_index;
	uint16_t false_index;
	operation ope;
	uint64_t play_count;
} monkey ;

#define MAX_MONKEYS 25
monkey monkeys[MAX_MONKEYS];
uint16_t monkeys_count = 0;
uint64_t plays[MAX_MONKEYS];

int maxima ( const void* a, const void* b )
{
	if ( *(uint64_t*)a > *(uint64_t*)b ) return -1;
	else if ( *(uint64_t*)a == *(uint64_t*)b ) return 0;
	else return 1;
}

uint64_t get_ans ( void )
{
	for ( uint16_t i = 0 ; i < monkeys_count ; ++i ) {
		plays[i] = monkeys[i].play_count;
	}
	qsort ( plays, monkeys_count, sizeof(uint64_t), maxima );
	return plays[0] * plays[1];
}

void simulate_one_round ( void )
{
	uint64_t divv = 1;
	for ( uint16_t i = 0 ; i < monkeys_count ; ++i ) {
		divv *= monkeys[i].test_val;
	}
	for ( uint16_t i = 0 ; i < monkeys_count ; ++i ) {
		for ( uint16_t j = 0 ; j < monkeys[i].items_count ; ++j ) {
			if ( monkeys[i].val_is_old ) monkeys[i].val = monkeys[i].items[j];
			uint64_t c_worry = monkeys[i].ope ( monkeys[i].items[j], monkeys[i].val );
			c_worry %= divv;
			//c_worry /= 3;
			if ( c_worry % monkeys[i].test_val != 0 ) {
				monkeys[monkeys[i].false_index].items[monkeys[monkeys[i].false_index].items_count++] = c_worry;
			} else {
			//	c_worry /= monkeys[i].test_val;
				monkeys[monkeys[i].true_index].items[monkeys[monkeys[i].true_index].items_count++] = c_worry;
			}
		}
		monkeys[i].play_count += monkeys[i].items_count;
		monkeys[i].items_count = 0;
	}
}

void read_input_and_populate_data ( const char* input_file )
{
	FILE* input = open_file_properly ( input_file );
	char* line = NULL; size_t len, slen;
	int16_t monkey_index = -1;
	char none[100];
	uint64_t cworry;
	while ( getline ( &line, &len, input ) != EOF ) {
		// get monkey number
		sscanf ( line, "%s %hd:\n", none, &monkey_index );
		assert ( monkey_index < MAX_MONKEYS );
		// get items
		if ( getline ( &line, &len, input ) == EOF ) assert ( false );
		slen = strlen ( line );
		cworry = 0;
		for ( size_t i = 18 ; i < slen ; ++i ) {
			switch ( line[i] ) {
				case ',':
					break;
				case ' ':
					monkeys[monkey_index].items[monkeys[monkey_index].items_count++] = cworry;
					cworry = 0;
					break;
				case '\n':
					monkeys[monkey_index].items[monkeys[monkey_index].items_count++] = cworry;
					cworry = 0;
					break;
				default:
					cworry *= 10;
					cworry += (uint64_t)( line[i] - '0' );
					break;
			}
		}
		if ( getline ( &line, &len, input ) == EOF ) assert ( false );
		assert ( string_match ( line, 13, "new = old " ) );
		uint64_t val = -1;
		monkeys[monkey_index].val_is_old = string_match ( line, 25, "old" );
		if ( !monkeys[monkey_index].val_is_old ) val = get_a_number ( line, 25 );
		switch ( line[23] ) {
			case '+':
				monkeys[monkey_index].ope = m_add;
				monkeys[monkey_index].val = val;
				break;
			case '-':
				monkeys[monkey_index].ope = m_sub;
				monkeys[monkey_index].val = val;
				break;
			case '*':
				monkeys[monkey_index].ope = m_mul;
				monkeys[monkey_index].val = val;
				break;
			case '/':
				monkeys[monkey_index].ope = m_div;
				monkeys[monkey_index].val = val;
				break;
			default:
				fprintf ( stderr, "Unknown operation\n" );
				break;
		}
		if ( getline ( &line, &len, input ) == EOF ) assert ( false );
		monkeys[monkey_index].test_val = get_a_number ( line, 21 );
		if ( getline ( &line, &len, input ) == EOF ) assert ( false );
		monkeys[monkey_index].true_index = get_a_number ( line, 29 );
		if ( getline ( &line, &len, input ) == EOF ) assert ( false );
		monkeys[monkey_index].false_index = get_a_number ( line, 30 );
		getline ( &line, &len, input );
		++monkeys_count;
	}
	free ( line );
	close_file_properly ( input_file, input );
}

void print_monkeys ( void )
{
	for ( size_t i = 0 ; i < monkeys_count ; ++i ) {
		printf ( "%d - ", monkeys[i].items_count );
		for ( int16_t j = 0 ; j < monkeys[i].items_count ; ++j ) {
			printf ( "%ld ", monkeys[i].items[j] );
		}
		puts ( "" );
		printf ( "Val : %ld\n", monkeys[i].val );
		printf ( "TVal : %ld\n", monkeys[i].test_val );
		printf ( "TI : %d\n", monkeys[i].true_index );
		printf ( "FI : %d\n", monkeys[i].false_index );
		printf ( "PC : %ld\n", monkeys[i].play_count );
		puts ( "" );
	}
}

int main ( int argc, char* argv[] )
{
	unused ( argc ); unused ( argv );
	const char* input_file = "sample.txt";
	// const char* input_file = "input.txt";
	read_input_and_populate_data ( input_file );
	for ( size_t i = 0 ; i < 10000 ; ++i ) {
		simulate_one_round ();
	}
	// print_monkeys ();
	uint64_t ans = get_ans ();
	printf ( "Ans is : %ld\n", ans );

	return 0;
}

