#include "sstring.h"

#include <string.h>
#include <stdbool.h>
#include <ctype.h>

static void
ensure_necessary_size(SString * str,
                      size_t size,
                      bool to_realloc)
{
	if(str->size > size)
		return ;

	str->size = size;

	if(to_realloc == true) {
		str->string = realloc(str->string, str->size);
	} else {
		if(str->string != NULL)
			free(str->string);

		str->string = malloc(str->size);
	}
}


typedef SString (new_SString_func)(const char * string,
                                   size_t size);


static SString
NULL_string_0_size(const char * string,
                   size_t size)
{
	return (SString){ 0, 0, NULL };
}

static SString
NULL_string_non0_size(const char * string,
                      size_t size)
{
	SString str = (SString){ 0, size, NULL };

	str.string = malloc(str.size);
	if(NULL == str.string)
		return (SString){0};

	str.string[0] = '\0';

	return str;
}

static SString
nonNULL_string_0_size(const char * string,
                      size_t size)
{
	size_t length = strlen(string);
	SString str = (SString){ length, length + 1, NULL };

	str.string = malloc(str.size);
	if(NULL == str.string)
		return (SString){0};

	strcpy(str.string, string);

	return str;
}

static SString
nonNULL_string_non0_size(const char * string,
                         size_t size)
{
	size_t length = strlen(string);
	SString str = (SString){
		.size = size,
		.length = (length > size) ? size - 1 : length
	};

	str.string = malloc(str.size);
	if(NULL == str.string)
		return (SString){0};

	strncpy(str.string, string, str.length);
	str.string[str.length] = '\0';

	return str;
}

static new_SString_func *
get_new_SString_func(const char * string,
                     size_t size)
{
	if(NULL == string)
		switch(size) {
		case 0:
			return NULL_string_0_size;

		default:
			return NULL_string_non0_size;
		}
	else
		switch(size) {
		case 0:
			return nonNULL_string_0_size;

		default:
			return nonNULL_string_non0_size;
		}
}

SString
new_sstring(const char * string,
            size_t size)
{
	new_SString_func * new_func = get_new_SString_func(string, size);
	return (new_func)(string, size);
}


inline void
free_sstring(SString * str)
{
	if(str == NULL)
		return ;

	if(str->string != NULL)
		free(str->string);

	str->string = NULL;
	str->length = 0;
	str->size = 0;
}


inline void
free_sstrings(struct SStrings ** strs)
{
	size_t i;

	if(strs == NULL  ||  *strs == NULL)
		return ;

	for(i = 0; i < (*strs)->length; i++)
		if((*strs)->sstrings[i].string != NULL)
			free((*strs)->sstrings[i].string);

	free(*strs);

	*strs = NULL;
}


inline SString
connect_sstrings(const struct SStrings * strs,
                 const SString * connector)
{
	size_t i;
	SString str = (SString){0};

	if(strs == NULL  ||  strs->length == 0)
		return (SString){0};

	for(i = 0; i < strs->length - 1; i++) {
		append_sstring(&str, &(strs->sstrings[i]));
		append_sstring(&str, connector);
	}
	append_sstring(&str, &(strs->sstrings[i]));

	return str;
}


