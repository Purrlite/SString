#include <stdio.h>
#include <time.h>
#include <string.h>

#include "sstring.h"

// NOTE: Works only on Linux because standard C implementation wouldn't be
//           accurate enough.

#define START_COUNTING  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start)
#define END_COUNTING    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end)

#define NUM_OF_LOOPS  10000

long
time_difference(struct timespec end,
                struct timespec beginning)
{
	long long diff = (long long)end.tv_sec - (long long)beginning.tv_sec;
	return(end.tv_nsec - beginning.tv_nsec + ((diff > 0) ? 1000000L : 0));
}


long
average(long numbers[NUM_OF_LOOPS])
{
	long long sum = 0;
	int i;

	for(i = 0; i < NUM_OF_LOOPS; i++)
		sum += numbers[i];

	return sum / NUM_OF_LOOPS;
}


int main(void) {
	struct timespec start;
	struct timespec end;
	long time_diffs[NUM_OF_LOOPS];
	int i;
	int j;
	char * str = NULL;
	SString * s_str = NULL;
	const char * test_strings[] = {
#include "lorem_ipsum.txt"
	};
	const int num_of_strings = sizeof(test_strings) / 8;
	const int size_of_strings[5] = { 8, 64, 1024, 8192, 65536 };


	clock_getres(CLOCK_PROCESS_CPUTIME_ID, &start);
	printf("Precision of clock is %li ns.\n", start.tv_nsec);

	// This for is to make sure the first few costly assigments don't affect
	//     the results
	for(i = 0; i < NUM_OF_LOOPS; i++) {
		str = "T";
	}

	for(j = 0; j < num_of_strings; j++) {/*
		for(i = 0; i < NUM_OF_LOOPS; i++) {
			START_COUNTING;
			str = malloc(size_of_strings[j]);
			strcpy(str, test_strings[j]);
			END_COUNTING;
			time_diffs[i] = time_difference(end, start);
			free(str);
		}
		printf("%4li ns - copying %i chars into a C string\n",
		       average(time_diffs), size_of_strings[j]);
*/

		for(i = 0; i < NUM_OF_LOOPS; i++) {
			s_str = new_sstring(test_strings[j], size_of_strings[j]);
			START_COUNTING;
			empty_sstring(s_str);
			END_COUNTING;
			time_diffs[i] = time_difference(end, start);
			free_sstring(s_str);
		}
		printf("%4li ns - emptying %s chars from SString v1\n",
		       average(time_diffs), size_of_strings[j]);

		for(i = 0; i < NUM_OF_LOOPS; i++) {
			s_str = new_sstring(test_strings[j], size_of_strings[j]);
			START_COUNTING;
			empty_sstring2(s_str);
			END_COUNTING;
			time_diffs[i] = time_difference(end, start);
			free_sstring(s_str);
		}
		printf("%4li ns - emptying %s chars from SString v2\n",
		       average(time_diffs), size_of_strings[j]);
	}

	return 0;
}
