#include "../vendor/unity/unity_internals.h"
#include "./assembler.c"

void setUp() {}

void tearDown() {}

#define SF(src_name, byte_size, expected_binary) RUN_TEST(test_assembler_rom_##src_name);

int main() {
	UNITY_BEGIN();

	ASSEMBLER_TESTS

	return UNITY_END();
}
#undef SF