static SString
to_X_sstring(const SString * str,
             bool to_lower)
{
	long location = 0;
	long previous_location = 0;
	SString new_str = new_sstring(NULL,str->length + 1);
	SString Xcase = (to_lower == true) ? new_sstring("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0)
	                : new_sstring("abcdefghijklmnopqrstuvwxyz", 0);
	int (* to_X_case)(int) = (to_lower == true) ? tolower : toupper;

	for(;;) {
		location = find_chars_in_sstring(str, &Xcase, location, 0);

		if(location == -1  ||  -2 == location)
			break;

		if(location != previous_location)
			append_n_sstring(&new_str, str, previous_location,
			                 location - previous_location);

		new_str.string[location] = (to_X_case)(str->string[location]);
		new_str.length++;

		location++;
		previous_location = location;
	}

	append_n_sstring(&new_str, str, previous_location, 0);

	return new_str;
}


SString
to_lower_sstring(const SString * str)
{
	if(str == NULL  ||  str->string == NULL)
		return (SString){0};

	return to_X_sstring(str, true);
}


SString
to_upper_sstring(const SString * str)
{
	if(str == NULL  ||  str->string == NULL)
		return (SString){0};

	return to_X_sstring(str, false);
}


int
copy_sstring(SString * restrict destination,
             const SString * restrict source)
{
	return copy_n_sstring(destination, source, 0, 0);
}


int
copy_n_sstring(SString * restrict destination,
               const SString * restrict source,
               size_t start,
               size_t num)
{
	size_t length;

	if(destination == NULL  ||  source == NULL  ||  NULL == source->string
		    ||  start > source->length)
		return -1;

	length = (start + num > source->length  ||  num == 0)
	         ? source->length - start : num;

	ensure_necessary_size(destination, length + 1, false);
	if(destination == NULL)
		return -2;

	strncpy(destination->string, &(source->string[start]), length);
	destination->string[length] = '\0';

	destination->length = length;

	return 1;
}


int
copy_str_to_sstring(SString * restrict destination,
                    const char * restrict source,
                    size_t num)
{
	size_t len;

	if(source == NULL)
		return -1;

	len = strlen(source);

	return copy_n_sstring(destination, &(SString){ len, len + 1, source }, 0, num);
}


int
append_sstring(SString * restrict destination,
               const SString * restrict source)
{
	return append_n_sstring(destination, source, 0, 0);
}


int
append_n_sstring(SString * restrict destination,
                 const SString * restrict source,
                 size_t start,
                 size_t num)
{
	size_t length;

	if(destination == NULL  ||  source == NULL  ||  NULL == source->string
		    ||  start >= source->length)
		return -1;

	length = (start + num > source->length  ||  num == 0)
	         ? source->length - start : num;

	ensure_necessary_size(destination, length + destination->length + 1, true);
	if(destination == NULL)
		return -2;

	strncpy(&(destination->string[destination->length]), &(source->string[start]),
	        length);

	destination->length += length;

	destination->string[destination->length] = '\0';

	return 1;
}


int
append_str_to_sstring(SString * restrict destination,
                      const char * restrict source,
                      size_t num)
{
	size_t len;

	if(source == NULL)
		return -1;

	len = strlen(source);

	return append_n_sstring(destination, &(SString){ len, len + 1, source }, 0, num);
}


int
insert_sstring(SString * restrict destination,
               const SString * restrict source,
               size_t insert_start)
{
	return insert_n_sstring(destination, source, insert_start, 0, 0);
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

	if(destination == NULL  ||  source == NULL  ||  NULL == source->string
		    ||  source_start >= source->length
		    ||  insert_start > destination->length)
		return -1;

	length = (source_start + num > source->length  ||  num == 0)
	         ? source->length - source_start : num;

	ensure_necessary_size(destination, destination->length + length + 1, true);
	if(destination == NULL)
		return -2;

	temp = new_sstring(&(destination->string[insert_start]), 0);

	strncpy(&(destination->string[insert_start]), &(source->string[source_start]),
	        length);
	strcpy(&(destination->string[insert_start + length]), temp.string);

	destination->length += length;

	free(temp.string);

	return 1;
}


int
insert_str_to_sstring(SString * restrict destination,
                      const char * restrict source,
                      size_t insert_start,
                      size_t num)
{
	size_t len;

	if(source == NULL)
		return -1;

	len = strlen(source);

	return insert_n_sstring(destination, &(SString){ len, len + 1, source },
	                        insert_start, 0, num);
}


int
compare_sstrings(const SString * restrict str1,
                 const SString * restrict str2)
{
	if(str1 == NULL  ||  str2 == NULL  ||  NULL == str1->string
		    ||  NULL == str2->string)
		return 0;

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

	if(str1 == NULL  ||  str2 == NULL  ||  NULL == str1->string
		    ||  NULL == str2->string  ||  num == 0)
		return 0;

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


long
find_chars_in_sstring(const SString * str,
                      const SString * chars,
                      size_t start,
                      bool inverse)
{
	size_t ret_value;
	char * found_char;

	if(str == NULL  ||  chars == NULL  ||  NULL == str->string
		    ||  NULL == chars->string  ||  start >= str->length)
		return -1;

	if(inverse == false)
		ret_value = strcspn(&(str->string[start]), chars->string) + start;
	else
		ret_value = strspn(&(str->string[start]), chars->string) + start;

	if(ret_value != str->length)
		return ret_value;

	found_char = strchr(chars->string, str->string[str->length - 1]);

	if(inverse == false)
		return (found_char == NULL) ? -2 : str->length - 1;
	else
		return (found_char != NULL) ? -2 : str->length - 1;
}


long
find_str_in_sstring(const SString * str,
                    const SString * sub_str,
                    size_t start)
{
	char * return_val;

	if(str == NULL  ||  sub_str == NULL  ||  NULL == str->string
		    ||  NULL == sub_str->string  ||  start >= str->length)
		return -1;

	return_val = strstr(&(str->string[start]), sub_str->string);

	return (return_val == NULL) ? -2 : (int)(return_val - str->string);
}


static int
copy_split_strings(struct SStrings * strs,
                   const SString * str,
                   int * separators,
                   size_t len_of_separator) {
	int index = 0;
	int ret_value;
	size_t i;

	if(separators[0] != 0) {
		ret_value = copy_n_sstring(&(strs->sstrings[index]), str,
		                           0, separators[0]);
		if(ret_value < 0)
			return ret_value;

		index++;
	}

	for(i = 1; i < strs->length - 1; i++) {
		if(separators[i] != separators[i - 1] + len_of_separator) {
			ret_value = copy_n_sstring(&(strs->sstrings[index]), str,
			                           separators[i - 1] + len_of_separator,
			                           separators[i] - separators[i - 1]
			                            - len_of_separator);
			if(ret_value < 0)
				return ret_value;

			index++;
		}
	}

	if((strs->length > 1  ||  separators[0] == 0)
		    &&  separators[i - 1] + len_of_separator != str->length) {
		ret_value = copy_n_sstring(&(strs->sstrings[index]), str,
		                           separators[i - 1] + len_of_separator, 0);
		if(ret_value < 0)
			return ret_value;

		index++;
	}

	return index;
}


struct SStrings *
split_sstring(const SString * str,
              const SString * separator)
{
	struct SStrings * split;
	int * locations;  // locations of separators relative to start of str
	unsigned int allocated_num = 8;  // allocated number of locations
	unsigned int num_of_locations = 0;
	int split_index = 0;
	size_t i;

	if(str == NULL  ||  separator == NULL  ||  NULL == str->string
		    ||  NULL == separator->string)
		return NULL;

	locations = malloc(sizeof(int) * allocated_num);
	if(locations == NULL)
		return NULL;

	locations[num_of_locations] = find_str_in_sstring(str, separator, 0);
	if(locations[num_of_locations] == -1) {
		split = NULL;
		goto bad_allocation;
	}
	if(locations[num_of_locations] == -2)  {
		locations[num_of_locations] = str->length;
		goto no_separator_found;
	}

	do {
		 num_of_locations++;

		if(num_of_locations >= allocated_num) {
			allocated_num += 8;
			locations = realloc(locations, sizeof(int) * allocated_num);
		}

		locations[num_of_locations] = find_str_in_sstring(str, separator,
			    locations[num_of_locations - 1] + separator->length);
	} while(locations[num_of_locations] > -1);

no_separator_found:

	split = malloc(sizeof(struct SStrings) + (num_of_locations + 1) * sizeof(SString));
	if(split == NULL)
		goto bad_allocation;

	split->length = num_of_locations + 1;

	for(i = 0; i < split->length; i++)
		split->sstrings[i] = (SString){0};

	split_index = copy_split_strings(split, str, locations, separator->length);
	if(split_index < 0) {
		for(i = 0; split->sstrings[i].length != 0; i++)
			free(split->sstrings[i].string);
		free(split);

		split = NULL;
		goto bad_allocation;
	}

	if(split_index < split->length) {
		split = realloc(split, sizeof(struct SStrings) + split_index * sizeof(SString));
		if(split == NULL)
			goto bad_allocation;

		split->length = split_index;
	}

bad_allocation:
	free(locations);

	return split;
}


struct SStrings *
split_sstrings(const struct SStrings * strs,
               const SString * separator)
{
	SString temp;
	struct SStrings * ret_value;

	if(strs == NULL  ||  separator == NULL  ||  separator->string == NULL
		    ||  strs->length == 0)
		return NULL;

	temp = connect_sstrings(strs, separator);
	ret_value = split_sstring(&temp, separator);

	free(temp.string);

	return ret_value;
}
