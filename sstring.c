#include "sstring.h"

#include <string.h>
#include <stdbool.h>

typedef SString (new_SStr_func)(const char * string,
                                size_t size);


static SString
NULL_array_0_size(const char * string,
                  size_t size)
{
	return (SString){ 0, 0, NULL };
}


static SString
NULL_array_non0_size(const char * string,
                     size_t size)
{
	SString s_str = (SString){ 0, size, NULL };

	s_str.string = malloc(s_str.size);
	if(NULL == s_str.string)
		return (SString){0};

	s_str.string[0] = '\0';

	return s_str;
}


static SString
nonNULL_array_0_size(const char * string,
                     size_t size)
{
	size_t length = strlen(string);
	SString s_str = (SString){ length, length + 1, NULL };

	s_str.string = malloc(s_str.size);
	if(NULL == s_str.string)
		return (SString){0};

	strcpy(s_str.string, string);

	return s_str;
}


static SString
nonNULL_array_non0_size(const char * string,
                        size_t size)
{
	size_t length = strlen(string);
	SString s_str = (SString){
		.size = size,
		.length = (length > size) ? size - 1 : length
	};

	s_str.string = malloc(s_str.size);
	if(NULL == s_str.string)
		return (SString){0};

	strncpy(s_str.string, string, s_str.length);
	s_str.string[s_str.length] = '\0';

	return s_str;
}


static new_SStr_func *
get_new_SS_func(const char * string,
                size_t size)
{
	if(NULL == string)
		switch(size) {
		case 0:
			return NULL_array_0_size;

		default:
			return NULL_array_non0_size;
		}
	else
		switch(size) {
		case 0:
			return nonNULL_array_0_size;

		default:
			return nonNULL_array_non0_size;
		}
}


SString
new_sstring(const char * string,
            size_t size)
{
	new_SStr_func * new_SS_func = get_new_SS_func(string, size);

	return((new_SS_func)(string, size));
}


inline void
empty_sstring(SString * str)
{
	if(str == NULL)
		return ;

	str->string = NULL;
	str->length = 0;
	str->size = 0;
}


inline void
free_sstring(SString * str)
{
	if(str == NULL)
		return ;

	if(str->string != NULL)
		free(str->string);

	free(str);
}


inline void
free_sstrings(struct SStrings * strs)
{
	size_t i;

	if(strs == NULL)
		return ;

	for(i = 0; i < strs->length; i++)
		if(strs->sstrings[i].string != NULL)
			free(strs->sstrings[i].string);

	free(strs);
}


int
copy_sstring(SString * restrict destination,
             const SString * restrict source)
{
	if(NULL == destination  ||  NULL == source  ||  NULL == source->string)
		return -1;

	if(source->length >= destination->size  ||  NULL == destination->string) {
		destination->size = source->length + 1;

		if(NULL != destination->string)
			free(destination->string);

		destination->string = malloc(destination->size);
		if(NULL == destination->string)
			return -2;
	}

	strcpy(destination->string, source->string);

	destination->length = source->length;

	return 1;
}


int
copy_n_sstring(SString * restrict destination,
               const SString * restrict source,
               size_t start,
               size_t num)
{
	size_t length = (start + num > source->length) ? source->length - start : num;
	char temp;

	if(NULL == destination  ||  NULL == source  ||  NULL == source->string
		    ||  0 == num  ||  start >= source->length)
		return -1;

	if(length > destination->size  ||  NULL == destination->string) {
		destination->size = length + 1;

		if(NULL != destination->string)
			free(destination->string);

		destination->string = malloc(destination->size);
		if(NULL == destination->string)
			return -2;
	}

	temp = source->string[start + length];
	source->string[start + length] = '\0';

	strcpy(destination->string, &(source->string[start]));

	source->string[start + length] = temp;

	destination->length = length;

	return 1;
}


int
copy_string_to_sstring(SString * restrict destination,
                       const char * restrict source,
                       size_t num)
{
	size_t len = strlen(source);
	size_t lenght = (num > len  ||  0 == num) ? len - 1 : num;

	if(NULL == destination  ||  NULL == source)
		return -1;

	if(lenght > destination->size  ||  NULL == destination->string) {
		destination->size = lenght + 1;

		if(NULL != destination->string)
			free(destination->string);

		destination->string = malloc(destination->size);
		if(NULL == destination->string)
			return -2;
	}

	strncpy(destination->string, source, lenght);
	destination->string[lenght] = '\0';

	destination->length = lenght;

	return 1;
}


int
append_sstring(SString * restrict destination,
               const SString * restrict source)
{
	if(NULL == destination  ||  NULL == source  ||  NULL == source->string)
		return -1;

	if(source->length + destination->length > destination->size
		    ||  NULL == destination->string) {
		destination->size = source->length + destination->length + 1;

		if(NULL != destination->string)
			free(destination->string);

		destination->string = malloc(destination->size);
		if(NULL == destination->string)
			return -2;
	}

	strcpy(&(destination->string[destination->length]), source->string);

	destination->length += source->length;

	destination->string[destination->length] = '\0';

	return 1;
}


