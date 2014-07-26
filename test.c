#include <stdio.h>
#include <time.h>
#include <string.h>

#include "sstring.h"

int main(void) {
	long i;
	long x = 0;
	char * str = NULL;
	char * str2 = NULL;
	SString s_str;
	SString s_str2;
	const char * test_strings[] = {
#include "lorem_ipsum.txt"
	};
	const int num_of_strings = sizeof(test_strings) / 8;
	const int size_of_strings[5] = { 8, 64, 1024, 8192, 65536 };


	for(i = 0; i < num_of_strings; i++) {
		str = malloc(size_of_strings[i] + 1);
		str2 = malloc(size_of_strings[i] + 1);
		strcpy(str, test_strings[i]);
		strncpy(str2, &(str[size_of_strings[i] / 2 - 1]), 3);

		x += (long)strchr(str, 'u');
		x += (long)strrchr(str, 'u');
		x += strspn(str, "ux");
		x += strcspn(str, "ux");
		x += (long)strpbrk(str, "ux");

		free(str);
		free(str2);


		s_str = new_sstring(test_strings[i], size_of_strings[i] + 1);
		s_str2 = new_sstring(NULL, size_of_strings[i] + 1);
		copy_n_sstring(&s_str, &s_str2, size_of_strings[i] / 2 - 1, 3);

		s_strchr(&s_str, 'u');
		s_strrchr(&s_str, 'u');
		s_strspn(&s_str, &SSTRING("ux"));
		s_strcspn(&s_str, &SSTRING("ux"));
		s_strpbrk(&s_str, &SSTRING("ux"));

		free(s_str.string);
		free(s_str2.string);


		s_str = new_sstring(test_strings[i], size_of_strings[i] + 1);
		s_str2 = new_sstring(NULL, size_of_strings[i] + 1);
		copy_n_sstring(&s_str, &s_str2, size_of_strings[i] / 2 - 1, 3);

		find_char_in_sstring(&s_str, 'u', 1);
		find_char_in_sstring(&s_str, 'u', 0);
		find_chars_in_sstring(&s_str, &SSTRING("ux"), 0);
		find_chars_in_sstring(&s_str, &SSTRING("ux"), 1);

		free(s_str.string);
		free(s_str2.string);
	}

	return 0;
}
