#include "s_string.h"

#include <string.h>

s_string *
s_init(const char * restrict string,
       size_t size)
{
	s_string * restrict s_str;

	s_str = malloc(sizeof(s_string));
	if(NULL == s_str)
		return NULL;

	if(NULL == string) {
		s_str->length = 0;
		s_str->size = size;

		if(0 != size) {
			s_str->string = malloc(size);
			if(NULL == s_str->string)
				return NULL;

			s_str->string[0] = '\0';
		} else
			s_str->string = NULL;

	} else {
		s_str->size = (0 == size) ? strlen(string) + 1 : size;
		s_str->length = (0 == size) ? s_str->size - 1
		                : ((size > strlen(string)) ? strlen(string) : size - 1);

		s_str->string = malloc(s_str->size);
		if(NULL == s_str->string)
			return NULL;

		strncpy(s_str->string, string, s_str->size);
	}

	return s_str;
}


typedef s_string * (s_str_init_func)(s_string * restrict s_str,
                                     const char * restrict string,
                                     size_t size);


static s_string *
NULL_array_0_size(s_string * restrict s_str,
                  const char * restrict string,
                  size_t size)
{
	s_str->length = 0;
	s_str->size = 0;
	s_str->string = NULL;

	return s_str;
}


static s_string *
NULL_array_non0_size(s_string * restrict s_str,
                     const char * restrict string,
                     size_t size)
{
	s_str->length = 0;
	s_str->size = size;

	s_str->string = malloc(s_str->size);
	if(NULL == s_str->string)
		return NULL;

	s_str->string[0] = '\0';

	return s_str;
}


static s_string *
nonNULL_array_0_size(s_string * restrict s_str,
                     const char * restrict string,
                     size_t size)
{
	s_str->length = strlen(string);
	s_str->size = s_str->length + 1;

	s_str->string = malloc(s_str->size);
	if(NULL == s_str->string)
		return NULL;

	strncpy(s_str->string, string, s_str->size);

	return s_str;
}


static s_string *
nonNULL_array_non0_size(s_string * restrict s_str,
                        const char * restrict string,
                        size_t size)
{
	s_str->size = size;
	s_str->length = (size <= strlen(string)) ? size - 1 : strlen(string);

	s_str->string = malloc(s_str->size);
	if(NULL == s_str->string)
		return NULL;

	strncpy(s_str->string, string, s_str->length);
	s_str->string[s_str->length] = '\0';

	return s_str;
}


