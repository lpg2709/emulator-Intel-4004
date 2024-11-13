#include "../vendor/unity/unity.h"

#include <stdlib.h>
#include <string.h>

#include "../../src/a_token.h"
#include "../../src/a_scanner.h"
#include "../../src/files.h"

static void test_only_comment() {
	scanner s;
	const char src[] = "/ From: MCS-4 Assembly Language Programming Manual\n"
		"/ Write the value 2 into status character 1 of DATA RAM register 0  of chip 0\n"
		"/   of the currently selected DAT RAM bank;\n";
	scan_tokens(&s, src, sizeof(src));
	TEST_ASSERT_EQUAL_INT(0, s.current_token);
	free(s.tokens);
}

// This gennerate a test for all OPCODE
#define X(code, value, name, words) static void test_only_##code () { \
	scanner s; \
	const char input[] = ""#code""; \
	scan_tokens(&s, input, strlen(input)); \
	TEST_ASSERT_EQUAL_INT(1, s.current_token); \
	TEST_ASSERT_EQUAL_INT(TOKEN_TYPE_OP_##code, s.tokens[0].type); \
	free(s.tokens); \
}
X_OPCODE
#undef X

static void test_FIM() {
	scanner s;
	scan_tokens(&s, "FIM 2P 5", 9);
	TEST_ASSERT_EQUAL_INT(3, s.current_token);
	TEST_ASSERT_EQUAL_INT(TOKEN_TYPE_OP_FIM, s.tokens[0].type);
	TEST_ASSERT_EQUAL_INT(TOKEN_TYPE_REGISTER_PAIR, s.tokens[1].type);
	TEST_ASSERT_EQUAL_INT(TOKEN_TYPE_NUMBER, s.tokens[2].type);
	free(s.tokens);
}

static void test_label_declaration() {
	scanner s;
	scan_tokens(&s, "label, NOP", 10);
	TEST_ASSERT_EQUAL_INT(2, s.current_token);
	TEST_ASSERT_EQUAL_INT(TOKEN_TYPE_LABEL_DECLARATION, s.tokens[0].type);
	TEST_ASSERT_EQUAL_INT(TOKEN_TYPE_OP_NOP, s.tokens[1].type);
	free(s.tokens);
}

static void test_scan_RDn() {
	long f_size;
	const char* source = read_file("./roms/src/RDn.s", &f_size);
	scanner s;
	scan_tokens(&s, source, f_size);
	TEST_ASSERT_EQUAL_INT(6, s.current_token);
	TEST_ASSERT_EQUAL_INT(TOKEN_TYPE_OP_FIM, s.tokens[0].type);
	TEST_ASSERT_EQUAL_INT(TOKEN_TYPE_REGISTER_PAIR, s.tokens[1].type);
	TEST_ASSERT_EQUAL_INT(TOKEN_TYPE_NUMBER, s.tokens[2].type);
	TEST_ASSERT_EQUAL_INT(TOKEN_TYPE_OP_SRC, s.tokens[3].type);
	TEST_ASSERT_EQUAL_INT(TOKEN_TYPE_REGISTER_PAIR, s.tokens[4].type);
	TEST_ASSERT_EQUAL_INT(TOKEN_TYPE_OP_RD3, s.tokens[5].type);
	free(s.tokens);
}

