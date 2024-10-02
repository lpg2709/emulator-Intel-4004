#include "../vendor/unity/unity_internals.h"
#include "./scan_tokens.c"

void setUp() {}

void tearDown() {}

#define X(code, value, name, words) RUN_TEST(test_only_##code);

int main() {
	UNITY_BEGIN();
	RUN_TEST(test_only_comment);
	// Set all only OPCODE teste
	X_OPCODE
	RUN_TEST(test_FIM);
	RUN_TEST(test_label_declaration);
	RUN_TEST(test_scan_RDn);
	return UNITY_END();
}
#undef X
