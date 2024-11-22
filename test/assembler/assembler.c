#include "../vendor/unity/unity.h"

#include <stdlib.h>
#include <inttypes.h>

#include "../../src/assembler.h"
#include "../../src/options_parser.h"
#include "../../src/error.h"
#include "../../src/files.h"

#define SF_ARRAY(...) { __VA_ARGS__ }

#define ASSEMBLER_TESTS  \
	SF(FIN,             1, SF_ARRAY( 0x34 ) ) \
	SF(JCN,             2, SF_ARRAY( 0x13, 0xF0 ) ) \
	SF(JIN,             1, SF_ARRAY( 0x35 ) ) \
	SF(JMS,             2, SF_ARRAY( 0x50, 0xF0 ) ) \
	SF(JUN,             2, SF_ARRAY( 0x40, 0xF0 ) ) \
	SF(Lables,          8, SF_ARRAY( 0x00, 0x40, 0x04, 0x00, 0xF2, 0x00, 0x40, 0x04 )) \
	SF(NOP,             1, SF_ARRAY( 0x00 ) )\
	SF(RDn,             4, SF_ARRAY( 0x24, 0x05, 0x25, 0xEF ) )\
	SF(WRM,             5, SF_ARRAY( 0x20, 0xB4, 0x21, 0xDF, 0xE0 ) )\
	SF(WRR,             5, SF_ARRAY( 0x28, 0x40, 0x29, 0xDF, 0xE2 ) )\
	SF(WRn,             5, SF_ARRAY( 0x20, 0x00, 0x21, 0xD2, 0xE5 ) )\
	SF(ram_status_test, 1, SF_ARRAY( 0x20 )) \
	/* SF(ram_test,        1) */

#define SF(src_name, byte_size, expected_binary) static void test_assembler_rom_##src_name () { \
	Options opt = { \
		.mode = ASSEMBLER, \
		.output_file_path = "assembled_roms/" ""#src_name"", \
		.in_file_path = "./roms/src/" ""#src_name"" ".s" \
	}; \
	Error e = assembler(&opt); \
	TEST_ASSERT_EQUAL_INT_MESSAGE(ERROR_NOT, e, "Assembler return an error when compiling test '"""#src_name""".s' "); \
	long file_size = 0; \
	uint8_t expected_content[(byte_size)] = expected_binary; \
	uint8_t *file_content = (uint8_t *) b_read_file(opt.output_file_path, &file_size); \
	TEST_ASSERT_EQUAL_INT((byte_size), file_size); \
	for(int i = 0; i < (byte_size); i++) { \
		TEST_ASSERT_EQUAL_INT(expected_content[i], file_content[i]); \
	} \
	free(file_content); \
}

ASSEMBLER_TESTS
#undef SF

