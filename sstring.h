#ifndef SSTRING_H_INCLUDED
#define SSTRING_H_INCLUDED

#include <stdlib.h>

typedef struct SString {
	size_t length;
	size_t size;
	char * string;
} SString;

/* Mainly intended as an easier way to make SString arrays.
 * Not intended for extensive use.
 */
struct SStrings {
	size_t length;
	SString sstrings[];
};

/* Note that empty SString means its string will be NULL and all other members
 * will be 0.
 */


/* Creates a SString with characters from a C-string either fully or up to
 * size - 1 if it's smaller than the whole string and allocates size amount of
 * characters for SString's string.
 *     If string is NULL, then it just allocates memory as long as size isn't 0.
 * If size is 0, then it zeroes the struct. If string isn't NULL and size is 0,
 * then it will only allocate as much memory as it needs to hold the string
 * including '\0'.
 *     Returns the new SString or an empty (all values zeroed) one on failure.
 */
SString
new_sstring(const char * restrict string,
            size_t size) ;


/* Creates a new SString made out of part of the original SString.
 *     Returns the new SString or an empty string if str or its string is NULL,
 * if start is bigger than str's length or if memory allocation fails.
 */
SString
sub_sstring(const SString * str,
            size_t start,
            size_t num) ;


/* Frees the memory allocated for the SString and the string in it.
 */
void
free_sstring(SString * str) ;


/* Frees the memory allocated for structure SStrings and the strings inside it
 */
void
free_sstrings(struct SStrings ** strs) ;


/* Connects all the SStrings in strs together with connector between each of
 * them into a new SString.
 *     If there is no connector, then it will just connect them together.
 *     Returns the new SString or empty SString if strs is NULL or has no
 * strings in it.
 */
SString
connect_sstrings(const struct SStrings * strs,
                 const SString * connector) ;


/* Converts all chars in str to lower case and returns new SString containing them.
 *     Returns the new SString or empty SString if str it's string is NULL.
 */
SString
to_lower_sstring(const SString * str) ;


/* Converts all chars in str to upper case and returns new SString containing them.
 *     Returns the new SString or empty SString if str it's string is NULL.
 */
SString
to_upper_sstring(const SString * str) ;


/* Removes leading and trailing spaces and tabs from SString.
 *     Returns 1 on success, 0 if there is no char that isn't a space or tab
 * or exits with -1 if str or its string is NULL.
 */
int
trim_sstring(SString * str) ;


/* Copies string from source into destination
 *     If destination's string is NULL or not big enough, then it allocates
 * enough memory to fit source's string in there.
 *     Returns 1 on success or exits with -1 if any of the arguments is NULL
 * or source's string is NULL or with -2 if allocating memory fails.
 */
int
copy_sstring(SString * restrict destination,
             const SString * restrict source) ;


/* Copies num characters from source starting at start into destination.
 *     If num is 0, then copies up to the end of source.
 *     If destination's string is NULL or not big enough, then it allocates
 * enough memory to fit the string in there.
 *     Returns 1 on success or exits with -1 if any of the arguments is NULL
 * or source's string is NULL or if start is bigger than source->length,
 * or with -2 if allocating memory fails.
 */
int
copy_n_sstring(SString * restrict destination,
               const SString * restrict source,
               size_t start,
               size_t num) ;


/* Copies num characters from string source into SString destination.
 *     If destination's string is NULL or not big enough, then it allocates
 * enough memory to fit the string in there.
 *     Returns 1 on success or exits with -1 if any of the arguments is NULL
 * or with -2 if allocating memory fails.
 */
int
copy_str_to_sstring(SString * restrict destination,
                    const char * restrict source,
                    size_t num);


/* Appends source to destination.
 *     If destination's string is NULL or not big enough, then it allocates
 * enough memory to fit the string in there.
 *     Returns 1 on success or exits with -1 if any of the arguments is NULL
 * or source's string is NULL or with -2 if allocating memory fails.
 */
int
append_sstring(SString * restrict destination,
               const SString * restrict source) ;


/* Appends n characters from source starting from start to destination.
 *     If num is 0, then appends all chars starting from start to the end.
 *     If destination's string is NULL or not big enough, then it allocates
 * enough memory to fit the string in there.
 *     Returns 1 on success or exits with -1 if any of the arguments is NULL
 * or source's string is NULL or if start is bigger or equal to source->length,
 * or with -2 if allocating memory fails.
 */
int
append_n_sstring(SString * restrict destination,
                 const SString * restrict source,
                 size_t start,
                 size_t num) ;

/* Appends num characters from source to destination.
 *     If num is 0, then appends all chars starting from start to the end.
 *     If destination's string is NULL or not big enough, then it allocates
 * enough memory to fit the string in there.
 *     Returns 1 on success or exits with -1 if any of the arguments is NULL or
 * source's string is NULL, or with -2 if memory allocation fails.
 */
int
append_str_to_sstring(SString * restrict destination,
                      const char * restrict source,
                      size_t num) ;


/* Inserts source into destination starting from insert_start.
 *     If destination's string is NULL or not big enough, then it allocates
 * enough memory to fit the string in there.
 *     Returns 1 on success or exits with -1 if any of the arguments is NULL
 * or source's string is NULL or if insert_start is bigger than destination's
 * length, or with -2 if allocating memory fails.
 */
int
insert_sstring(SString * restrict destination,
               const SString * restrict source,
               size_t insert_start) ;


/* Inserts num chars from source starting from source_start into destination
 * starting from insert_start.
 *     If num is 0, then it inserts source from source_start to the end.
 *     If destination's string is NULL or not big enough, then it allocates
 * enough memory to fit the string in there.
 *     Returns 1 on success or exits with -1 if any of the arguments is NULL
 * or source's string is NULL if source_start is bigger or equal to source's
 * lengthor if insert_start is bigger than destination's length, or with -2 if
 * allocating memory fails.
 */
int
insert_n_sstring(SString * restrict destination,
                 const SString * restrict source,
                 size_t insert_start,
                 size_t source_start,
                 size_t num) ;


/* Inserts num chars from source into destination starting at insert_start.
 *     If num is 0, then it inserts source from source_start to the end.
 *     If destination's string is NULL or not big enough, then it allocates
 * enough memory to fit the string in there.
 *     Returns 1 on success or exits with -1 if any of the arguments is NULL or
 * source's string is NULL or if insert_start is bigger than destination's
 * lenght, or with -2 if memory allocation fails.
 */
int
insert_str_to_sstring(SString * restrict destination,
                      const char * restrict source,
                      size_t insert_start,
                      size_t num) ;


/* Removes chars in str starting from start up to num chars.
 *     If num is 0, then it removes all characters starting from start.
 *     Returns 1 on success or exits with -1 if str or its string is NULL, or
 * with -2 if memory allocation fails.
 */
int
remove_sstring(SString * str,
               size_t start,
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
                      _Bool inverse) ;


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
 *     Returns the split strings or the source string inside struct SStrings
 * if no separator was found or NULL if either of the arguments or their string
 * is NULL or function fails allocating memory.
 */
struct SStrings *
split_sstring(const SString * str,
              const SString * separator) ;


/* Splits strings from struct SString into smaller ones devided by separator.
 * Doesn't remove any extra whitespace from them or do anything else.
 *     Returns the split strings or the source string inside struct SStrings
 * if no separator was found or NULL if either of the arguments or their string
 * is NULL or function fails allocating memory or if there are no SStrings in
 * strs.
 */
struct SStrings *
split_sstrings(const struct SStrings * strs,
               const SString * separator) ;


#endif // SSTRING_H_INCLUDED
