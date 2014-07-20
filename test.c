#include <stdio.h>
#include <time.h>
#include <string.h>

#include "sstring.h"

int main(void) {
	long i;
	char * str = NULL;
	SString s_str;
	SString s_str2;
	const char * test_strings[] = {
#include "lorem_ipsum.txt"
	};
	const int num_of_strings = sizeof(test_strings) / 8;
	const int size_of_strings[5] = { 8, 64, 1024, 8192, 65536 };


	for(i = 0; i < num_of_strings; i++) {
		str = malloc(size_of_strings[i] + 1);

		strncpy(str, test_strings[i], size_of_strings[i]);

		free(str);


		s_str = new_sstring(NULL, size_of_strings[i] + 1);
		s_str2 = new_sstring(test_strings[i], 0);

		copy_n_sstring(&s_str, &s_str2, 0, size_of_strings[i]);

		free(s_str.string);
		free(s_str2.string);


		s_str = new_sstring(NULL, size_of_strings[i] + 1);
		s_str2 = new_sstring(test_strings[i], 0);

		copy_n_sstring2(&s_str, &s_str2, 0, size_of_strings[i]);

		free(s_str.string);
		free(s_str2.string);
	}

	return 0;
}
