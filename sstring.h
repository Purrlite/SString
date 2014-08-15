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


enum errors_SS {
	SUCCESS_SS = 1,  // Used as return value by functions that don't have other return value indicating success
	NO_ERROR_SS = 0,  // Used when passed as an argument
	NULL_ARGUMENT_SS = -1,
	BAD_ARGUMENT_SS = -2,
	NO_MEMORY_SS = -3,
};

enum special_return_vals_SS {
	// For searching SStrings:
	NOT_FOUND_SS = -10,

	// For comparing SStrings:
	NO_DIFFERENCE = 0,
	FIRST_STRs_CHAR_BIGGER = 2,  // 1st string's char is bigger
	SECOND_STRs_CHAR_BIGGER = 1, // 2nd string's char is bigger

	// For trimming SStrings:
	NOTHING_TO_TRIM_SS = 0,
};

/* Note: passing NULL to functions that have error as their argument other than
 * print_error_sstring() will make the function function as normal except you
 * won't have a way to tell what error occured inside it.
 */


void
print_error_sstring(enum errors_SS error,
                    const char * message) ;
/**
 * Prints error message to stderr.
 */


SString
new_sstring(const char * restrict string,
            size_t size,
            enum errors_SS * error) ;
/**
 * Creates a new SString.
 *
 * Returns: new SString     on success
 *          zeroed SString  on error
 *
 * Error values: NO_ERROR_SS   if no error occurs
 *               NO_MEMORY_SS  if allocating memory fails
 *
 * NOTE: If size is 0, then exactly the amount of bytes needed for the new
 *   string will be allocated.
 *       If size is smaller than string's length then it will only copy size-1
 *   chars into the new string.
 */


SString
sub_sstring(const SString * str,
            size_t start,
            size_t num,
            enum errors_SS * error) ;
/**
 * Returns a new SString created chosen part of the original one.
 *
 * Returns: new SString     on success
 *          zeroed SString  on error
 *
 * Error values: NO_ERROR_SS       if no error occurs
 *               NO_MEMORY_SS      if allocating memory fails
 *               NULL_ARGUMENT_SS  if str or str->string is NULL
 *               BAD_ARGUMENT_SS   if start > str->length
 *
 * NOTE: If size is 0, then it will create a new SString from start to the end.
 */


enum errors_SS
free_sstring(SString * str) ;
/**
 * Frees SString.
 *
 * Returns: SUCCESS_SS        on success
 *          NULL_ARGUMENT_SS  if str or str->string is NULL
 *
 * NOTE: Doesn't free SString itself as it never gets dynamically allocated
 *   inside the library, so if you dynamically allocated it yourself, then
 *   you have to use additional free() to free it.
 */


enum errors_SS
free_sstrings(struct SStrings ** strs) ;
/**
 * Frees struct SString and sets it's pointer to NULL.
 *
 * Returns: SUCCESS_SS        on success
 *          NULL_ARGUMENT_SS  if strs or *strs is NULL
 */


SString
connect_sstrings(const struct SStrings * strs,
                 const SString * connector,
                 enum errors_SS * error) ;
/**
 * Connects SStrings together into a single SString.
 *
 * Returns: the resulting SString  on success
 *          zeroed SString         on error
 *
 * Error values: NO_ERROR_SS       if no error occurs
 *               NULL_ARGUMENT_SS  if strs is NULL
 *               BAD_ARGUMENT_SS   if strs->length is 0
 *
 * NOTE: When NULL gets passed in as connector, it will simply not put anything
 *   between the connected SStrings.
 */


SString
to_lower_sstring(const SString * str,
                 enum errors_SS * error) ;

SString
to_upper_sstring(const SString * str,
                 enum errors_SS * error) ;
/**
 * Returns a new SString with all chars in it changed to upper/lower versions.
 *
 * Returns: the resulting SString  on success
 *          zeroed SString         on error
 *
 * Error values: NO_ERROR_SS       if no error occurs
 *               NULL_ARGUMENT_SS  if str or str->string is NULL
 *               NO_MEMORY_SS      if allocating memory fails
 */


enum errors_SS
trim_sstring(SString * str) ;
/**
 * Removes leading and trailing spaces and tabs.
 *
 * Returns: SUCCESS_SS          on success
 *          NULL_ARGUMENT_SS    if str or str->string is NULL
 *          NOTHING_TO_TRIM_SS  if it doesn't find non-space, non-tab char in str
 *
 * NOTE: If the whole string is made of spaces or tabs, then it won't trim
 *   anything.
 */


enum errors_SS
copy_sstring(SString * restrict destination,
             const SString * restrict source) ;

enum errors_SS
copy_n_sstring(SString * restrict destination,
               const SString * restrict source,
               size_t start,
               size_t num) ;

enum errors_SS
copy_str_to_sstring(SString * restrict destination,
                    const char * restrict source,
                    size_t num);
/**
 * Copies string from source to destination.
 *
 * Returns: SUCCESS_SS        on success
 *          NULL_ARGUMENT_SS  if destination, source or source->string is NULL
 *          BAD_ARGUMENT_SS   if start > source->length
 *          NO_MEMORY_SS      if allocating memory fails
 *
 * NOTE: If num is 0 or start + num > source->length then it will copy all chars
 *   from source (starting from start if there is one).
 *       If destination's string is NULL or not big enough to contain source's
 *   string, then a big enough size will be allocated for it.
 */


