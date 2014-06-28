#include <stdio.h>
#include <time.h>

#include "s_string.h"

// NOTE: Works only on Linux because standard C implementation wouldn't be
//           accurate enough.

#define START_COUNTING  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start)
#define END_COUNTING    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end)
#define PRINT_TIME_DIFFERENCE( message )  \
    printf("%lis " message, end.tv_nsec - start.tv_nsec);

int main(void) {
	struct timespec start;
	struct timespec end;
	int i;
	char * str;
	s_string s_str;
	const char * test_strings[] = {
		"Testing",
		"Testing a test.",
		"Lorem ipsum dolor sit volutpat.",
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit posuere.",
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam faucibus libero mi, vitae ornare justo tristique eget turpis duis.",
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris et scelerisque ipsum. Curabitur non ipsum auctor, porttitor quam quis, cursus lacus. Fusce suscipit porta arcu vel mollis. Ut commodo arcu at odio volutpat tempus. Nam scelerisque id orci id."
	};
	const int num_of_strings = 6;

	START_COUNTING;
	str = "Testing";
	END_COUNTING;
	PRINT_TIME_DIFFERENCE("- assigning 8 chars to a string.\n");

	START_COUNTING;
	s_str = S_STRING("Testing");
	END_COUNTING;
	PRINT_TIME_DIFFERENCE("- assigning 8 chars to a s_string.\n");


	return 0;
}
