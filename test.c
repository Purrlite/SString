#include <stdio.h>
#include <time.h>
#include <string.h>

#include "sstring.h"

// NOTE: Works only on Linux because standard C implementation wouldn't be
//           accurate enough.

#define START_COUNTING  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start)
#define END_COUNTING    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end)

#define NUM_OF_LOOPS  10000

static unsigned long
time_difference(struct timespec end,
                struct timespec beginning)
{
	unsigned long long diff = (unsigned long long)end.tv_sec
	                          - (unsigned long long)beginning.tv_sec;
	return(end.tv_nsec - beginning.tv_nsec + ((diff > 0) ? 1000000UL : 0));
}


static unsigned long
average(unsigned long numbers[NUM_OF_LOOPS])
{
	unsigned long long sum = 0;
	int i;

	for(i = 0; i < NUM_OF_LOOPS; i++)
		sum += numbers[i];

	return sum / NUM_OF_LOOPS;
}


int main(void) {
	struct timespec start;
	struct timespec end;
	unsigned long time_diffs[NUM_OF_LOOPS];
	int i;
	int j;
	char * str = NULL;
	SString * s_str = NULL;
	SString * s_str2 = NULL;
	const char * test_strings[] = {
#include "lorem_ipsum.txt"
	};
	const int num_of_strings = sizeof(test_strings) / 8;
	const int size_of_strings[5] = { 8, 64, 1024, 8192, 65536 };


	// This for is to make sure the first few costly assigments don't affect
	//     the results
	for(i = 0; i < NUM_OF_LOOPS; i++) {
		str = test_strings[i % num_of_strings];
	}

	for(j = 0; j < num_of_strings; j++) {
		for(i = 0; i < NUM_OF_LOOPS; i++) {
			str = malloc(size_of_strings[j] * 2);
			strcpy(str, test_strings[j]);

			START_COUNTING;
			strcat(str, test_strings[j]);
			END_COUNTING;

			time_diffs[i] = time_difference(end, start);
			free(str);
		}
		printf("%5li ns - appending %i chars to a C string\n",
		       average(time_diffs), size_of_strings[j]);


		for(i = 0; i < NUM_OF_LOOPS; i++) {
			s_str = new_sstring(test_strings[j], size_of_strings[j] * 2);
			s_str2 = new_sstring(test_strings[j], size_of_strings[j]);

			START_COUNTING;
			append_sstring2(s_str, s_str2);
			END_COUNTING;

			time_diffs[i] = time_difference(end, start);
			free_sstring(s_str);
			free_sstring(s_str2);
		}
		printf("%5lu ns - appending %i chars to a SString v1\n",
		       average(time_diffs), size_of_strings[j]);

		for(i = 0; i < NUM_OF_LOOPS; i++) {
			s_str = new_sstring(test_strings[j], size_of_strings[j] * 2);
			s_str2 = new_sstring(test_strings[j], size_of_strings[j]);

			START_COUNTING;
			append_sstring2(s_str, s_str2);
			END_COUNTING;

			time_diffs[i] = time_difference(end, start);
			free_sstring(s_str);
			free_sstring(s_str2);
		}
		printf("%5lu ns - appending %i chars to a SStrings v2\n",
		       average(time_diffs), size_of_strings[j]);

		putchar('\n');
	}

	return 0;
}