enum errors_SS
append_sstring(SString * restrict destination,
               const SString * restrict source) ;

enum errors_SS
append_n_sstring(SString * restrict destination,
                 const SString * restrict source,
                 size_t start,
                 size_t num) ;

enum errors_SS
append_str_to_sstring(SString * restrict destination,
                      const char * restrict source,
                      size_t num) ;
/**
 * Appends string from source to destination.
 *
 * Returns: SUCCESS_SS        on success
 *          NULL_ARGUMENT_SS  if destination, source or source->string is NULL
 *          BAD_ARGUMENT_SS   if start >= source->length
 *          NO_MEMORY_SS      if allocating memory fails
 *
 * NOTE: If num is 0 or start + num > source->length then it will append all
 *   chars from source (starting from start if there is one).
 *       If destination's string is NULL or not big enough to contain source's
 *   string and the current string in destination, then a big enough size will
 *   be allocated for it.
 */


enum errors_SS
insert_sstring(SString * restrict destination,
               const SString * restrict source,
               size_t insert_start) ;

enum errors_SS
insert_n_sstring(SString * restrict destination,
                 const SString * restrict source,
                 size_t insert_start,
                 size_t source_start,
                 size_t num) ;

enum errors_SS
insert_str_to_sstring(SString * restrict destination,
                      const char * restrict source,
                      size_t insert_start,
                      size_t num) ;
/**
 * Inserts string from source into destination.
 *
 * Returns: SUCCESS_SS        on success
 *          NULL_ARGUMENT_SS  if destination, source or source->string is NULL
 *          BAD_ARGUMENT_SS   if source_start >= source->length
 *                               or insert_start > destination->length
 *          NO_MEMORY_SS      if allocating memory fails
 *
 * NOTE: If num is 0 or start + num > source->length then it will insert all
 *   chars from source (starting from source_start if there is one).
 *       If destination's string is NULL or not big enough to contain source's
 *   string and the current string in destination, then a big enough size will
 *   be allocated for it.
 */


enum errors_SS
remove_sstring(SString * str,
               size_t start,
               size_t num) ;
/**
 * Removes/Deletes string from str.
 *
 * Returns: SUCCESS_SS        on success
 *          NULL_ARGUMENT_SS  if str or str->string is NULL
 *          BAD_ARGUMENT_SS   if start >= start->length
 *          NO_MEMORY_SS      if allocating memory fails
 *
 * NOTE: If num is 0 or start + num > source->length then everything gets removed
 */


int
compare_sstrings(const SString * restrict str1,
                 const SString * restrict str2) ;

int
compare_n_sstrings(const SString * restrict str1,
                   const SString * restrict str2,
                   size_t num) ;
/**
 * Compares 2 strings together.
 *
 * Returns: NO_DIFFERENCE            if there is no difference between both strings
 *          FIRST_STRs_CHAR_BIGGER   if first differing char has a bigger value
 *                                     in str1 than str2
 *          SECOND_STRs_CHAR_BIGGER  if first differing char has a bigger value
 *                                     in str2 than str1
 *          NULL_ARGUMENT_SS         if str1 or str2 or any of their strings is NULL
 *          BAD_ARGUMENT_SS          if num is 0
 */


long
find_chars_in_sstring(const SString * str,
                      const SString * chars,
                      size_t start,
                      _Bool inverse) ;
/**
 * Searches str for any of the characters in chars, starting to search from start.
 *
 * If inverse is non-0, then it will look for any character NOT in chars.
 *
 * Returns: >= 0              on success (distance of the found character from
 *                               the actual start of str)
 *          NOT_FOUND_SS      if no chars from chars were found in str.
 *          NULL_ARGUMENT_SS  if str, chars or any of their strings is NULL
 *          BAD_ARGUMENT_SS   if start >= str->length
 */


long
find_str_in_sstring(const SString * str,
                    const SString * sub_str,
                    size_t start) ;
/**
 * Searches str for a string sub_str, starting to search from start.
 *
 * Returns: >= 0              on success (distance of the first character from
 *                               the actual start of str)
 *          NOT_FOUND_SS      if the string from sub_str is not found in str
 *          NULL_ARGUMENT_SS  if str, sub_str or any of their strings is NULL
 *          BAD_ARGUMENT_SS   if start >= str->length
 */


struct SStrings *
split_sstring(const SString * str,
              const SString * separator,
              enum errors_SS * error) ;

struct SStrings *
split_sstrings(const struct SStrings * strs,
               const SString * separator,
               enum errors_SS * error) ;
/**
 * Splits string according to the separator.
 *
 * Returns: The resulting struct SStrings *  on success
 *          NULL                             on error
 *
 * Error values: NO_ERROR_SS       if no error occurs
 *               NULL_ARGUMENT_SS  if str, separator or any of their strings is NULL
 *               BAD_ARGUMENT_SS   if str or separator is 0 chars long
 *                                    or if strs has no SStrings in it
 *               NO_MEMORY_SS      if memory allocation fails
 */


#endif // SSTRING_H_INCLUDED
