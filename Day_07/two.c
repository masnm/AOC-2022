#define _GNU_SOURCE
#include <stdint.h>
#include <assert.h>
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

#define MAX_STR_LEN 100
typedef struct {
	uint16_t size;
	char data[MAX_STR_LEN];
} string ;

#define MAX_STRINGS 1010
string strings[MAX_STRINGS];
uint16_t strings_count = 0;

typedef struct {
	uint64_t size;
	char data[MAX_STR_LEN];
} item ;
#define MAX_STACK_SIZE 1010
item stack[MAX_STACK_SIZE];
int16_t stack_top = -1;

typedef struct {
	uint64_t size;
	char data[MAX_STR_LEN];
} dir ;
#define MAX_SIZES 1010
dir sizes[MAX_SIZES];
uint16_t sizes_count = 0;

uint64_t total_disk = 70000000, need_disk = 30000000;
uint64_t used_disk = 0;


void read_input_from_file ( const char* input_file )
{
	FILE* input = open_file_for_read ( input_file );
	char* line = NULL; size_t len;
	while ( getline ( &line, &len, input ) != -1 ) {
		strings[strings_count].size = strlen ( line );
		strcpy ( strings[strings_count].data, line );
		++strings_count;
	}
	free ( line );
	close_file_properly ( input_file, input );
}

bool is_file ( uint16_t index )
{
	uint64_t size; char name[MAX_STR_LEN];
	return sscanf ( strings[index].data, "%zu %s\n", &size, name ) != 0;
}

void calculate_this_size ( uint16_t index )
{
	uint16_t next_cmd = 0;
	for ( uint16_t i = index + 1 ; i < strings_count ; ++i )
		if ( strings[i].data[0] == '$' ) {
			next_cmd = i;
			break;
		}
	if ( next_cmd == 0 ) next_cmd = strings_count;
	uint64_t ans = 0, size;
	for ( uint16_t i = index ; i < next_cmd ; ++i ) {
		if ( is_file ( i ) ) {
			sscanf ( strings[i].data, "%zu", &size );
			ans += size;
		}
	}
	used_disk += ans;
	stack[stack_top].size += ans;
}

void process_data ( void )
{
	const char *ls_cmd_str = "$ ls\n", *cd_up_dir = "$ cd ..\n";
	for ( uint16_t i = 0 ; i < strings_count ; ++i ) {
		if ( strcmp ( strings[i].data, cd_up_dir ) == 0 ) {
			sizes[sizes_count].size = stack[stack_top].size;
			strcpy ( sizes[sizes_count].data, stack[stack_top].data );
			stack[stack_top-1].size += stack[stack_top].size;
			-- stack_top; ++sizes_count;
		} else if ( strncmp ( strings[i].data, cd_up_dir, 4 ) == 0 ) {
			++ stack_top;
			stack[stack_top].size = 0;
			strcpy ( stack[stack_top].data, strings[i].data + 5 );
		} else if ( strcmp ( strings[i].data, ls_cmd_str ) == 0 ) {
			calculate_this_size ( i );
		}
	}
	while ( stack_top >= 0 ) {
		sizes[sizes_count].size = stack[stack_top].size;
		strcpy ( sizes[sizes_count].data, stack[stack_top].data );
		if ( stack_top > 0 ) stack[stack_top-1].size += stack[stack_top].size;
		-- stack_top; ++sizes_count;
	}
}

int less ( const void* a, const void* b )
{
	uint64_t l, r;
	l = (*(dir*)a).size;
	r = (*(dir*)b).size;
	if ( l < r ) return -1;
	else if ( l == r ) return 0;
	else return 1;
}

uint64_t find_ans ()
{
	uint64_t cindex = ULLONG_MAX, need;
	uint64_t free_disk = total_disk - used_disk;
	need = need_disk - free_disk;
	for ( uint16_t i = 0 ; i < sizes_count ; ++i ) {
		if ( sizes[i].size > need ) {
			if ( cindex == ULLONG_MAX ) cindex = i;
			else if ( sizes[cindex].size >= sizes[i].size ) cindex = i;
		}
	}
	return cindex;
}

int main ( int argc, char* argv[] )
{
	unused ( argc ); unused ( argv );

	const char* input_file = "sample.txt";
//	const char* input_file = "input.txt";
	read_input_from_file ( input_file );
	process_data ();
	uint64_t ans = find_ans ();
	printf ( "Ans is : %zu for %s", sizes[ans].size, sizes[ans].data );

	return 0;
}