static s_str_init_func *
get_function(const char * restrict string,
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


s_string *
s_init2(const char * restrict string,
        size_t size)
{
	s_string * restrict s_str;

	s_str_init_func * init_function = get_function(string, size);

	s_str = malloc(sizeof(s_string));
	if(NULL == s_str)
		return NULL;

	return((init_function)(s_str, string, size));
}


void
s_empty(s_string * restrict str)
{
	if(str == NULL  ||  str->string == NULL)
		return ;

	free(str->string);

	str->string = NULL;
	str->length = 0;
	str->size = 0;
}


void
s_free(s_string * restrict str)
{
	if(str == NULL)
		return ;

	if(str->string != NULL)
		free(str->string);

	free(str);
	str = NULL;
}


char *
s_sstonews(s_string * restrict str)
{
	char * s;
	unsigned int i;

	if(NULL == str  ||  NULL == str->string)
		return NULL;

	 s = malloc(str->length + 1);

	for(i = 0; i < str->length; i++) {
		s[i] = str->string[i];
	}

	s[str->length] = '\0';

	return s;
}

// Version 2
char *
s_sstonews2(s_string * restrict str)
{
	char * s;
	register unsigned int i;

	if(NULL == str  ||  NULL == str->string)
		return NULL;

	s = malloc(str->length + 1);

	for(i = 0; i < str->length; i++) {
		s[i] = str->string[i];
	}

	s[str->length] = '\0';

	return s;
}


void *
s_memnull(void * restrict memory,
          size_t num)
{
	unsigned int i;

	for(i = 0; i < num; i++)
		*((char*)memory + i) = 0;

	return memory;
}

// version 2
void *
s_memnull2(void * memory,
           size_t num)
{
	char * restrict _help = memory;
	unsigned int i;

	for(i = 0; i < num; i++)
		_help[i] = 0;

	return _help;
}

// version 3
void *
s_memnull3(void * restrict memory,
           size_t num)
{
	register unsigned int i;

	for(i = 0; i < num; i++)
		*((char*)memory + i) = 0;

	return memory;
}

// version 4
void *
s_memnull4(void * memory,
           size_t num)
{
	char * restrict _help = memory;
	register unsigned int i;

	for(i = 0; i < num; i++)
		_help[i] = 0;

	return _help;
}


void *
s_memcpy(void * destination,
         const void * source,
         size_t num)
{
	char * restrict _help = destination;
	const char * restrict _help2 = source;
	unsigned int i;

	for(i = 0; i < num; i++)
		_help[i] = _help2[i];

	return _help;
}

// version 2
void *
s_memcpy2(void * restrict destination,
          const void * restrict source,
          size_t num)
{
	unsigned int i;

	for(i = 0; i < num; i++)
		*((char*)destination + i) = *((const char*)source + i);

	return destination;
}

// version 3
void *
s_memcpy3(void * restrict destination,
          const void *restrict source,
          size_t num)
{
	unsigned int i;

	for(i = 0; i < num; i++)
		*((char*)destination++) = *((const char*)source++);

	return destination;
}


void *
s_memmove(void * destination,
          const void * source,
          size_t num)
{
	char * restrict _copy = malloc(num);
	char * _help = destination;
	const char *_help2 = source;
	unsigned int i;

	for(i = 0; i < num; i++) {
		_copy[i] = _help2[i];
	}

	for(i = 0; i < num; i++) {
		_help[i] = _copy[i];
	}

	free(_copy);

	return _help;
}

// version 2
void *
s_memmove2(void * destination,
           const void * source,
           size_t num)
{
	void * restrict _copy = malloc(num);
	unsigned int i;

	for(i = 0; i < num; i++) {
		*((char *)_copy + i) = *((const char*)source + i);
	}

	for(i = 0; i < num; i++) {
		*((char*)destination + i) = *((char *)_copy + i);
	}

	free(_copy);

	return destination;
}

// version 3
void *
s_memmove3(void * destination,
           const void * source,
           size_t num)
{
	void * restrict _copy = malloc(num);
	unsigned int i;

	for(i = 0; i < num; i++) {
		*((char *)_copy++) = *((const char*)source++);
	}

	_copy -= num;

	for(i = 0; i < num; i++) {
		*((char*)destination++) = *((char *)_copy++);
	}

	_copy -= num;
	free(_copy);

	return destination;
}


s_string *
s_strcpy(s_string * restrict destination,
         const s_string * restrict source)
{
	unsigned int i;
	size_t length;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	length = (source->length > destination->size) ? destination->size
	         : source->length;

	for(i = 0; i < length; i++)
		destination->string[i] = source->string[i];

	destination->length = length;

	return destination;
}

// Version 2
s_string *
s_strcpy2(s_string * restrict destination,
          const s_string * restrict source)
{
	register unsigned int i;
	register size_t length;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	length = (source->length > destination->size) ? destination->size
	         : source->length;

	for(i = 0; i < length; i++)
		destination->string[i] = source->string[i];

	destination->length = length;

	return destination;
}

// Version 3
s_string *
s_strcpy3(s_string * restrict destination,
          const s_string * restrict source)
{
	unsigned int i;
	size_t length;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	length = (source->length > destination->size) ? destination->size
	         : source->length;

	for(i = 0; i < length / 8; i++)
		((uint64_t *)(destination->string))[i] = ((uint64_t *)(source->string))[i];

	for(i *= 8; i < length; i++)
		destination->string[i] = source->string[i];

	destination->length = i;

	return destination;
}

// Version 4
s_string *
s_strcpy4(s_string * restrict destination,
          const s_string * restrict source)
{
	unsigned int i;
	uint64_t * _dest = (uint64_t *)(destination->string);
	uint64_t * _sour = (uint64_t *)(source->string);
	size_t length;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	length = (source->length > destination->size) ? destination->size
	         : source->length;

	for(i = 0; i < length / 8; i++)
		_dest[i] = _sour[i];

	for(i *= 8; i < length; i++)
		destination->string[i] = source->string[i];

	destination->length = i;

	return destination;
}


s_string *
s_strncpy(s_string * restrict destination,
          const s_string * restrict source,
          size_t num)
{
	unsigned int i;
	size_t length;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	if(0 == num)
		return destination;

	if(num > source->length)
		length = (source->length > destination->size) ? destination->size
		          : source->length;
	else
		length = (num > destination->size) ? destination->size : num;

	for(i = 0; i < length; i++)
		destination->string[i] = source->string[i];

	destination->length = length;

	return destination;
}

// version 2
s_string *
s_strncpy2(s_string * restrict destination,
           const s_string * restrict source,
           size_t num)
{
	unsigned int i;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	if(0 == num)
		return destination;

	for(i = 0; i < num / 8; i++)
		((uint64_t *)(destination->string))[i] = ((uint64_t *)(source->string))[i];

	for(i *= 8; i < num; i++)
		destination->string[i] = source->string[i];

	destination->length = num;

	return destination;
}

// version 3
s_string *
s_strncpy3(s_string * destination,
           const s_string * source,
           size_t num)
{
	unsigned int i;
	uint64_t * restrict _dest = (uint64_t *)(destination->string);
	uint64_t * restrict _sour = (uint64_t *)(source->string);

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	if(0 == num)
		return destination;

	_dest = (uint64_t *)(destination->string);
	_sour = (uint64_t *)(source->string);

	for(i = 0; i < num / 8; i++)
		_dest[i] = _sour[i];

	for(i *= 8; i < num; i++)
		_dest[i] = _sour[i];

	destination->length = num;

	return destination;
}


s_string *
s_strcpyX(s_string * restrict destination,
          const char * source,
          size_t num)
{
	unsigned int i;
	size_t max;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string)
		return NULL;

	max = (destination->size >= num) ? num - 1 : destination->size - 1;
	for(i = 0; i < max; i++)
		destination->string[i] = source[i];
	destination->string[max] = '\0';

	destination->length = max;

	return destination;
}


