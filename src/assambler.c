#include "./assambler.h"

void parser(const char* source_path) {
	const char* source = read_file(source_path);

	printf(source);
}
