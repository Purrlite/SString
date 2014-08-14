#include "sstring.h"

#include <string.h>
#include <stdio.h>
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


void
print_error_sstring(enum errors_SString error_num,
                    const char * message)
{
	char * error;

	switch(error_num) {
	case NO_ERROR_SS:
		error = "No error";
		break;

	case NULL_ARGUMENT_SS:
		error = "One of the arguments was a NULL pointer";
		break;

	case BAD_ARGUMENT_SS:
		error = "Invalid argument";
		break;

	case NO_MEMORY_SS:
		error = "Not enough memory";
		break;

	default:
		error = "Unknown error";
		break;
	}

	if(message == NULL)
		fprintf(stderr, "%s\n", error);
	else
		fprintf(stderr, "%s %s\n", message, error);
}


typedef SString (new_SString_func)(const char * ,
                                   size_t ,
                                   enum errors_SString *);


static SString
NULL_string_0_size(const char * string,
                   size_t size,
                   enum errors_SString * error)
{
	return (SString){ 0, 0, NULL };
}

static SString
NULL_string_non0_size(const char * string,
                      size_t size,
                      enum errors_SString * error)
{
	SString str = (SString){ 0, size, NULL };

	str.string = malloc(str.size);
	if(NULL == str.string) {
		*error = NO_MEMORY_SS;
		return (SString){0};
	}

	str.string[0] = '\0';

	return str;
}

static SString
nonNULL_string_0_size(const char * string,
                      size_t size,
                      enum errors_SString * error)
{
	size_t length = strlen(string);
	SString str = (SString){ length, length + 1, NULL };

	str.string = malloc(str.size);
	if(NULL == str.string) {
		*error = NO_MEMORY_SS;
		return (SString){0};
	}

	strcpy(str.string, string);

	return str;
}

static SString
nonNULL_string_non0_size(const char * string,
                         size_t size,
                         enum errors_SString * error)
{
	size_t length = strlen(string);
	SString str = (SString){
		.size = size,
		.length = (length > size) ? size - 1 : length
	};

	str.string = malloc(str.size);
	if(NULL == str.string) {
		*error = NO_MEMORY_SS;
		return (SString){0};
	}

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
            size_t size,
            enum errors_SString * error)
{
	enum errors_SString useless_var;

	if(error == NULL)
		error = &useless_var;

	*error = NO_ERROR_SS;

	new_SString_func * new_func = get_new_SString_func(string, size);
	return (new_func)(string, size, error);
}


SString
sub_sstring(const SString * str,
            size_t start,
            size_t num,
            enum errors_SString * error)
{
	size_t length;
	enum errors_SString useless_var;

	if(error == NULL) {
		error = &useless_var;
	}

	if(str == NULL  ||  str->string == NULL) {
		*error = NULL_ARGUMENT_SS;
		return (SString){0};
	}

	if(start > str->length) {
		*error = BAD_ARGUMENT_SS;
		return (SString){0};
	}

	length = (start + num > str->length  ||  num == 0)
	         ? str->length - start : num;

	*error = NO_ERROR_SS;

	return new_sstring(&(str->string[start]), length + 1, error);
}


inline enum errors_SString
free_sstring(SString * str)
{
	if(str == NULL)
		return NULL_ARGUMENT_SS;

	if(str->string != NULL)
		free(str->string);

	str->string = NULL;
	str->length = 0;
	str->size = 0;

	return NO_ERROR_SS;
}


inline enum errors_SString
free_sstrings(struct SStrings ** strs)
{
	size_t i;

	if(strs == NULL  ||  *strs == NULL)
		return NULL_ARGUMENT_SS;

	for(i = 0; i < (*strs)->length; i++)
		if((*strs)->sstrings[i].string != NULL)
			free((*strs)->sstrings[i].string);

	free(*strs);

	*strs = NULL;

	return NO_ERROR_SS;
}


inline SString
connect_sstrings(const struct SStrings * strs,
                 const SString * connector,
                 enum errors_SString * error)
{
	size_t i;
	SString str = (SString){0};
	enum errors_SString useless_var;

	if(error == NULL)
		error = &useless_var;

	if(strs == NULL) {
		*error  = NULL_ARGUMENT_SS;
		return (SString){0};
	}

	if(strs->length == 0) {
		*error = BAD_ARGUMENT_SS;
		return (SString){0};
	}

	for(i = 0; i < strs->length - 1; i++) {
		append_sstring(&str, &(strs->sstrings[i]));
		append_sstring(&str, connector);
	}
	append_sstring(&str, &(strs->sstrings[i]));

	*error = NO_ERROR_SS;

	return str;
}


