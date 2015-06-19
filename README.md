SString (Struct String) [![Build Status](https://travis-ci.org/Purlox/SString.svg?branch=master)](https://travis-ci.org/Purlox/SString) [![Coverity Scan Build Status](https://scan.coverity.com/projects/5455/badge.svg)](https://scan.coverity.com/projects/5455)
=======================

SString is a string library that introduces a new structure SString with many functions to work with them.

The documentation can be found in sstring.h.

----------

The SString structure consists of:
  1. The string itself, which is a C string, so normal C string functions can use it.
  2. Length, which is the amount of meaningful bits for characters in the string. It doesn't include \0 or  characters behind it.
  3. Size, which is the size of the allocated memory for the string.

