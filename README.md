s_string
========

s_string is a string library that introduces a new structure s_string, that works like a string.

----------

The structure includes:
 1. The string itself, which is a C string, so normal C functions can use it.
 2. Length, which is the length of actual characters in the string. It doesn't include \0 or characters behind it. 
 3. Size, which is the size of the allocated memory for the string.

Functions that manipulate with s_strings have the same name like standard C string library functions except they are prefixed with "s_".
