#ifndef SSTRING_H_INCLUDED
#define SSTRING_H_INCLUDED

#include <stdlib.h>

#define restrict  __restrict__
#define bool      _Bool

typedef struct SString {
	size_t length;
	size_t size;
	char * string;
} SString;


struct SStrings {
	size_t length;
	SString sstrings[];
};

// Static allocation of a SString
#define SSTRING(str) ((SString){ sizeof(str)-1 , sizeof(str) , str })


/* Creates a SString with characters from a C-string either fully or up to
 * size - 1 if it's smaller than the whole string and allocates size amount of
 * characters for SString's string.
 *     If string is NULL, then it just allocates memory as long as size isn't 0.
 * Otherwise it just sets the variables members of the struct. If string isn't
 * NULL and size is 0, then it will only allocate as much memory as it needs to
 * hold the string including '/0'.
 *     Returns pointer to the initialized SString or NULL on failure.
 */
SString
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


/* Frees the memory allocated for structure SStrings and the strings inside it
 */
void
free_sstrings(struct SStrings * strs) ;


/* Copies string from source into destination
 *     Returns 1 on success or exits with -1 if any of the arguments is NULL
 * or destination's string is NULL or with -2 if allocating memory fails.
 */
int
copy_sstring(SString * restrict destination,
             const SString * restrict source) ;


/* Copies num characters from source starting at start into destination.
 *     Returns 1 on success or exits with -1 if any of the arguments is NULL
 * or source's string is NULL or with -2 if allocating memory fails.
 */
int
copy_n_sstring(SString * restrict destination,
               const SString * restrict source,
               size_t start,
               size_t num) ;


/* Copies num characters from string source into SString destination.
 *     Returns 1 on success or exits with -1 if any of the arguments is NULL
 * or with -2 if allocating memory fails.
 */
int
copy_string_to_sstring(SString * restrict destination,
                       const char * restrict source,
                       size_t num);


/* Appends source to destination.
 *     Returns 1 on success or exits with -1 if any of the arguments is NULL
 * or source's string is NULL or with -2 if allocating memory fails.
 */
int
append_sstring(SString * restrict destination,
               const SString * restrict source) ;


/* Appends n characters from source starting from start to destination.
 *     Returns 1 on success or exits with -1 if any of the arguments is NULL
 * or source's string is NULL or with -2 if allocating memory fails.
 */
int
append_n_sstring(SString * restrict destination,
                 const SString * restrict source,
                 size_t start,
                 size_t num) ;


/* Inserts source into destination starting from insert_start.
 *     Returns 1 on success or exits with -1 if any of the arguments is NULL
 * or source's string is NULL or with -2 if allocating memory fails.
 */
int
insert_sstring(SString * restrict destination,
               const SString * restrict source,
               size_t insert_start) ;


/* Inserts num chars from source starting from source_start into destination
 * starting from insert_start.
 *     Returns 1 on success or exits with -1 if any of the arguments is NULL
 * or source's string is NULL or with -2 if allocating memory fails.
 */
int
insert_n_sstring(SString * restrict destination,
                 const SString * restrict source,
                 size_t insert_start,
                 size_t source_start,
                 size_t num) ;


/* Compares characters until '\0' or a first differring character.
 *     Returns either 0 if there is no difference, positive number if ptr1's
 * character has higher value than ptr2's, negative if ptr1's character has
 * smaller value than ptr2's character. Also returns 0 if either of the
 * arguments or their string is NULL.
 */
int
compare_sstrings(const SString * restrict str1,
                 const SString * restrict str2) ;


/* Compares characters until '\0', num chars or a first differring character.
 *     Returns either 0 if there is no difference, positive number if ptr1's
 * character has higher value than ptr2's, negative if ptr1's character has
 * smaller value than ptr2's character. Also returns 0 if either of the
 * arguments or their string is NULL.
 */
int
compare_n_sstrings(const SString * restrict str1,
                   const SString * restrict str2,
                   size_t num) ;


/* Finds the x-th (place being the x) or last (if place is 0) character in str
 *     Returns the distance of the character from the start of the string or
 * returns -2 if it isn't in the string or exits with -1 if str or string in it
 * is NULL.
 */
int
find_char_in_sstring(const SString * str,
                     char character,
                     unsigned int place) ;


/* Checks if the characters present in chars are or aren't inside str
 *     inverse determines if to check for existence of absence of the chars in
 * str. If inverse is false, then existence is checked, but if inverse is true,
 * then absence is checked.
 *     Returns the amount of chars that are in str before finding a character
 * that is in str that isn't inside chars. If inverse is true, then returns the
 * amount of chars before finding a char that is present in chars. Or exits with
 * -1 if either of the arguments or their string is NULL.
 */
int
find_chars_in_sstring(const SString * str,
                      const SString * chars,
                      bool inverse) ;


/* Finds substring sub_str in str, starting from start amount of chars.
 *     Returns number of characters before sub_str up to the beginning of str.
 * (it ignores start) or exits with -1 if either of the arguments or their
 * string is NULL.
 */
int
find_str_in_sstring(const SString * str,
                    const SString * sub_str,
                    size_t start) ;


/* Splits strings into smaller ones devided by separator. Doesn't remove any
 * extra whitespace from them or anything else.
 *     Returns the split strings or NULL if separator isn't found or if either
 * of the arguments or their string is NULL.
 */
struct SStrings *
split_sstring(const SString * str,
              const SString * separator) ;


#endif // SSTRING_H_INCLUDED
