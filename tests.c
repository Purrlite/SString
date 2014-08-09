#include "tests.h"

#include "sstring.h"
#include <string.h>
#include <ctype.h>

#include "stdio.h"


int 
test_sstring_allocation(void)
{
	SString test = (SString){0,0,0};
	test = new_sstring(NULL,0);
	if(test.size != 0 || test.length != 0 || test.string != NULL)
		return -1;
	
	char* temp = "something";
	test = new_sstring(temp,0);
	if(test.size != strlen(temp)+1 || test.length != strlen(temp) || test.string == NULL || strcmp(test.string,temp))
		return -2;

	free_sstring(&test);
	if(test.size != 0 || test.length != 0 || test.string != NULL)
		return -3;

	test = new_sstring(NULL,2);
	if(test.size != 2 || test.length != 0 || test.string == NULL || test.string[0] != '\0')
		return -4;
	free_sstring(&test);

	char* temp2 = "another longer test string";
	test = new_sstring(temp2,10);
	if(test.size != 10 || test.length != 9 || test.string == NULL || strncmp(test.string,temp2,9))
		return -5;
	free_sstring(&test);

	return 0;
}

bool test_to_lower_sstring_helper(const char* input, const char* output)
{
	SString test = new_sstring(input,0);
	SString lowerstring = to_lower_sstring(&test);

	if(input != NULL) {
		if(strcmp(output,lowerstring.string))
			return true;
	} else {
		if(lowerstring != NULL)
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
	SString test = new_sstring(input,0);
	SString upperstring = to_upper_sstring(&test);

	if(input != NULL) {
		if(strcmp(output,upperstring.string))
			return true;
	} else {
		if(lowerstring != NULL)
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


bool test_all_sstring_functions()
{
	int allocation_failcode = test_sstring_allocation();
	if(allocation_failcode)
		printf("failed allocation test with: %i \r\n", allocation_failcode);
	else
		printf("allocation tests succeeded \n");

	int to_lower_failcode = test_to_lower_sstring();
	if(to_lower_failcode) 
		printf("failed to_lower test with: %i \r\n", to_lower_failcode);
	else
		printf("to lower tests succeeded \n");

	int to_upper_failcode = test_to_upper_sstring();
	if(to_upper_failcode) 
		printf("failed to_upper test with: %i \r\n", to_upper_failcode);
	else
		printf("to upper tests succeeded \n");

	return (!allocation_failcode && !to_lower_failcode && !to_upper_failcode);
}