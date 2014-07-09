#ifndef SSTRING_H_INCLUDED
#define SSTRING_H_INCLUDED

#include <stdlib.h>
#include <stdint.h>

#define restrict __restrict__

typedef struct SString {
	size_t length;
	size_t size;
	char * string;
} SString;


// Static allocation of a SString
#define SSTRING(str) (SString){ sizeof(str)-1 , sizeof(str) , str }


/* Creates a SString with characters from a C-string either fully or up to
 * size - 1 if it's smaller than the whole string and allocates size amount of
 * characters for SString's string.
 *     If string is NULL, then it just allocates memory as long as size isn't 0.
 * Otherwise it just sets the variables members of the struct. If string isn't
 * NULL and size is 0, then it will only allocate as much memory as it needs to
 * hold the string including '/0'.
 *     Returns pointer to the initialized SString or NULL on failure.
 */
SString *
new_sstring(const char * restrict string,
            size_t size) ;


/* Empties a SString, but doesn't free any memory.
 */
void
empty_sstring(SString * str) ;


/* Frees the memory allocated for SString.
 */
void
free_sstring(SString * str) ;


/* Copies SString from source and replaces destination with it.
 *     destination is where the data will be copied to, source is where the data
 * will be coppied from.
 *     Returns pointer to destination or NULL if any of the arguments is NULL or
 * their string is NULL.
 */
SString *
s_strcpy(SString * restrict destination,
         const SString * restrict source) ;

SString *
s_strcpy2(SString * restrict destination,
          const SString * restrict source) ;

SString *
s_strcpy3(SString * restrict destination,
          const SString * restrict source) ;

SString *
s_strcpy4(SString * restrict destination,
          const SString * restrict source) ;


/* Copies n characters from source and replaces destination with it.
 *     destination is where the data will be copied to, source is where the data
 * will be coppied from, num is the number of bytes to be coppied.
 *     Returns pointer to destination or NULL if any of the arguments is null or
 * their string member is.
 */
SString *
s_strncpy(SString * restrict destination,
          const SString * restrict source,
          size_t num) ;

SString *
s_strncpy2(SString * restrict destination,
           const SString * restrict source,
           size_t num) ;

SString *
s_strncpy3(SString * destination,
           const SString * source,
           size_t num);


SString *
s_strcpyX(SString * restrict destination,
          const char * source,
          size_t num);


/* Adds string source at the end of destination.
 *     destination is where the data will be copied to, source is where the data
 * will be coppied from.
 *     Returns pointer to destination.
 */
SString *
s_strcat(SString * restrict destination,
         const SString * restrict source) ;

SString *
s_strcat2(SString * restrict destination,
          const SString * restrict source) ;


/* Adds n characters from source at the end of destination.
 *     destination is where the data will be copied to, source is where the data
 * will be coppied from, num is the number of bytes to be coppied.
 *     Returns pointer to destination.
 */
SString *
s_strncat(SString * restrict destination,
          const SString * restrict source,
          size_t num) ;

SString *
s_strncat2(SString * restrict destination,
           const SString * restrict source,
           size_t num) ;

SString *
s_strncat3(SString * restrict destination,
           const SString * restrict source,
           size_t num) ;


/* Compares bytes in memories until num bytes or a first differnt byte.
 *     ptr1 and ptr2 are the memories being compared, num is number of bytes
 * being compared.
 *     Returns either 0 if there is no difference, positive number if ptr1's
 * byte has higher value than ptr2's byte, negative if ptr1's byte has smaller
 * value than ptr2's byte.
 */
int
s_memcmp(const void * ptr1,
         const void * ptr2,
         size_t num) ;

int
s_memcmp2(const void * restrict ptr1,
          const void * restrict ptr2,
          size_t num);


/* Lexically compares characters in the SStrings until the end of one of the
 * SStrings or a first different character.
 *     str1 and str2 are the SStrings being compared.
 *     Returns either 0 if there is no difference, positive number if ptr1's
 * character has higher value than ptr2's, negative if ptr1's character has
 * smaller value than ptr2's character.
 */
