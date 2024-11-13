#include "../vendor/unity/unity_internals.h"
#include "./a_token.c"

void setUp() {}

void tearDown() {}

int main() {
	UNITY_BEGIN();
	RUN_TEST(test_empty_token);
	RUN_TEST(test_create_new_token_eof);
	RUN_TEST(test_check_opcode_macro_gennerated);
	return UNITY_END();
}
