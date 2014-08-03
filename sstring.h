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

/* Static allocation of a SString.
 * Don't use with anything else than "str".
 */
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


/* Frees the memory allocated for the string inside SString and sets everything
 * to 0, but doesn't free str itself.
 */
void
free_sstring(SString * str) ;


/* Frees the memory allocated for structure SStrings and the strings inside it
 */
void
free_sstrings(struct SStrings ** strs) ;


/* Copies string from source into destination
 *     Returns 1 on success or exits with -1 if any of the arguments is NULL
 * or destination's string is NULL or with -2 if allocating memory fails.
 */
int
copy_sstring(SString * restrict destination,
             const SString * restrict source) ;


/* Copies num characters from source starting at start into destination.
 *     If num is 0, then copies up to the end of source.
 *     Returns 1 on success or exits with -1 if any of the arguments is NULL
 * or source's string is NULL or if start is bigger or equal to source->length,
 * or with -2 if allocating memory fails.
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
 *     If num is 0, then appends all chars starting from start to the end.
 *     Returns 1 on success or exits with -1 if any of the arguments is NULL
 * or source's string is NULL or if start is bigger or equal to source->length,
 * or with -2 if allocating memory fails.
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
 *     If num is 0, then it inserts source from source_start to the end.
 *     Returns 1 on success or exits with -1 if any of the arguments is NULL
 * or source's string is NULL if source_start is bigger or equal to source's
 * length, or with -2 if allocating memory fails.
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
 * arguments or their string is NULL or num is 0.
 */
int
compare_n_sstrings(const SString * restrict str1,
                   const SString * restrict str2,
                   size_t num) ;


/* Finds the first occurance of a char from chars inside str starting from start
 * or if inverse is true, then finds the first occurance of a char that isn't
 * in chars that is located in str starting from start.
 *     Returns the number of chars before such character or -2 if it's not found.
 * Counting starts from the actual start of the string in str, not from start.
 * It also exits with -1 if any of the arguments or their string is NULL or if
 * start is bigger than str's length.
 */
long
find_chars_in_sstring(const SString * str,
                      const SString * chars,
                      size_t start,
                      bool inverse) ;


/* Finds substring sub_str in str, starting from start amount of chars.
 *     Returns number of characters leading up to the sub_str in str from the
 * beginning of str or -2 if it wasn't found. Or exits with -1 if either of the
 * arguments or their string is NULL.
 */
long
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
