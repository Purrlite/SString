#ifndef SSTRING_H_INCLUDED
#define SSTRING_H_INCLUDED

#include <stdlib.h>
#include <stdint.h>

#define restrict __restrict__

typedef struct s_string {
	size_t length;
	size_t size;
	char *string;
} s_string;


// Fast way to initialize s_string with a C string
#define S_STRING(str) (s_string){ sizeof(str)-1 , sizeof(str) , str }


/******************************************************************************\
  Initializes s_string with characters from a C-string and allocates new memory
for .string even if there was some allocated before.
  str is a s_string to which the characters will be added, array is a string of
characters to be added to str, num is the number of characters of the string to
be copied unless it finds a '\0';
  If str is NULL, then it creates a new one. If array is NULL, then it allocates
memory normally, but won't put any characters in there. If array is NULL and num
is 0, then it sets length to 0, but doesn't allocate anything. If num is 0, then
it will search until it finds '\0' and length will get set automatically to the
length of the array.
  Returns pointer to the initialized s_string;
\******************************************************************************/
s_string *
s_init(s_string * restrict str,
       char * restrict array,
       size_t size) ;

s_string *
s_init2(s_string * restrict str,
        char * restrict array,
        size_t size)


/******************************************************************************\
  Empties a s_string and frees memory used for the string inside it
  str is a s_string which will have its memory freed;
  Doesn't return anything (apart from the memory that it returns to OS);
\******************************************************************************/
void
s_empty(s_string * restrict str) ;


/******************************************************************************\
  Frees the memory allocated for s_string. Use this for actual pointers to
s_string;
  str is a s_string which will have its memory freed;
  Doesn't return anything (apart from the memory that it returns to OS);
\******************************************************************************/
void
s_free(s_string * restrict str) ;


/******************************************************************************\
  Makes a Cstring out of a s_string;
  str is a s_string which will have it's string become Cstring;
  Returns pointer to a Cstring in str or NULL if argument or string is NULL;
\******************************************************************************/
char *
s_sstos(s_string * restrict str) ;


/******************************************************************************\
  Makes a new Cstring out of a s_string;
  str is a s_string from which the new Cstring will be created;
  Returns pointer to a new Cstring copied from str or NULL if argument or string
is NULL;
\******************************************************************************/
char *
s_sstonews(s_string * restrict str) ;

char *
s_sstonews2(s_string * restrict str) ;


/******************************************************************************\
  Makes num bytes in memory 0;
  memory is the memory that is going to get edited, num is the amount of bytes
that will be changed;
  Returns pointer to the start of the edited memory;
\******************************************************************************/
void *
s_memnull(void * restrict memory,
          size_t num) ;

void *
s_memnull2(void *memory,
           size_t num) ;

void *
s_memnull3(void * restrict memory,
           size_t num) ;

void *
s_memnull4(void * memory,
           size_t num) ;


/******************************************************************************\
  Copies momory from source and replaces destination with it. Source can't
overlap with destination;
  destination is where the data will be copied to, source is where the data will
be coppied from, num is the number of bytes that will be copied;
  Returns pointer to destination;
\******************************************************************************/
void *
s_memcpy(void * destination,
         const void * source,
         size_t num) ;

void *
s_memcpy2(void * restrict destination,
          const void * restrict source,
          size_t num) ;

void *
s_memcpy3(void * restrict destination,
          const void * restrict source,
          size_t num) ;


/******************************************************************************\
  Copies momory from source and replaces destination with it. Source can overlap
with destination;
  destination is where the data will be copied to, source is where the data will
be coppied from, num is the number of bytes that will be copied;
  Returns pointer to destination;
\******************************************************************************/
void *
s_memmove(void * destination,
          const void * source,
          size_t num) ;

void *
s_memmove2(void * destination,
           const void *source,
           size_t num) ;

void *
s_memmove3(void * destination,
           const void *source,
           size_t num) ;


/******************************************************************************\
  Copies s_string from source and replaces destination with it;
  destination is where the data will be copied to, source is where the data will
be coppied from;
  Returns pointer to destination or NULL if any of the arguments is NULL or
their string is NULL;
\******************************************************************************/
s_string *
s_strcpy(s_string * restrict destination,
         const s_string * restrict source) ;

s_string *
s_strcpy2(s_string * restrict destination,
          const s_string * restrict source) ;

s_string *
s_strcpy3(s_string * restrict destination,
          const s_string * restrict source) ;

s_string *
s_strcpy4(s_string * restrict destination,
          const s_string * restrict source) ;


/******************************************************************************\
  Copies n characters from source and replaces destination with it;
  destination is where the data will be copied to, source is where the data will
be coppied from, num is the number of bytes to be coppied;
  Returns pointer to destination or NULL if any of the arguments is null or
their string member is;
\******************************************************************************/
s_string *
s_strncpy(s_string * restrict destination,
          const s_string * restrict source,
          size_t num) ;

s_string *
s_strncpy2(s_string * restrict destination,
           const s_string * restrict source,
           size_t num) ;

s_string *
s_strncpy3(s_string * destination,
           const s_string * source,
           size_t num);


/******************************************************************************\
  Adds string source at the end of destination;
  destination is where the data will be copied to, source is where the data will
be coppied from;
  Returns pointer to destination;
\******************************************************************************/
s_string *
s_strcat(s_string * restrict destination,
         const s_string * restrict source) ;

s_string *
s_strcat2(s_string * restrict destination,
          const s_string * restrict source) ;


