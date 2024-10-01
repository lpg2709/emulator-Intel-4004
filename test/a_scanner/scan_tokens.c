#include "../vendor/unity/unity.h"
#include <stdlib.h>
#include <string.h>

#include "../../src/a_token.h"
#include "../../src/a_scanner.h"

static void test_only_comment() {
	scanner s;
	const char src[] = "/ From: MCS-4 Assembly Language Programming Manual\n"
		"/ Write the value 2 into status character 1 of DATA RAM register 0  of chip 0\n"
		"/   of the currently selected DAT RAM bank;\n";
	scan_tokens(&s, src, sizeof(src));
	TEST_ASSERT_EQUAL_INT(s.current_token, 0);
	free(s.tokens);
}

// This gennerate a test for all OPCODE
#define X(code, value, name, words) static void test_only_##code () { \
	scanner s; \
	const char input[] = ""#code""; \
	scan_tokens(&s, input, strlen(input)); \
	TEST_ASSERT_EQUAL_INT(s.current_token, 1); \
	TEST_ASSERT_EQUAL_INT(s.tokens[0].type, TOKEN_TYPE_OP_##code); \
	free(s.tokens); \
}
X_OPCODE
#undef X

static void test_FIM() {
	scanner s;
	scan_tokens(&s, "FIM 2P 5", 9);
	TEST_ASSERT_EQUAL_INT(s.current_token, 3);
	TEST_ASSERT_EQUAL_INT(s.tokens[0].type, TOKEN_TYPE_OP_FIM);
	TEST_ASSERT_EQUAL_INT(s.tokens[1].type, TOKEN_TYPE_REGISTER_PAIR);
	TEST_ASSERT_EQUAL_INT(s.tokens[2].type, TOKEN_TYPE_NUMBER);
	free(s.tokens);
}

static void test_label_declaration() {
	scanner s;
	scan_tokens(&s, "label, NOP", 10);
	TEST_ASSERT_EQUAL_INT(s.current_token, 2);
	TEST_ASSERT_EQUAL_INT(s.tokens[0].type, TOKEN_TYPE_LABEL_DECLARATION);
	TEST_ASSERT_EQUAL_INT(s.tokens[1].type, TOKEN_TYPE_OP_NOP);
	free(s.tokens);
}
