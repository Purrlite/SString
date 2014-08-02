#include "sstring.h"

#include <string.h>
#include <stdbool.h>

#define CHECK_NULL(ret, str1, str2, additional)  \
	if(NULL == str1  ||  NULL == str2  additional)  \
		return ret;

#define CHECK_FREE_SPACE_IN_SSTRING(dest, len)  \
	if((len) > dest->size  ||  NULL == dest->string) {  \
		dest->size = (len);  \
\
		if(NULL != dest->string)  \
			free(dest->string);  \
\
		dest->string = malloc(dest->size);  \
		if(NULL == dest->string)  \
			return -2;  \
	}

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
	CHECK_NULL(-1, destination, source,  ||  NULL == source->string)
	CHECK_FREE_SPACE_IN_SSTRING(destination, source->length + 1)

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
	size_t length;
	char temp;

	CHECK_NULL(-1, destination, source,  ||  NULL == source->string
	            ||  start >= source->length)

	length = (start + num > source->length  ||  num == 0)
	         ? source->length - start : num;

	CHECK_FREE_SPACE_IN_SSTRING(destination, length + 1)

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
	size_t len;
	size_t lenght;

	CHECK_NULL(-1, destination, source, )

	len = strlen(source);
	lenght = (num > len  ||  0 == num) ? len - 1 : num;

	CHECK_FREE_SPACE_IN_SSTRING(destination, lenght + 1)

	strncpy(destination->string, source, lenght);
	destination->string[lenght] = '\0';

	destination->length = lenght;

	return 1;
}


int
append_sstring(SString * restrict destination,
               const SString * restrict source)
{
	CHECK_NULL(-1, destination, source,  ||  NULL == source->string)
	CHECK_FREE_SPACE_IN_SSTRING(destination, source->length + destination->length + 1)

	strcpy(&(destination->string[destination->length]), source->string);

	destination->length += source->length;

	return 1;
}


int
append_n_sstring(SString * restrict destination,
                 const SString * restrict source,
                 size_t start,
                 size_t num)
{
	size_t length;
	char temp;

	CHECK_NULL(-1, destination, source,  ||  NULL == source->string
	            ||  start >= source->length)

	length = (start + num > source->length  ||  num == 0)
	         ? source->length - start : num;

	CHECK_FREE_SPACE_IN_SSTRING(destination, length + destination->length + 1)

	temp = source->string[start + length];
	source->string[start + length] = '\0';

	strcpy(&(destination->string[destination->length]), &(source->string[start]));

	source->string[start + length] = temp;

	destination->length += length;

	return 1;
}


int
insert_sstring(SString * restrict destination,
               const SString * restrict source,
               size_t insert_start)
{
	SString temp;

	CHECK_NULL(-1, destination, source,  ||  NULL == source->string)
	CHECK_FREE_SPACE_IN_SSTRING(destination, destination->length + source->length + 1)

	temp = new_sstring(&(destination->string[insert_start]), 0);

	destination->string[insert_start] = '\0';
	destination->length = insert_start;

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
	size_t length;

	CHECK_NULL(-1, destination, source,  ||  NULL == source->string
	            ||  source_start >= source->length)

	length = (source_start + num > source->length  ||  num == 0)
	         ? source->length - source_start : num;

	CHECK_FREE_SPACE_IN_SSTRING(destination, destination->length + length + 1)

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
	CHECK_NULL(0, str1, str2,  ||  NULL == str1->string  ||  NULL == str2->string)

	return strcmp(str1->string, str2->string);
}


int
compare_n_sstrings(const SString * restrict str1,
                   const SString * restrict str2,
                   size_t num)
{
	int return_val;
	size_t length;
	char temp1;
	char temp2;

	CHECK_NULL(0, str1, str2,  ||  NULL == str1->string  ||  NULL == str2->string
	            ||  num == 0)

	length = (str1->length > str2->length) ? str2->length : str1->length;

	if(num >= length)
		return strcmp(str1->string, str2->string);
	else
		length = num;

	temp1 = str1->string[length];
	temp2 = str2->string[length];
	str1->string[length] = '\0';
	str2->string[length] = '\0';

	return_val = strcmp(str1->string, str2->string);

	str1->string[length] = temp1;
	str2->string[length] = temp2;

	return return_val;
}


int
find_char_in_sstring(const SString * str,
                     char character,
                     unsigned int place)
{
	register unsigned int i;
	char * found_char;

	CHECK_NULL(-1, str, str->string, )

	if(place == 0) {
		found_char = strrchr(str->string, (int)character);

		if(found_char == NULL)
			return -2;
	} else {
		found_char = str->string;

		for(i = 0; i < place; i++) {
			found_char = strchr(&(str->string[str->string - found_char]),
			                    (int)character);
			if(found_char == NULL)
				return -2;
		}
	}

	return (int)(str->string - found_char);
}


int
find_chars_in_sstring(const SString * str,
                      const SString * chars,
                      bool inverse)
{
	CHECK_NULL(-1, str, chars,  ||  NULL == str->string  ||  NULL == chars->string)

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

	CHECK_NULL(-1, str, sub_str,  ||  NULL == str->string  ||  NULL == sub_str->string
	            ||  start >= str->length)

	return_val = strstr(&(str->string[start]), sub_str->string);

	if(return_val == NULL)
		return -2;
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

	CHECK_NULL(NULL, str, separator,  ||  NULL == str->string
	            ||  NULL == separator->string)

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

#undef CHECK_NULL
#undef CHECK_FREE_SPACE_IN_SSTRING