static SString
to_X_sstring(const SString * str,
             bool to_lower)
{
	long location = 0;
	long previous_location = 0;
	SString new_str = new_sstring(NULL,str->length + 1, NULL);
	SString Xcase = (to_lower == true)
	                ? new_sstring("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0, NULL)
	                : new_sstring("abcdefghijklmnopqrstuvwxyz", 0, NULL);
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
to_lower_sstring(const SString * str,
                 enum errors_SString * error)
{
	enum errors_SString pointless_var;

	if(error == NULL)
		error = &pointless_var;

	if(str == NULL  ||  str->string == NULL) {
		*error = NULL_ARGUMENT_SS;
		return (SString){0};
	}

	return to_X_sstring(str, true);
}


SString
to_upper_sstring(const SString * str,
                 enum errors_SString * error)
{
	enum errors_SString pointless_var;

	if(error == NULL)
		error = &pointless_var;

	if(str == NULL  ||  str->string == NULL) {
		*error = NULL_ARGUMENT_SS;
		return (SString){0};
	}

	return to_X_sstring(str, false);
}


int
trim_sstring(SString * str)
{
	SString temp;
	size_t i;
	size_t location = 0; // location of a char that isn't space or tab

	if(str == NULL  ||  str->string == NULL)
		return NULL_ARGUMENT_SS;

	for(i = str->length; i > 0; i--)
		if(isblank(str->string[i]) == false) {
			location = i;
			break;
		}

	if(location == 0)
		return 1;

	if(location != str->length)
		remove_sstring(str, location, 0);

	temp = new_sstring(" \t", 0, NULL);

	location = find_chars_in_sstring(str, &temp, 0, true);

	if(location != 0)
		remove_sstring(str, 0, location);

	free(temp.string);

	return 1;
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

	if(destination == NULL  ||  source == NULL  ||  NULL == source->string)
		return NULL_ARGUMENT_SS;

	if(start > source->length)
		return BAD_ARGUMENT_SS;

	length = (start + num > source->length  ||  num == 0)
	         ? source->length - start : num;

	ensure_necessary_size(destination, length + 1, false);
	if(destination == NULL)
		return NO_MEMORY_SS;

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
		return NULL_ARGUMENT_SS;

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

	if(destination == NULL  ||  source == NULL  ||  NULL == source->string)
		return NULL_ARGUMENT_SS;

	if(start >= source->length)
		return BAD_ARGUMENT_SS;

	length = (start + num > source->length  ||  num == 0)
	         ? source->length - start : num;

	ensure_necessary_size(destination, length + destination->length + 1, true);
	if(destination == NULL)
		return NO_MEMORY_SS;

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
		return NULL_ARGUMENT_SS;

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

	if(destination == NULL  ||  source == NULL  ||  NULL == source->string)
		return NULL_ARGUMENT_SS;

	if(source_start >= source->length  ||  insert_start > destination->length)
		return BAD_ARGUMENT_SS;

	length = (source_start + num > source->length  ||  num == 0)
	         ? source->length - source_start : num;

	ensure_necessary_size(destination, destination->length + length + 1, true);
	if(destination == NULL)
		return NO_MEMORY_SS;

	temp = new_sstring(&(destination->string[insert_start]), 0, NULL);

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
		return NULL_ARGUMENT_SS;

	len = strlen(source);

	return insert_n_sstring(destination, &(SString){ len, len + 1, source },
	                        insert_start, 0, num);
}


int
remove_sstring(SString * str,
               size_t start,
               size_t num)
{
	SString temp;

	if(str == NULL  ||  str->string == NULL)
		return NULL_ARGUMENT_SS;

	if(start >= str->length)
		return BAD_ARGUMENT_SS;

	if(start + num > str->length  ||  num == 0) {
		str->string[start] = '\0';
		str->length = start;
	} else {
		temp = new_sstring(&(str->string[start + num]), 0, NULL);
		if(temp.string == NULL)
			return NO_MEMORY_SS;

		str->string[start] = '\0';
		str->length = start;

		append_sstring(str, &temp);

		free(temp.string);
	}

	return 1;
}


int
compare_sstrings(const SString * restrict str1,
                 const SString * restrict str2)
{
	int return_value;

	if(str1 == NULL  ||  str2 == NULL  ||  NULL == str1->string
		    ||  NULL == str2->string)
		return NULL_ARGUMENT_SS;

	return_value = strcmp(str1->string, str2->string);

	if(return_value == 0)
		return NO_DIFFERENCE;
	else if(return_value > 0)
		return FIRST_STRs_CHAR_BIGGER;
	else
		return SECOND_STRs_CHAR_BIGGER;
}


int
compare_n_sstrings(const SString * restrict str1,
                   const SString * restrict str2,
                   size_t num)
{
	int return_value;
	size_t length;
	char temp1;
	char temp2;

	if(str1 == NULL  ||  str2 == NULL  ||  NULL == str1->string
		    ||  NULL == str2->string)
		return NULL_ARGUMENT_SS;

	if(num == 0)
		return BAD_ARGUMENT_SS;

	length = (str1->length > str2->length) ? str2->length : str1->length;

	if(num >= length) {
		return_value = strcmp(str1->string, str2->string);
		goto got_return_value;
	} else
		length = num;

	temp1 = str1->string[length];
	temp2 = str2->string[length];
	str1->string[length] = '\0';
	str2->string[length] = '\0';

	return_value = strcmp(str1->string, str2->string);

	str1->string[length] = temp1;
	str2->string[length] = temp2;

got_return_value:
	if(return_value == 0)
		return NO_DIFFERENCE;
	else if(return_value > 0)
		return FIRST_STRs_CHAR_BIGGER;
	else
		return SECOND_STRs_CHAR_BIGGER;
}


long
find_chars_in_sstring(const SString * str,
                      const SString * chars,
                      size_t start,
                      _Bool inverse)
{
	size_t ret_value;
	char * found_char;

	if(str == NULL  ||  chars == NULL  ||  NULL == str->string
		    ||  NULL == chars->string)
		return NULL_ARGUMENT_SS;

	if(start >= str->length)
		return BAD_ARGUMENT_SS;

	if(inverse == false)
		ret_value = strcspn(&(str->string[start]), chars->string) + start;
	else
		ret_value = strspn(&(str->string[start]), chars->string) + start;

	if(ret_value != str->length)
		return ret_value;

	found_char = strchr(chars->string, str->string[str->length - 1]);

	if(inverse == false)
		return (found_char == NULL) ? NOT_FOUND_SS : str->length - 1;
	else
		return (found_char != NULL) ? NOT_FOUND_SS : str->length - 1;
}


long
find_str_in_sstring(const SString * str,
                    const SString * sub_str,
                    size_t start)
{
	char * return_val;

	if(str == NULL  ||  sub_str == NULL  ||  NULL == str->string
		    ||  NULL == sub_str->string)
		return NULL_ARGUMENT_SS;

	if(start >= str->length)
		return BAD_ARGUMENT_SS;

	return_val = strstr(&(str->string[start]), sub_str->string);

	return (return_val == NULL) ? NOT_FOUND_SS : (long)(return_val - str->string);
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
              const SString * separator,
              enum errors_SString * error)
{
	struct SStrings * split;
	int * locations;  // locations of separators relative to start of str
	unsigned int allocated_num = 8;  // allocated number of locations
	unsigned int num_of_locations = 0;
	int split_index = 0;
	size_t i;
	enum errors_SString useless_var;

	if(error == NULL)
		error = &useless_var;

	if(str == NULL  ||  separator == NULL  ||  NULL == str->string
		    ||  NULL == separator->string) {
		*error = NULL_ARGUMENT_SS;
		return NULL;
	}

	locations = malloc(sizeof(int) * allocated_num);
	if(locations == NULL) {
		*error = NO_MEMORY_SS;
		return NULL;
	}

	locations[num_of_locations] = find_str_in_sstring(str, separator, 0);
	if(locations[num_of_locations] == NOT_FOUND_SS) {
		locations[num_of_locations] = str->length;
		goto no_separator_found;
	}
	if(locations[num_of_locations] < 0) {
		*error = locations[num_of_locations];
		split = NULL;
		goto bad_allocation;
	}

	do {
		 num_of_locations++;

		if(num_of_locations >= allocated_num) {
			allocated_num += 8;
			locations = realloc(locations, sizeof(int) * allocated_num);
		}

		locations[num_of_locations] = find_str_in_sstring(str, separator,
			    locations[num_of_locations - 1] + separator->length);
	} while(locations[num_of_locations] >= 0);

no_separator_found:

	split = malloc(sizeof(struct SStrings) + (num_of_locations + 1) * sizeof(SString));
	if(split == NULL) {
		*error = NO_MEMORY_SS;
		goto bad_allocation;
	}

	split->length = num_of_locations + 1;

	for(i = 0; i < split->length; i++)
		split->sstrings[i] = (SString){0};

	split_index = copy_split_strings(split, str, locations, separator->length);
	if(split_index < 0) {
		for(i = 0; split->sstrings[i].length != 0; i++)
			free(split->sstrings[i].string);
		free(split);

		*error = split_index;
		split = NULL;
		goto bad_allocation;
	}

	if(split_index < split->length) {
		split = realloc(split, sizeof(struct SStrings) + split_index * sizeof(SString));
		if(split == NULL) {
			*error = NO_MEMORY_SS;
			goto bad_allocation;
		}

		split->length = split_index;
	}

bad_allocation:
	free(locations);

	return split;
}


struct SStrings *
split_sstrings(const struct SStrings * strs,
               const SString * separator,
               enum errors_SString * error)
{
	SString temp;
	struct SStrings * ret_value;
	enum errors_SString useless_var;

	if(error == NULL)
		error = &useless_var;

	if(strs == NULL  ||  separator == NULL  ||  separator->string == NULL) {
		*error = NULL_ARGUMENT_SS;
		return NULL;
	}

	if(strs->length == 0) {
		*error  = BAD_ARGUMENT_SS;
		return NULL;
	}

	temp = connect_sstrings(strs, separator, error);
	ret_value = split_sstring(&temp, separator, error);

	free(temp.string);

	return ret_value;
}