int
append_n_sstring(SString * restrict destination,
                 const SString * restrict source,
                 size_t start,
                 size_t num)
{
	size_t length = (start + num > source->length) ? source->length - start : num;
	char temp;

	if(NULL == destination  ||  NULL == source  ||  NULL == source->string)
		return -1;

	if(length + destination->length > destination->size
		    ||  NULL == destination->string) {
		destination->size = length + destination->length + 1;

		if(NULL != destination->string)
			free(destination->string);

		destination->string = malloc(destination->size);
		if(NULL == destination->string)
			return -2;
	}

	temp = source->string[start + length];
	source->string[start + length] = '\0';

	strcpy(&(destination->string[destination->length]), &(source->string[start]));

	source->string[start + length] = temp;

	destination->length += length;

	destination->string[destination->length] = '\0';

	return 1;
}


int
insert_sstring(SString * restrict destination,
               const SString * restrict source,
               size_t start)
{
	SString temp;

	if(NULL == destination  ||  NULL == source  ||  NULL == source->string)
		return -1;

	if(destination->length + source->length > destination->size
		    ||  NULL == source->string) {
		destination->size = destination->length + source->length + 1;

		if(NULL != destination->string)
			free(destination->string);

		destination->string = malloc(destination->size);
		if(NULL == destination->string)
			return -2;
	}

	temp = new_sstring(&(destination->string[start]), 0);

	destination->string[start] = '\0';
	destination->length = start;

	append_sstring(destination, source);
	append_sstring(destination, &temp);

	free(temp.string);

	return 1;
}


int
insert_n_sstring(SString * restrict destination,
                 const SString * restrict source,
                 size_t insert_start,
                 size_t source_start,
                 size_t num)
{
	SString temp;
	size_t length = (source_start + num > source->length)
	                ? source->length - source_start : num;

	if(NULL == destination  ||  NULL == source  ||  NULL == source->string)
		return -1;

	if(destination->length + length > destination->size
		    ||  NULL == source->string) {
		destination->size = destination->length + length + 1;

		if(NULL != destination->string)
			free(destination->string);

		destination->string = malloc(destination->size);
		if(NULL == destination->string)
			return -2;
	}

	temp = new_sstring(&(destination->string[insert_start]), 0);

	destination->string[insert_start] = '\0';
	destination->length = insert_start;

	append_n_sstring(destination, source, source_start, length);
	append_sstring(destination, &temp);

	free(temp.string);

	return 1;
}


int
compare_sstrings(const SString * restrict str1,
                 const SString * restrict str2)
{
	return strcmp(str1->string, str2->string);
}


int
compare_n_sstrings(const SString * restrict str1,
                   const SString * restrict str2,
                   size_t num)
{
	int return_val;
	size_t length = (str1->length > str2->length) ? str2->length : str1->length;
	char temp;

	if(num >= length)
		return strcmp(str1->string, str2->string);
	else
		length = num;

	temp = str1->string[length];
	str1->string[length] = '\0';

	return_val = strcmp(str1->string, str2->string);

	str1->string[length] = temp;

	return return_val;
}


int
find_char_in_sstring(const SString * str,
                     char character,
                     unsigned int place)
{
	register unsigned int i;
	char * found_char;

	if(place == 0) {
		found_char = strrchr(str->string, (int)character);

		if(found_char == NULL)
			return -1;
	} else {
		found_char = str->string;

		for(i = 0; i < place; i++) {
			found_char = strchr(&(str->string[str->string - found_char]),
			                    (int)character);
			if(found_char == NULL)
				return -1;
		}
	}

	return (int)(str->string - found_char);
}


int
find_chars_in_sstring(const SString * str,
                      const SString * chars,
                      bool inverse)
{
	if(inverse == false)
		return strspn(str->string, chars->string);
	else
		return strcspn(str->string, chars->string);
}


int
find_str_in_sstring(const SString * str,
                    const SString * sub_str,
                    size_t start)
{
	char * return_val;

	if(start >= str->length)
		return -1;

	return_val = strstr(&(str->string[start]), sub_str->string);

	if(return_val == NULL)
		return -1;
	else
		return (int)(str->string - return_val);
}


struct SStrings *
split_sstring(const SString * str,
              const SString * separator)
{
	struct SStrings * split;
	int * locations;  // locations of separators
	int allocated_num = 8;  // allocated number of locations
	int num_of_locations = 0;
	unsigned int i;

	locations = malloc(sizeof(int) * allocated_num);

	locations[num_of_locations] = find_str_in_sstring(str, separator, 0);
	if(locations[num_of_locations] == -1)
		return NULL;

	num_of_locations++;

	for(; locations[num_of_locations - 1] != -1; num_of_locations++) {
		if(num_of_locations >= allocated_num) {
			allocated_num *= 2;
			locations = realloc(locations, sizeof(int) * allocated_num);
		}

		locations[num_of_locations] = find_str_in_sstring(str, separator,
			    locations[num_of_locations - 1] + separator->length);
	}

	split = malloc(sizeof(struct SStrings) + (num_of_locations + 1) * sizeof(SString));

	split->length = num_of_locations + 1;

	split->sstrings[0] = (SString){0};
	copy_n_sstring(&(split->sstrings[0]), str, 0, locations[0]);

	for(i = 1; i < split->length; i++) {
		split->sstrings[i] = (SString){0};
		copy_n_sstring(&(split->sstrings[i]), str, locations[i - 1], locations[i]);
	}

	free(locations);

	return split;
}