/******************************************************************************\
  Adds n characters from source at the end of destination;
  destination is where the data will be copied to, source is where the data will
be coppied from, num is the number of bytes to be coppied;
  Returns pointer to destination;
\******************************************************************************/
s_string *
s_strncat(s_string * restrict destination,
          const s_string * restrict source,
          size_t num) ;

s_string *
s_strncat2(s_string * restrict destination,
           const s_string * restrict source,
           size_t num) ;

s_string *
s_strncat3(s_string * restrict destination,
           const s_string * restrict source,
           size_t num) ;


/******************************************************************************\
  Compares bytes in memories until num bytes or a first differnt byte;
  ptr1 and ptr2 are the memories being compared, num is number of bytes being
compared;
  Returns either 0 if there is no difference, positive number if ptr1's
byte has higher value than ptr2's byte, negative if ptr1's byte has smaller
value than ptr2's byte;
\******************************************************************************/
int
s_memcmp(const void * ptr1,
         const void * ptr2,
         size_t num) ;

int
s_memcmp2(const void * restrict ptr1,
          const void * restrict ptr2,
          size_t num);


/******************************************************************************\
  Lexically compares characters in the s_strings until the end of one of the
s_strings or a first different character;
  str1 and str2 are the s_strings being compared;
  Returns either 0 if there is no difference, positive number if ptr1's
character has higher value than ptr2's, negative if ptr1's character has smaller
value than ptr2's character;
\******************************************************************************/
int
s_strcmp(const s_string * restrict str1,
         const s_string * restrict str2) ;

int
s_strcmp2(const s_string * str1,
          const s_string * str2) ;


/******************************************************************************\
  Lexically compares characters in the s_strings until the end of one of the
s_strings, a first different character or num characters;
  str1 and str2 are the s_strings being compared, num is the number of
characters being compared;
  Returns either 0 if there is no difference, positive number if ptr1's
character has higher value than ptr2's, negative if ptr1's character has smaller
value than ptr2's character;
\******************************************************************************/
int
s_strncmp(const s_string * str1,
          const s_string * str2,
          size_t num) ;


/******************************************************************************\
  Locates byte with specified value in memory in the first num bytes;
  ptr is memory being searched, value is the value being searched for, num is
the number of bytes being searched
  Peturns a pointer to first matching byte. If such byte is not found then it
returns NULL;
\******************************************************************************/
void *
s_memchr(void * ptr,
         int value,
         size_t num) ;

void *
s_memchr2(void * ptr,
          int value,
          size_t num) ;


/******************************************************************************\
  Locates specified character in s_string;
  ptr is s_string being searched, character is the character being searched for;
  Returns a pointer to the first matching character; if no matching character
was found then it returns NULL;
\******************************************************************************/
s_string *
s_strchr(s_string * str,
         int character) ;


/******************************************************************************\
  Get the amount of characters before the first occurance of a character that
isn't in str2
  str1 is the s_strings being searched in, str2 contians characters that are
being searched against
  Returns the number of characters before the first non-matching character,
returns -1 in case of no character from str2 being in str1;
\******************************************************************************/
size_t
s_strspn(const s_string * str1,
         const s_string * str2) ;


/******************************************************************************\
  Get the amount of characters before the first occurance of a character that
is in str2;
  str1 is the s_string being searched in, str2 contians characters that are
being searched against;
  Returns the number of characters before the first non-matching character,
returns -1 in case of all characters from str2 are in str1;
\******************************************************************************/
size_t
s_strcspn(const s_string * str1,
          const s_string * str2) ;


/******************************************************************************\
  Locates one of the characters from str2 in str1;
  str1 is the s_string being searched in, str2 contians characters that are
being searched against;
  Returns a pointer to the first matching character. If the character isn't
found, returns NULL;
\******************************************************************************/
char *
s_strpbrk(const s_string * str1,
          const s_string * str2) ;


/******************************************************************************\
  Locates the last occurance of the specified character;
  str is the s_string being searched in, character is the character being
searched for;
  Returns a pointer to the last occurance of such character. If the charecter
isn't found, then it returns NULL;
\******************************************************************************/
char *
s_strrchr(s_string * str,
          int character) ;


/******************************************************************************\
  Locates substring str2 in str1;
  str1 is the string being searched, str2 contains the substring being searched
for;
  Returns a pointer to the start of the substring; Returns NULL if not found;
\******************************************************************************/
char *
s_strstr(const s_string * str1,
         const s_string * str2) ;


/******************************************************************************\
  Splits s_string into tokens (smaller Cstrings devided by one of the signs in
delimiters). The function is intialized by giving it string in str and continues
searching through the last string supplied on NULL;
  str is the s_string being split, delimeners are the characters deviding  the
tokens;
  Returns pointer to next token, which is a new string and returns NULL if there
isn't any more tokens
\******************************************************************************/
char *
s_strtok(s_string * str,
         const s_string * delimiters) ;


/******************************************************************************\
  Splits s_string into tokens (smaller s_strings devided by one of the signs in
delimiters). The function is intialized by giving it string in str and continues
searching through the last string supplied on NULL;
  str is the s_string being split, delimeners are the characters deviding  the
tokens;
  Returns pointer to next token, which is a new string and returns NULL if there
isn't any more tokens
\******************************************************************************/
s_string *
s_sstrtok(s_string * str,
          const s_string * delimiters) ;


/******************************************************************************\
  Fills block of memory with value;
  ptr is the memory being changed, value is the value being filled up with, num
is the number of bytes being changed;
  Returns pointer to the memory;
\******************************************************************************/
void *
s_memset(void * ptr,
         int value,
         size_t num) ;

void *
s_memset2(void * ptr,
          int value,
          size_t num) ;


#endif // SSTRING_H_INCLUDED
