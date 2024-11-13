#include "../vendor/unity/unity.h"

#include "../../src/a_token.h"

static void test_empty_token() {
	token t = { 0 };
	TEST_ASSERT_EQUAL_INT(TOKEN_TYPE_UNKNOW, t.type);
	TEST_ASSERT_EQUAL_INT(0, t.line);
	TEST_ASSERT_NULL(t.lex);
	TEST_ASSERT_EQUAL_INT(0, t.lex_size);
}

static void test_create_new_token_eof() {
	token t = token_new(TOKEN_TYPE_EOF, 1, NULL, 1);
	TEST_ASSERT_EQUAL_INT(TOKEN_TYPE_EOF, t.type);
	TEST_ASSERT_EQUAL_INT(1, t.line);
	TEST_ASSERT_NULL(t.lex);
	TEST_ASSERT_EQUAL_INT(1, t.lex_size);
}

static void test_check_opcode_macro_gennerated() {
	TEST_ASSERT_GREATER_THAN(11, TOKEN_TYPE_LEN);
}

