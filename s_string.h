#ifndef SSTRING_H_INCLUDED
#define SSTRING_H_INCLUDED

#include <stdlib.h>
#include <stdint.h>

#define restrict __restrict__

// @TODO: Add checks for NULL at the start of the functions
// @TODO: Check functions if I could add restrict keywords to them 

/******************************************************************************\
  s_string stores length of the string in "length" that is equal to the exact 
amount of meaningfull characters in the string, stores the size of the string,
which is the amount of allocated memory for the string, it stores the string in
"string";
  s_string doesn't always end with '\0' at the end of the string, but can be
added in there when needed;
\******************************************************************************/
typedef struct {
  size_t length;
  size_t size;
  char *string;
} s_string;

// Just a hacky struct that is being used for testing speed
typedef struct {
  char pointer[64];
} __pointer64;


// Fast way to initialize s_string with a C string
#define s_string(str) (s_string){ sizeof(str)-1 , sizeof(str) , str }


/******************************************************************************\
  Initializes s_string with characters from a Cstring (ending with '\0') and
allocates new memory for .string even if there was some allocated before.
  str is a s_string to which the characters will be added, array is a string of
characters to be added to str, num is the number of characters of the string to
be copied unless it finds a '\0';
  If str is NULL, then it creates a new one. If array is NULL, then it allocates
memory normally, but won't put any characters in ther. If array is NULL and num
is 0, then it sets length to 0, but doesn't allocate anything. If num is 0, then
it will search until it finds '\0' and length will get set automatically to the
length of the array.
  Returns pointer to the initialized s_string;
\******************************************************************************/
s_string *s_init(s_string *restrict str, char *restrict array, size_t size) {
  register int i;

  if(NULL == str) {
    str = malloc(sizeof(s_string));
    
    if(NULL == str) {
      perror("Error allocating memory for s_string");
      return NULL;  
    }
  }

  if(NULL == array) {
    str->length = 0;
    str->size = size;
    
    if(0 != size) {
      str->string = malloc(size * sizeof(char));
      if(NULL == str->string) {
        perror("Error allocating memory for s_string");
        return NULL;  
      }
    } else
      str->string = NULL;
      
  } else {
    if(0 == size) {
      str->string = NULL;

      for(i = 0; array[i] != '\0'; i++) {
        str->string = realloc((void *)(str->string), i + 1);
        if(NULL == str->string) {
          perror("Error allocating memory for s_string");
          return NULL;  
        }
        str->string[i] = array[i];
      }

      str->length = str->size = i;
    } else {
      str->string = malloc(size);
      if(NULL == str->string) {
        perror("Error allocating memory for s_string");
        return NULL;  
      }
      str->size = size;

      for(i = 0; i < size; i++)
        str->string[i] = array[i];
        
      str->length = ('\0' == str->string[i]) ? size-1 : size;
    }
  }
  
  return str;
}

/******************************************************************************\
  Frees the memory allocated for s_string. Use this only on s_string, not on
pointers;
  str is a s_string which will have its memory freed;
  Doesn't return anything (apart from the memory that it returns to OS);
\******************************************************************************/
void s_free(s_string *restrict str) {
  if(str != NULL  &&  str->string != NULL) {
    free(str->string);
    
    str->string = NULL;
    str->length = 0;
    str->size = 0;
  }
}


/******************************************************************************\
  Frees the memory allocated for s_string. Use this for actual pointers to
s_string;
  str is a s_string which will have its memory freed;
  Doesn't return anything (apart from the memory that it returns to OS);
\******************************************************************************/
void s_pfree(s_string *restrict str) {
  if(str != NULL) {
    if(str->string != NULL) {
      free(str->string);
      str->string = NULL;
    }
    
    free(str);
    str = NULL;
  }
}


/******************************************************************************\
  Makes a Cstring out of a s_string;
  str is a s_string which will have it's string become Cstring;
  Returns pointer to a Cstring in str or NULL if argument or string is NULL;
\******************************************************************************/
char *s_sstos(s_string *restrict str) {
  if(NULL == str  ||  NULL == str->string)
    return NULL; 
  
  if('\0' != str->string[str->length])
    str->string[str->length] = '\0';
  return str->string;
}