s_string *
s_strcat(s_string * restrict destination,
         const s_string * restrict source)
{
	unsigned int i;
	unsigned int j;
	size_t length;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	length = (source->length > destination->length) ? destination->length
	         : source->length;

	for(i = destination->length, j = 0; j < length; i++, j++)
		destination->string[i] = source->string[j];

	destination->length += source->length;

	return destination;
}

// version 2
s_string *
s_strcat2(s_string * restrict destination,
          const s_string * restrict source)
{
	unsigned int i;
	size_t length;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	length = (source->length > destination->length) ? destination->length
	         : source->length;

	for(i = 0; i < length; i++)
		destination->string[i + destination->length] = source->string[i];

	destination->length += source->length;

	return destination;
}


s_string *
s_strncat(s_string * restrict destination,
          const s_string * restrict source,
          size_t num)
{
	unsigned int i;
	unsigned int j;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	for(i = destination->length, j = 0; j < num; i++, j++)
		destination->string[i] = source->string[j];

	destination->length += num;

	return destination;
}

// version 2
s_string *
s_strncat2(s_string * restrict destination,
           const s_string * restrict source,
           size_t num)
{
	unsigned int i;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	for(i = 0; i < num; i++)
		destination->string[i + destination->length] = source->string[i];

	destination->length += num;

	return destination;
}

// version 3
s_string *
s_strncat3(s_string * restrict destination,
           const s_string * restrict source,
           size_t num)
{
	int _help;
	unsigned int i;

	if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
	   || NULL == source->string)
		return NULL;

	_help = destination->length;

	for(i = 0; i < num; i++)
		destination->string[i + _help] = source->string[i];

	destination->length += num;

	return destination;
}


int
s_memcmp(const void * ptr1,
         const void * ptr2,
         size_t num)
{
	const char * restrict _p1 = ptr1;
	const char * restrict _p2 = ptr2;
	unsigned int i;

	for(i = 0; i < num; i++) {
		if(_p1[i] > _p2[i])
			return (_p1[i] - _p2[i]);
		else if(_p1[i] < _p2[i])
			return (_p1[i] - _p2[i]);
	}

	return 0;
}