int
s_strcmp(const SString * restrict str1,
         const SString * restrict str2) ;

int
s_strcmp2(const SString * str1,
          const SString * str2) ;


/* Lexically compares characters in the SStrings until the end of one of the
 * SStrings, a first different character or num characters.
 *     str1 and str2 are the SStrings being compared, num is the number of
 * characters being compared.
 *     Returns either 0 if there is no difference, positive number if ptr1's
 * character has higher value than ptr2's, negative if ptr1's character has
 * smaller value than ptr2's character.
 */
int
s_strncmp(const SString * str1,
          const SString * str2,
          size_t num) ;


/* Locates byte with specified value in memory in the first num bytes.
 *     ptr is memory being searched, value is the value being searched for, num
 * is the number of bytes being searched.
 *     Peturns a pointer to first matching byte. If such byte is not found then
 * it returns NULL.
 */
void *
s_memchr(void * ptr,
         int value,
         size_t num) ;

void *
s_memchr2(void * ptr,
          int value,
          size_t num) ;


/* Locates specified character in SString.
 *     ptr is SString being searched, character is the character being searched
 * for.
 *     Returns a pointer to the first matching character; if no matching
 * character was found then it returns NULL.
 */
SString *
s_strchr(SString * str,
         int character) ;


/* Get the amount of characters before the first occurance of a character that
 * isn't in str2.
 *     str1 is the SStrings being searched in, str2 contians characters that
 * are being searched against.
 *     Returns the number of characters before the first non-matching character.
 * Returns -1 in case of no character from str2 being in str1.
 */
size_t
s_strspn(const SString * str1,
         const SString * str2) ;


/* Get the amount of characters before the first occurance of a character that
 * is in str2.
 *     str1 is the SString being searched in, str2 contians characters that are
 * being searched against.
 *     Returns the number of characters before the first non-matching character,
 * Returns -1 in case of all characters from str2 are in str1.
 */
size_t
s_strcspn(const SString * str1,
          const SString * str2) ;


/* Locates one of the characters from str2 in str1.
 *     str1 is the SString being searched in, str2 contians characters that are
 * being searched against.
 *     Returns a pointer to the first matching character. If the character isn't
 * found, returns NULL.
 */
char *
s_strpbrk(const SString * str1,
          const SString * str2) ;


/* Locates the last occurance of the specified character.
 *     str is the SString being searched in, character is the character being
 * searched for.
 *     Returns a pointer to the last occurance of such character. If the
 * charecter isn't found, then it returns NULL.
 */
char *
s_strrchr(SString * str,
          int character) ;


/* Locates substring str2 in str1.
 *     str1 is the string being searched, str2 contains the substring being
 * searched for.
 *     Returns a pointer to the start of the substring; Returns NULL if not
 * found.
 */
char *
s_strstr(const SString * str1,
         const SString * str2) ;


/* Splits SString into tokens (smaller Cstrings devided by one of the signs in
 * delimiters). The function is intialized by giving it string in str and
 * continues searching through the last string supplied on NULL.
 *     str is the SString being split, delimeners are the characters deviding
 * the tokens.
 *     Returns pointer to next token, which is a new string and returns NULL if
 * there isn't any more tokens.
 */
char *
s_strtok(SString * str,
         const SString * delimiters) ;


/* Splits SString into tokens (smaller SStrings devided by one of the signs in
 * delimiters). The function is intialized by giving it string in str and
 * continues searching through the last string supplied on NULL.
 *     str is the SString being split, delimeners are the characters deviding
 * the tokens.
 *     Returns pointer to next token, which is a new string and returns NULL if
 * there isn't any more tokens.
 */
SString *
s_sstrtok(SString * str,
          const SString * delimiters) ;


/* Fills block of memory with value.
 *     ptr is the memory being changed, value is the value being filled up with,
 * num is the number of bytes being changed.
 *     Returns pointer to the memory.
 */
void *
s_memset(void * ptr,
         int value,
         size_t num) ;

void *
s_memset2(void * ptr,
          int value,
          size_t num) ;


#endif // SSTRING_H_INCLUDED
