#include "../vendor/unity/unity_internals.h"
#include "./assembler.c"

void setUp() {}

void tearDown() {}

int main() {
	UNITY_BEGIN();
	RUN_TEST(test_assembler_rom_FIN);
	RUN_TEST(test_assembler_rom_JUN);
	return UNITY_END();
}