// Version 2
int
s_memcmp2(const void * restrict ptr1,
          const void * restrict ptr2,
          size_t num)
{
	unsigned int i;

	for(i = 0; i < num; i++) {
		if(*((char*)ptr1 + i) > *((char*)ptr2 + i))
			return (*((char*)ptr1 + i) - *((char*)ptr2 + i));
		else if(*((char*)ptr1 + i) < *((char*)ptr2 + i))
			return (*((char*)ptr1 + i) - *((char*)ptr2 + i));
	}

	return 0;
}


int
s_strcmp(const s_string * restrict str1,
         const s_string * restrict str2)
{
	unsigned int i;

	for(i = 0; i < str1->length && i < str2->length; i++) {
		if(str1->string[i] > str2->string[i])
			return (str1->string[i] - str2->string[i]);
		else if(str1->string[i] < str2->string[i])
			return (str1->string[i] - str2->string[i]);
	}

	return 0;
}

// Version 2
int
s_strcmp2(const s_string * str1,
          const s_string * str2)
{
	unsigned int i;
	const s_string * restrict _help = str1;
	const s_string * restrict _help2 = str2;

	for(i = 0; i < _help->length && i < _help2->length; i++) {
		if(_help->string[i] > _help2->string[i])
			return (_help->string[i] - _help2->string[i]);
		else if(_help->string[i] < _help2->string[i])
			return (_help->string[i] - _help2->string[i]);
	}

	return 0;
}


int
s_strncmp(const s_string * str1,
          const s_string * str2,
          size_t num)
{
	unsigned int i;

	for(i = 0; i < num && i < str1->length && i < str2->length; i++) {
		if(str1->string[i] > str2->string[i])
			return (str1->string[i] - str2->string[i]);
		else if(str1->string[i] < str2->string[i])
			return (str1->string[i] - str2->string[i]);
	}

	return 0;
}


void *
s_memchr(void * ptr,
         int value,
         size_t num)
{
	char * _p = ptr;
	unsigned int i;

	for(i = 0; i < num; i++) {
		if(_p[i] == value)
			return(ptr+i);
	}

	return NULL;
}

// version 2
void *
s_memchr2(void * ptr,
          int value,
          size_t num)
{
	unsigned int i;

	for(i = 0; i < num; i++) {
		if(*((char *)ptr + i) == value)
			return(ptr+i);
	}

	return NULL;
}


s_string *
s_strchr(s_string * str,
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
s_strspn(const s_string * str1,
         const s_string * str2)
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
s_strcspn(const s_string * str1,
          const s_string * str2)
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
s_strpbrk(const s_string * str1,
          const s_string * str2)
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
s_strrchr(s_string * str,
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
s_strstr(const s_string * str1,
         const s_string * str2)
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
s_strtok(s_string * str,
         const s_string * delimiters)
{
	static char * _lastPos = NULL;
	static s_string * _str = NULL;
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

	for(i = _lastPos - &(str->string[0]); i < _str->length; i++) {
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


s_string *
s_sstrtok(s_string * str,
          const s_string * delimiters)
{
	static char * _lastPos = NULL;
	static s_string *_str = NULL;
	char * startOfToken = NULL;
	char numOfDelimiters;
	unsigned int lengthOfToken = 0;
	unsigned int i;
	unsigned int j;
	s_string * newSstr = malloc(sizeof(s_string));

	if(str != NULL)
		_str = str;

	if(_lastPos == NULL)
		_lastPos = &(_str->string[0]);

	for(i = _lastPos - &(_str->string[0]); i < _str->length; i++) {
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


void *
s_memset(void * ptr,
         int value,
         size_t num)
{
	char * _p = ptr;
	unsigned int i;

	for(i = 0; i < num; i++)
		_p[i] = value;

	return ptr;
}

// version 2
void *
s_memset2(void * ptr,
          int value,
          size_t num)
{
	unsigned int i;

	for(i = 0; i < num; i++)
		*((char *)ptr + i) = value;

	return ptr;
}
