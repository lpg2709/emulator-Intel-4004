#include "../vendor/unity/unity.h"

#include <stdlib.h>
#include "../../src/a_token.h"
#include "../../src/a_scanner.h"

void test_only_comment() {
	scanner s;
	const char src[] = "; From: MCS-4 Assembly Language Programming Manual\n"
		"; Write the value 2 into status character 1 of DATA RAM register 0  of chip 0\n"
		";   of the currently selected DAT RAM bank;\n";
	scan_tokens(&s, src, sizeof(src));
	TEST_ASSERT_EQUAL_INT(s.current_token, 0);
}

void test_only_NOP() {
	scanner s;
	scan_tokens(&s, "NOP", 4);
	TEST_ASSERT_EQUAL_INT(s.current_token, 1);
	TEST_ASSERT_EQUAL_INT(s.tokens[0].type, TOKEN_TYPE_OP_NOP);
}

void test_only_FIM() {
	scanner s;
	scan_tokens(&s, "FIM", 4);
	TEST_ASSERT_EQUAL_INT(s.current_token, 1);
	TEST_ASSERT_EQUAL_INT(s.tokens[0].type, TOKEN_TYPE_OP_FIM);
}

void test_FIM() {
	scanner s;
	scan_tokens(&s, "FIM 2P 5", 9);
	TEST_ASSERT_EQUAL_INT(s.current_token, 3);
	TEST_ASSERT_EQUAL_INT(s.tokens[0].type, TOKEN_TYPE_OP_FIM);
	TEST_ASSERT_EQUAL_INT(s.tokens[1].type, TOKEN_TYPE_REGISTER_PAIR);
	TEST_ASSERT_EQUAL_INT(s.tokens[2].type, TOKEN_TYPE_NUMBER);
}

