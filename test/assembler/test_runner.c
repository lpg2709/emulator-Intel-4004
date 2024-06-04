#include "../vendor/unity/unity_internals.h"
#include "./scan_tokens.c"

void setUp() {}

void tearDown() {}

int main() {
	UNITY_BEGIN();
	RUN_TEST(test_only_comment);
	RUN_TEST(test_only_NOP);
	RUN_TEST(test_only_FIM);
	RUN_TEST(test_FIM);
	return UNITY_END();
}

