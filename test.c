#include <stdio.h>
#include <time.h>
#include <string.h>

#include "sstring.h"

int main(void) {
	long i;
	char * str = NULL;
	char * str2 = NULL;
	SString s_str;
	const char * test_strings[] = {
#include "lorem_ipsum.txt"
	};
	const int num_of_strings = sizeof(test_strings) / 8;
	const int size_of_strings[5] = { 8, 64, 1024, 8192, 65536 };


	for(i = 0; i < num_of_strings; i++) {
		str = malloc(size_of_strings[i] + 1);
		strcpy(str, test_strings[i]);

		strstr(str, "th");

		free(str);
		free(str2);


		s_str = new_sstring(test_strings[i], size_of_strings[i] + 1);

		find_str_in_sstring(&s_str, &SSTRING("th"), 0);

		free(s_str.string);
	}

	return 0;
}
