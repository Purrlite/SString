#ifndef SSTRING_TESTS_H_INCLUDED
#define SSTRING_TESTS_H_INCLUDED

#include <stdbool.h>

/*
 * Tests: allocation
 *        to upper
 *        to lower
 *        copying
 *
 * Returns: true   if all tests went well
 *          false  if at least one test failed
 */
bool
test_all_sstring_functions(void);

#endif
