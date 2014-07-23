#include "sstring.h"

#include <string.h>

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

	if(str->string != NULL) {
		free(str->string);
		str->string = NULL;
	}

	str->length = 0;
	str->size = 0;
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

	strncpy(destination->string, &(source->string[start]), length);
	destination->string[length] = '\0';

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
                 size_t num)
{
	if(NULL == destination  ||  NULL == source  ||  NULL == source->string)
		return -1;

	if(num + destination->length > destination->size
		    ||  NULL == destination->string) {
		destination->size = num + destination->length + 1;
		if(NULL != destination->string)
			free(destination->string);
		destination->string = malloc(destination->size);

		if(NULL == destination->string)
			return -2;
	}

	strncpy(&(destination->string[destination->length]), source->string,
	        num);

	destination->length += num;

	destination->string[destination->length] = '\0';

	return 1;
}


int
compare_sstrings(const SString * restrict str1,
                 const SString * restrict str2)
{
	unsigned int i;
	unsigned int length = (str1->length > str2->length)
	                      ? str2->length : str1->length;

	for(i = 0; i < length; i++)
		if(str1->string[i] != str2->string[i])
			return (str1->string[i] - str2->string[i]);

	return 0;
}


int
s_strncmp(const SString * str1,
          const SString * str2,
          size_t num)
{
	unsigned int i;

	for(i = 0;  i < num  &&  i < str1->length  &&  i < str2->length;  i++) {
		if(str1->string[i] > str2->string[i])
			return (str1->string[i] - str2->string[i]);
		else if(str1->string[i] < str2->string[i])
			return (str1->string[i] - str2->string[i]);
	}

	return 0;
}


SString *
s_strchr(SString * str,
         int character)
{
	unsigned int i;

	for(i = 0; i < str->length; i++) {
		if(str->string[i] == character)
			return(str+i);
	}

	return NULL;
}


size_t
s_strspn(const SString * str1,
         const SString * str2)
{
	unsigned int i;
	unsigned int j;

	for(i = 0; i < str1->length; i++) {
		for(j = 0; j < str2->length; j++) {
			if(str1->string[i] != str2->string[j])
				return i;
		}
	}

	return -1;
}


size_t
s_strcspn(const SString * str1,
          const SString * str2)
{
	unsigned int i;
	unsigned int j;

	for(i = 0; i < str1->length; i++) {
		for(j = 0; j < str2->length; j++) {
			if(str1->string[i] == str2->string[j])
	 			return i;
		}
	}

	return -1;
}


char *
s_strpbrk(const SString * str1,
          const SString * str2)
{
	unsigned int i;
	unsigned int j;

	for(i = 0; i < str1->length; i++) {
		for(j = 0; j < str2->length; j++) {
			if(str1->string[i] == str2->string[j])
				return(&(str1->string[i]));
		}
	}

	return NULL;
}


char *
s_strrchr(SString * str,
          int character)
{
	char * pointer = NULL;
	unsigned int i;

	for(i = 0; i < str->length; i++) {
		if(str->string[i] == character)
			pointer = &(str->string[i]);
	}

	return pointer;
}


char *
s_strstr(const SString * str1,
         const SString * str2)
{
	unsigned int matching = 0;
	unsigned int i;

	for(i = 0; i < str1->length; i++) {
		if(str1->string[i] == str2->string[matching])
			matching++;
		else if(matching > 0)
			matching = 0;

		if(matching == str2->length)
			return&(str1->string[i-matching]);
	}

	return NULL;
}


char *
s_strtok(SString * str,
         const SString * delimiters)
{
	static char * _lastPos = NULL;
	static SString * _str = NULL;
	char * startOfToken = NULL;
	char numOfDelimiters;
	unsigned int lengthOfToken = 0;
	unsigned int i;
	unsigned int j;
	char * newString;

	if(str != NULL)
		_str = str;

	if(_lastPos == NULL)
		_lastPos = &(str->string[0]);

	for(i = _lastPos - &(str->string[0]);  i < _str->length;  i++) {
		numOfDelimiters = 0;

		for(j = 0; j < delimiters->length; j++) {
			if(_str->string[i] == delimiters->string[j])
				numOfDelimiters++;
		}

		if(numOfDelimiters != 0) {
			startOfToken = &(_str->string[i]);
			break;
		}
	}

	if(startOfToken == NULL) // No new token found
		return NULL;

	lengthOfToken = _lastPos - startOfToken;
	newString = malloc(lengthOfToken+1);

	for(i = 0; i < lengthOfToken; i++)
		newString[i] = *(_lastPos + i);
	newString[lengthOfToken] = '\0';

	_lastPos = startOfToken;

	return newString;
}


SString *
s_sstrtok(SString * str,
          const SString * delimiters)
{
	static char * _lastPos = NULL;
	static SString *_str = NULL;
	char * startOfToken = NULL;
	char numOfDelimiters;
	unsigned int lengthOfToken = 0;
	unsigned int i;
	unsigned int j;
	SString * newSstr = malloc(sizeof(SString));

	if(str != NULL)
		_str = str;

	if(_lastPos == NULL)
		_lastPos = &(_str->string[0]);

	for(i = _lastPos - &(_str->string[0]);  i < _str->length;  i++) {
		numOfDelimiters = 0;

		for(j = 0; j < (*delimiters).length; j++) {
			if(_str->string[i] == delimiters->string[j])
				numOfDelimiters++;
		}

		if(numOfDelimiters != 0) {
			startOfToken = &(_str->string[i]);
			break;
		}
	}

	if(startOfToken == NULL) // No new token found
		return NULL;

	lengthOfToken = _lastPos - startOfToken;
	newSstr->string = malloc(lengthOfToken);

	for(i = 0; i < lengthOfToken; i++)
		newSstr->string[i] = *(_lastPos + i);
	newSstr->length = i;

	_lastPos = startOfToken;

	return newSstr;
}
