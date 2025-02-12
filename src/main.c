#include <stdbool.h>

#include "4004_chip.h"
#include "application.h"
#include "options_parser.h"
#include "error.h"

int main(int argc, char **argv){
	Options options;
	Error err_opt = options_parser(argc, argv, &options);
	if(err_opt != ERROR_NOT) {
		LOG_ERROR_ARGS("Fail to parse options: %d\n", err_opt);
		return err_opt;
	}

	chip_4004 myChip;
	init4004(&myChip);

	application_init(options, &myChip);

	application_main_loop(&myChip);
	// cli_d_print_rom_memory(&myChip);
	return 0;
}