/******************************************************************************\
  Makes a new Cstring out of a s_string;
  str is a s_string from which the new Cstring will be created;
  Returns pointer to a new Cstring copied from str or NULL if argument or string
is NULL;
\******************************************************************************/
char *s_sstonews(s_string *restrict str) {
  char *s;
  int i;
  
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
char *s_sstonews2(s_string *restrict str) {
  char *s;
  register int i;
  
  if(NULL == str  ||  NULL == str->string)
    return NULL; 
    
   s = malloc(str->length + 1);

  for(i = 0; i < str->length; i++) {
    s[i] = str->string[i];
  }

  s[str->length] = '\0';

  return s;
}


/******************************************************************************\
  Makes num bytes in memory 0;
  memory is the memory that is going to get edited, num is the amount of bytes
that will be changed;
  Returns pointer to the start of the edited memory;
\******************************************************************************/
void *s_memnull(void *restrict memory, size_t num) {
  int i;

  for(i = 0; i < num; i++)
    *((char*)memory + i) = 0;

  return memory;
}

// second version
void *s_memnull2(void *memory, size_t num) {
  char *restrict _help = memory;
  int i;
  
  for(i = 0; i < num; i++)
    _help[i] = 0;

  return _help;
}

// Third version
void *s_memnull3(void *restrict memory, size_t num) {
  register int i;

  for(i = 0; i < num; i++)
    *((char*)memory + i) = 0;

  return memory;
}

// Fourth version
void *s_memnull4(void *memory, size_t num) {
  char *restrict _help = memory;
  register int i;
  
  for(i = 0; i < num; i++)
    _help[i] = 0;

  return _help;
}


/******************************************************************************\
  Copies momory from source and replaces destination with it. Source can't
overlap with destination;
  destination is where the data will be copied to, source is where the data will
be coppied from, num is the number of bytes that will be copied;
  Returns pointer to destination;
\******************************************************************************/
void *s_memcpy(void *destination, const void *source, size_t num) {
  char *restrict _help = destination;
  const char *restrict _help2 = source;
  int i;

  for(i = 0; i < num; i++)
    _help[i] = _help2[i];

  return _help;
}

// Second version
void *s_memcpy2(void *restrict destination, const void *restrict source,
                size_t num) {
  int i;
  
  for(i = 0; i < num; i++)
    *((char*)destination + i) = *((const char*)source + i);

  return destination;
}

// Third version
void *s_memcpy3(void *restrict destination, const void *restrict source,
                size_t num) {
  int i;
  
  for(i = 0; i < num; i++)
    *((char*)destination++) = *((const char*)source++);

  return destination;
}


/******************************************************************************\
  Copies momory from source and replaces destination with it. Source can overlap
with destination;
  destination is where the data will be copied to, source is where the data will
be coppied from, num is the number of bytes that will be copied;
  Returns pointer to destination;
\******************************************************************************/
void *s_memmove(void *destination, const void *source, size_t num) {
  char *restrict _copy = malloc(num),
        *_help = destination;
   const char *_help2 = source;
   int i;

  for(i = 0; i < num; i++) {
    _copy[i] = _help2[i];
  }

  for(i = 0; i < num; i++) {
    _help[i] = _copy[i];
  }

  free(_copy);

  return _help;
}

// Second version
void *s_memmove2(void *destination, const void *source, size_t num) {
  void *restrict _copy = malloc(num);
  int i;

  for(i = 0; i < num; i++) {
    *((char *)_copy + i) = *((const char*)source + i);
  }

  for(i = 0; i < num; i++) {
    *((char*)destination + i) = *((char *)_copy + i);
  }

  free(_copy);

  return destination;
}

// Third version
void *s_memmove3(void *destination, const void *source, size_t num) {
  void *restrict _copy = malloc(num);
  int i;

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


/******************************************************************************\
  Copies s_string from source and replaces destination with it;
  destination is where the data will be copied to, source is where the data will
be coppied from;
  Returns pointer to destination or NULL if any of the arguments is NULL or 
their string is NULL;
\******************************************************************************/
s_string *s_strcpy(s_string *restrict destination, const s_string *restrict 
                   source) {
  int i;
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
s_string *s_strcpy2(s_string *restrict destination, const s_string *restrict 
                   source) {
  register int i;
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
s_string *s_strcpy3(s_string *restrict destination, const s_string *restrict
                    source) {
  int i;
  __pointer64 *_dest = (__pointer64 *)(destination->string),
              *_sour = (__pointer64 *)(source->string);
  short _size = sizeof(__pointer64);
  size_t length;
  
  if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
     || NULL == source->string)
    return NULL;
    
  length = (source->length > destination->size) ? destination->size 
           : source->length;
  
  for(i = 0; i < length / _size; i++)
    _dest[i] = _sour[i];
  
  for(i *= _size; i < length; i++)
    destination->string[i] = source->string[i];

  destination->length = i;

  return destination;
}

// Version 4
s_string *s_strcpy4(s_string *restrict destination, const s_string *restrict
                    source) {
  int i;
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

// Version 5
s_string *s_strcpy5(s_string *restrict destination, const s_string *restrict 
                    source) {
  int i;
  uint64_t *_dest = (uint64_t *)(destination->string),
           *_sour = (uint64_t *)(source->string);
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


/******************************************************************************\
  Copies n characters from source and replaces destination with it;
  destination is where the data will be copied to, source is where the data will
be coppied from, num is the number of bytes to be coppied;
  Returns pointer to destination or NULL if any of the arguments is null or 
their string member is;
\******************************************************************************/
s_string *s_strncpy(s_string *restrict destination, const s_string *restrict 
                    source, size_t num) {
  int i;
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

// Second version
s_string *s_strncpy2(s_string *restrict destination, const s_string *restrict 
                     source, size_t num) {
  int i;
  __pointer64 *_dest = (__pointer64 *)(destination->string),
              *_sour = (__pointer64 *)(source->string);
  short _size = sizeof(__pointer64);
    
  if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
     || NULL == source->string)
    return NULL;
    
  if(0 == num)
    return destination;
  
  for(i = 0; i < num / _size; i++)
    _dest[i] = _sour[i];
  
  for(i *= _size; i < num; i++)
    _dest[i] = _sour[i];

  destination->length = num;

  return destination;
}

// third version
s_string *s_strncpy3(s_string *restrict destination, const s_string *restrict 
                     source, size_t num) {
  int i;
  
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

// fourth version
s_string *s_strncpy4(s_string *destination, const s_string *source, size_t num){
  int i;
  uint64_t *__restrict__ _dest = (uint64_t *)(destination->string),
           *__restrict__ _sour = (uint64_t *)(source->string);
  
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

/******************************************************************************\
  Adds string source at the end of destination;
  destination is where the data will be copied to, source is where the data will
be coppied from;
  Returns pointer to destination;
\******************************************************************************/
s_string *s_strcat(s_string *restrict destination, const s_string *restrict 
                   source) {
  int i,
      j;
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

// second version
s_string *s_strcat2(s_string *restrict destination, const s_string *restrict
                    source) {
  int i;
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


/******************************************************************************\
  Adds n characters from source at the end of destination;
  destination is where the data will be copied to, source is where the data will
be coppied from, num is the number of bytes to be coppied;
  Returns pointer to destination;
\******************************************************************************/
s_string *s_strncat(s_string *restrict destination, const s_string *restrict
                    source, size_t num) {
  int i,
      j;
      
  if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
     || NULL == source->string)
    return NULL;

  for(i = destination->length, j = 0; j < num; i++, j++)
    destination->string[i] = source->string[j];

  destination->length += num;

  return destination;
}

// version 2
s_string *s_strncat2(s_string *restrict destination, const s_string *restrict
                     source, size_t num) {
  int i;
  
  if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
     || NULL == source->string)
    return NULL;

  for(i = 0; i < num; i++)
    destination->string[i + destination->length] = source->string[i];

  destination->length += num;

  return destination;
}

// version 3
s_string *s_strncat3(s_string *restrict destination, const s_string *restrict
                     source, size_t num) {
  int _help;
  int i;
  
  if(NULL == destination  ||  NULL == source  ||  NULL == destination->string
     || NULL == source->string)
    return NULL;
    
  _help = destination->length;

  for(i = 0; i < num; i++)
    destination->string[i + _help] = source->string[i];

  destination->length += num;

  return destination;
}


/******************************************************************************\
  Compares bytes in memories until num bytes or a first differnt byte;
  ptr1 and ptr2 are the memories being compared, num is number of bytes being
compared;
  Returns either 0 if there is no difference, positive number if ptr1's
byte has higher value than ptr2's byte, negative if ptr1's byte has smaller
value than ptr2's byte;
\******************************************************************************/
int s_memcmp(const void *ptr1, const void *ptr2, size_t num) {
  const char *restrict _p1 = ptr1,
             *restrict _p2 = ptr2;
  int i;

  for(i = 0; i < num; i++) {
    if(_p1[i] > _p2[i])
      return (_p1[i] - _p2[i]);
    else if(_p1[i] < _p2[i])
      return (_p1[i] - _p2[i]);
  }

  return 0;
}

// Version 2
int s_memcmp2(const void *restrict ptr1, const void *restrict ptr2, size_t num){
  int i;

  for(i = 0; i < num; i++) {
    if(*((char*)ptr1 + i) > *((char*)ptr2 + i))
      return (*((char*)ptr1 + i) - *((char*)ptr2 + i));
    else if(*((char*)ptr1 + i) < *((char*)ptr2 + i))
      return (*((char*)ptr1 + i) - *((char*)ptr2 + i));
  }

  return 0;
}


/******************************************************************************\
  Lexically compares characters in the s_strings until the end of one of the
s_strings or a first different character;
  str1 and str2 are the s_strings being compared;
  Returns either 0 if there is no difference, positive number if ptr1's
character has higher value than ptr2's, negative if ptr1's character has smaller
value than ptr2's character;
\******************************************************************************/
int s_strcmp(const s_string *restrict str1, const s_string *restrict str2) {
  int i;

  for(i = 0; i < str1->length && i < str2->length; i++) {
    if(str1->string[i] > str2->string[i])
      return (str1->string[i] - str2->string[i]);
    else if(str1->string[i] < str2->string[i])
      return (str1->string[i] - str2->string[i]);
  }

  return 0;
}

// Version 2
int s_strcmp2(const s_string *str1, const s_string *str2) {
  int i;
  const s_string *restrict _help = str1,
                 *restrict _help2 = str2;

  for(i = 0; i < _help->length && i < _help2->length; i++) {
    if(_help->string[i] > _help2->string[i])
      return (_help->string[i] - _help2->string[i]);
    else if(_help->string[i] < _help2->string[i])
      return (_help->string[i] - _help2->string[i]);
  }

  return 0;
}


/******************************************************************************\
  Lexically compares characters in the s_strings until the end of one of the
s_strings, a first different character or num characters;
  str1 and str2 are the s_strings being compared, num is the number of
characters being compared;
  Returns either 0 if there is no difference, positive number if ptr1's
character has higher value than ptr2's, negative if ptr1's character has smaller
value than ptr2's character;
\******************************************************************************/
int s_strncmp(const s_string *str1, const s_string *str2, size_t num) {
  int i;

  for(i = 0; i < num && i < str1->length && i < str2->length; i++) {
    if(str1->string[i] > str2->string[i])
      return (str1->string[i] - str2->string[i]);
    else if(str1->string[i] < str2->string[i])
      return (str1->string[i] - str2->string[i]);
  }

  return 0;
}


/******************************************************************************\
  Locates byte with specified value in memory in the first num bytes;
  ptr is memory being searched, value is the value being searched for, num is
the number of bytes being searched
  Peturns a pointer to first matching byte. If such byte is not found then it
returns NULL;
\******************************************************************************/
void *s_memchr(void *ptr, int value, size_t num) {
  char *_p = ptr;
  int i;

  for(i = 0; i < num; i++) {
    if(_p[i] == value)
      return(ptr+i);
  }

  return NULL;
}

// Not sure which version would be faster, so I put both of them in here for now
void *s_memchr2(void *ptr, int value, size_t num) {
  int i;

  for(i = 0; i < num; i++) {
    if(*((char *)ptr + i) == value)
      return(ptr+i);
  }

  return NULL;
}


/******************************************************************************\
  Locates specified character in s_string;
  ptr is s_string being searched, character is the character being searched for;
  Returns a pointer to the first matching character; if no matching character
was found then it returns NULL;
\******************************************************************************/
s_string *s_strchr(s_string *str, int character) {
  int i;

  for(i = 0; i < str->length; i++) {
    if(str->string[i] == character)
      return(str+i);
  }

  return NULL;
}


/******************************************************************************\
  Get the amount of characters before the first occurance of a character that
isn't in str2
  str1 is the s_strings being searched in, str2 contians characters that are
being searched against
  Returns the number of characters before the first non-matching character,
returns -1 in case of no character from str2 being in str1;
\******************************************************************************/
size_t s_strspn(const s_string *str1, const s_string *str2) {
  int i,
      j;

  for(i = 0; i < str1->length; i++) {
    for(j = 0; j < str2->length; j++) {
      if(str1->string[i] != str2->string[j])
        return i;
    }
  }

  return -1;
}


/******************************************************************************\
  Get the amount of characters before the first occurance of a character that
is in str2;
  str1 is the s_string being searched in, str2 contians characters that are
being searched against;
  Returns the number of characters before the first non-matching character,
returns -1 in case of all characters from str2 are in str1;
\******************************************************************************/
size_t s_strcspn(const s_string *str1, const s_string *str2) {
  int i,
      j;

  for(i = 0; i < str1->length; i++) {
    for(j = 0; j < str2->length; j++) {
      if(str1->string[i] == str2->string[j])
        return i;
    }
  }

  return -1;
}


/******************************************************************************\
  Locates one of the characters from str2 in str1;
  str1 is the s_string being searched in, str2 contians characters that are
being searched against;
  Returns a pointer to the first matching character. If the character isn't
found, returns NULL;
\******************************************************************************/
char *s_strpbrk(const s_string *str1, const s_string *str2) {
  int i,
      j;

  for(i = 0; i < str1->length; i++) {
    for(j = 0; j < str2->length; j++) {
      if(str1->string[i] == str2->string[j])
        return(&(str1->string[i]));
    }
  }

  return NULL;
}


/******************************************************************************\
  Locates the last occurance of the specified character;
  str is the s_string being searched in, character is the character being
searched for;
  Returns a pointer to the last occurance of such character. If the charecter
isn't found, then it returns NULL;
\******************************************************************************/
char *s_strrchr(s_string *str, int character) {
  char *pointer = NULL;
  int i;

  for(i = 0; i < str->length; i++) {
    if(str->string[i] == character)
      pointer = &(str->string[i]);
  }

  return pointer;
}


/******************************************************************************\
  Locates substring str2 in str1;
  str1 is the string being searched, str2 contains the substring being searched
for;
  Returns a pointer to the start of the substring; Returns NULL if not found;
\******************************************************************************/
char *s_strstr(const s_string *str1, const s_string *str2) {
  int matching = 0;
  int i;

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


/******************************************************************************\
  Splits s_string into tokens (smaller Cstrings devided by one of the signs in
delimiters). The function is intialized by giving it string in str and continues
searching through the last string supplied on NULL;
  str is the s_string being split, delimeners are the characters deviding  the
tokens;
  Returns pointer to next token, which is a new string and returns NULL if there
isn't any more tokens
\******************************************************************************/
char *s_strtok(s_string *str, const s_string *delimiters) {
  static char *_lastPos = NULL;
  static s_string *_str = NULL;
  char *startOfToken = NULL;
  char numOfDelimiters;
  int lengthOfToken = 0;
  int i,
      j;
  char *newString;

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


/******************************************************************************\
  Splits s_string into tokens (smaller s_strings devided by one of the signs in
delimiters). The function is intialized by giving it string in str and continues
searching through the last string supplied on NULL;
  str is the s_string being split, delimeners are the characters deviding  the
tokens;
  Returns pointer to next token, which is a new string and returns NULL if there
isn't any more tokens
\******************************************************************************/
s_string *s_sstrtok(s_string *str, const s_string *delimiters) {
  static char *_lastPos = NULL;
  static s_string *_str = NULL;
  char *startOfToken = NULL;
  char numOfDelimiters;
  int lengthOfToken = 0;
  int i,
      j;
  s_string *newSstr = malloc(sizeof(s_string));
  
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


/******************************************************************************\
  Fills block of memory with value;
  ptr is the memory being changed, value is the value being filled up with, num
is the number of bytes being changed;
  Returns pointer to the memory;
\******************************************************************************/
void *s_memset(void *ptr, int value, size_t num) {
  char *_p = ptr;
  int i;

  for(i = 0; i < num; i++)
    _p[i] = value;

  return ptr;
}

// Not sure which version would be faster, so I put both of them in here for now
void *s_memset2(void *ptr, int value, size_t num) {
  int i;
  
  for(i = 0; i < num; i++)
    *((char *)ptr + i) = value;

  return ptr;
}


#endif // SSTRING_H_INCLUDED
