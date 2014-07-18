#include <stdio.h>
#include <time.h>
#include <string.h>

#include "sstring.h"

int main(void) {
	long i;
	long j;
	char * str = NULL;
	SString * s_str = NULL;
	SString * s_str2 = NULL;
	const char * test_strings[] = {
#include "lorem_ipsum.txt"
	};
	const int num_of_strings = sizeof(test_strings) / 8;
	const int size_of_strings[5] = { 8, 64, 1024, 8192, 65536 };


	// This for is to make sure the first few costly assigments don't affect
	//     the speeds of others
	for(i = 0; i < 100000; i++) {
		str = malloc(size_of_strings[0] * 2 + 1);
		strcpy(str, test_strings[0]);
		strcat(str, test_strings[0]);
		free(str);
	}

	for(j = 0; j < num_of_strings; j++) {
		str = malloc(size_of_strings[j] * 2 + 1);
		strcpy(str, test_strings[j]);

		strncat(str, test_strings[j], size_of_strings[j]);

		free(str);


		s_str = new_sstring(test_strings[j], size_of_strings[j] * 2 + 1);
		s_str2 = new_sstring(test_strings[j], size_of_strings[j]);

		append_n_sstring(s_str, s_str2, size_of_strings[j]);

		free_sstring(s_str);
		free_sstring(s_str2);


		s_str = new_sstring(test_strings[j], size_of_strings[j] * 2 + 1);
		s_str2 = new_sstring(test_strings[j], size_of_strings[j]);

		append_n_sstring2(s_str, s_str2, size_of_strings[j]);

		free_sstring(s_str);
		free_sstring(s_str2);


		s_str = new_sstring(test_strings[j], size_of_strings[j] * 2 + 1);
		s_str2 = new_sstring(test_strings[j], size_of_strings[j]);

		append_n_sstring3(s_str, s_str2, size_of_strings[j]);

		free_sstring(s_str);
		free_sstring(s_str2);


		s_str = new_sstring(test_strings[j], size_of_strings[j] * 2 + 1);
		s_str2 = new_sstring(test_strings[j], size_of_strings[j]);

		append_n_sstring4(s_str, s_str2, size_of_strings[j]);

		free_sstring(s_str);
		free_sstring(s_str2);
	}

	return 0;
}
