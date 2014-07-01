#include <stdio.h>
#include <time.h>
#include <string.h>

#include "s_string.h"

// NOTE: Works only on Linux because standard C implementation wouldn't be
//           accurate enough.

#define START_COUNTING  clock_gettime(CLOCK_MONOTONIC, &start)
#define END_COUNTING    clock_gettime(CLOCK_MONOTONIC, &end)
#define TIME_DIFFERENCE (end.tv_nsec - start.tv_nsec)

#define NUM_OF_LOOPS  1024

long long
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
	s_string * s_str = NULL;
	const char * test_strings[] = {
#include "lorem_ipsum.txt"
	};
	const int num_of_strings = sizeof(test_strings) / 8;
	const int size_of_strings[5] = { 8, 64, 1024, 8192, 65536 };


	for(i = 0; i < NUM_OF_LOOPS; i++) {
		START_COUNTING;
		str = "Testing";
		END_COUNTING;
		time_diffs[i] = TIME_DIFFERENCE;
	}
	printf("%lli ns - assigning 8 chars to a string\n", average(time_diffs));

	for(i = 0; i < NUM_OF_LOOPS; i++) {
		s_str = malloc(8);
		START_COUNTING;
		*s_str = S_STRING("Testing");
		END_COUNTING;
		time_diffs[i] = TIME_DIFFERENCE;
		free(s_str);
	}
	printf("%lli ns - assigning 8 chars to a s_string\n\n", average(time_diffs));

	for(j = 0; j < num_of_strings; j++) {
		for(i = 0; i < NUM_OF_LOOPS; i++) {
			START_COUNTING;
			str = malloc(size_of_strings[j]);
			strcpy(str, test_strings[j]);
			END_COUNTING;
			time_diffs[i] = TIME_DIFFERENCE;
			free(str);
		}
		printf("%lli ns - copying %i chars into a string\n",
		       average(time_diffs), size_of_strings[j]);

		for(i = 0; i < NUM_OF_LOOPS; i++) {
			START_COUNTING;
			s_str = s_init(test_strings[j], size_of_strings[j]);
			END_COUNTING;
			time_diffs[i] = TIME_DIFFERENCE;
			s_free(s_str);
		}
		printf("%lli ns - copying %i chars into a s_string\n",
		       average(time_diffs), size_of_strings[j]);

		for(i = 0; i < NUM_OF_LOOPS; i++) {
			START_COUNTING;
			s_str = s_init2(test_strings[j], size_of_strings[j]);
			END_COUNTING;
			time_diffs[i] = TIME_DIFFERENCE;
			s_free(s_str);
		}
		printf("%lli ns - copying %i chars into a s_string 2\n\n",
		       average(time_diffs), size_of_strings[j]);
	}

	return 0;
}
