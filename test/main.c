#include "test_c.h"

int main(){
	testClass_c* test = testClass_construct(44);
	testClass_doSomething(test, 3.14, true);
	testClass_destroy(test);

	return 0;
}
