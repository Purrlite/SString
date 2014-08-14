#include "tests.h"

#include "sstring.h"
#include <string.h>
#include <ctype.h>

#include "stdio.h"


int
test_sstring_allocation(void)
{
	SString test = (SString){0,0,0};
	test = new_sstring(NULL,0, NULL);
	if(test.size != 0 || test.length != 0 || test.string != NULL)
		return -1;

	char* temp = "something";
	test = new_sstring(temp,0, NULL);
	if(test.size != strlen(temp)+1 || test.length != strlen(temp) || test.string == NULL || strcmp(test.string,temp))
		return -2;

	free_sstring(&test);
	if(test.size != 0 || test.length != 0 || test.string != NULL)
		return -3;

	test = new_sstring(NULL,2, NULL);
	if(test.size != 2 || test.length != 0 || test.string == NULL || test.string[0] != '\0')
		return -4;
	free_sstring(&test);

	char* temp2 = "another longer test string";
	test = new_sstring(temp2,10, NULL);
	if(test.size != 10 || test.length != 9 || test.string == NULL || strncmp(test.string,temp2,9))
		return -5;
	free_sstring(&test);

	return 0;
}

bool test_to_lower_sstring_helper(const char* input, const char* output)
{
	SString test = new_sstring(input,0, NULL);
	SString lowerstring = to_lower_sstring(&test, NULL);

	if(output != NULL) {
		if(strcmp(output,lowerstring.string))
			return true;
	} else {
		if(lowerstring.string != NULL)
			return true;
	}


	free_sstring(&test);
	free_sstring(&lowerstring);
	return false;
}

int test_to_lower_sstring()
{
	if(test_to_lower_sstring_helper("tEsTiNg","testing"))
		return -1;
	if(test_to_lower_sstring_helper("TESTING","testing"))
		return -2;
	if(test_to_lower_sstring_helper("testing","testing"))
		return -3;
	if(test_to_lower_sstring_helper(NULL,NULL))
		return -4;
	if(test_to_lower_sstring_helper("",""))
		return -5;
	return 0;
}

bool test_to_upper_sstring_helper(const char* input, const char* output)
{
	SString test = new_sstring(input,0, NULL);
	SString upperstring = to_upper_sstring(&test, NULL);

	if(output != NULL) {
		if(strcmp(output,upperstring.string))
			return true;
	} else {
		if(upperstring.string != NULL)
			return true;
	}

	free_sstring(&test);
	free_sstring(&upperstring);
	return false;
}

int test_to_upper_sstring()
{
	if(test_to_upper_sstring_helper("tEsTiNg","TESTING"))
		return -1;
	if(test_to_upper_sstring_helper("TESTING","TESTING"))
		return -2;
	if(test_to_upper_sstring_helper("testing","TESTING"))
		return -3;
	if(test_to_upper_sstring_helper(NULL,NULL))
		return -4;
	if(test_to_upper_sstring_helper("",""))
		return -5;
	return 0;
}

bool test_copy_sstring_helper(const char* string)
{
	SString output = (SString){0,0,0};
	SString input = new_sstring(string,0, NULL);
	if(copy_sstring(&output,&input) == NULL_ARGUMENT_SS)
		return true;
	if(strcmp(output.string,string))
		return true;
	free_sstring(&output);
	free_sstring(&input);
	return false;
}

int test_copy_sstring()
{
	SString temp;
	if(copy_sstring(NULL,NULL) != NULL_ARGUMENT_SS)
		return -1;
	if(copy_sstring(&temp, NULL) != NULL_ARGUMENT_SS)
		return -2;
	if(test_copy_sstring_helper(""))
		return -3;
	if(test_copy_sstring_helper("test"))
		return -4;
	temp = new_sstring(NULL,100, NULL);
	const char* inputStr = "some short string";
	SString input = new_sstring(inputStr,0, NULL);
	if(copy_sstring(&temp,&input) == NULL_ARGUMENT_SS || strcmp(temp.string,inputStr) || temp.size != 100 || temp.length != strlen(inputStr))
		return -5;

	free_sstring(&temp);
	free_sstring(&input);
	return 0;
}

bool test_helper(int (*testFunc)(void),char* name)
{
	int test_failcode = testFunc();
	if(test_failcode)
		printf("failed %s test with: %i \n", name,  test_failcode);
	else
		printf("%s tests succeeded \n", name);
	return !test_failcode;
}

bool test_all_sstring_functions()
{
	bool allocation_success = test_helper(test_sstring_allocation, "allocation");
	bool to_lower_success = test_helper(test_to_lower_sstring, "to lower");
	bool to_upper_success = test_helper(test_to_upper_sstring,"to upper");
	bool copy_success = test_helper(test_copy_sstring,"copy");

	return allocation_success && to_lower_success && to_upper_success && copy_success;
}
